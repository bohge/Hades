require "utility"
require "pacman"
require "torch";
require "dpnn"
require "rnn"
require "nn"


local training_package_size = 32  --每一个数据集的第一维大小
local training_batch_size = 1000   --一次随机梯度下降计算的个数
local training_frequency = training_package_size * training_batch_size --每经过多少帧训练一次
local max_experience_pool = training_package_size * training_batch_size * 10 --经验池大小
local experiences_discount = 0.98 --经验递减
local experiences_learning_rate = 0.1--经验的学习速率
local learning_rate = 0.001        --学习速率
local exploration_min = 0.1       --最低探索率
local exploration_rate = 0.99  --探索率随迭代次数的衰减

local qlearning = {}
qlearning.partionexperiences = nil; --最新的部分经验
qlearning.experiences = nil;  --经验池
qlearning.insertindex = nil;  --插入位置
qlearning.poolcount = nil;    --池总经验大小
qlearning.current_state = nil; --游戏状态
qlearning.meanstd = nil;


function qlearning:Initialize()
  self.insertindex = 1;
  self.poolcount = 0;
  self.partionexperiences = {};
  
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dqn_resnet_1_22_19.lua");
  loadstring(self.neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  
  self.savefile = IFileSystem:PathAssembly("docs:save/pac_net.tensor");
  
  self.meanstd = IFileSystem:PathAssembly("comm:document/pacman.table");
  self.meanstd = torch.load(self.meanstd);
  
  --初始状态
  start_new_game();--初始化游戏
  self.current_state = get_pac_state(self.meanstd);
  
  --初始化lua内存
  LOG("initialize experiences list");
  self.experiences = {}
  for i=1, max_experience_pool do
    local batch =
    {
      current_state = "current_state",--占位
      next_state = "next_state",
      action = "action";
      reward = "reward";
    }
    table.insert(self.experiences, batch);
  end
  
  --[[LOG("initialize experiences data");
  for _, batch in ipairs(self.experiences) do
    batch.current_state = torch.Tensor(1, 21, 19);
    batch.next_state = torch.Tensor(1, 21, 19);
    batch.action = 0;
    batch.reward = 0;
  end]]--
  
  --测试神经网络速度
  --[[local state = get_pac_state();
  local begin = os.clock();
  state = state:view(1,1,22,19);
  for i=1, 1000 do
    self.neuronnet:forward(state)
  end  
  begin = os.clock() - begin;
  print(begin);]]--
  LOG("initialize experiences done");
  return true;
end

--插入经验
function qlearning:InsertExperience(current_state, next_state, action, reward)
  batch = self.experiences[self.insertindex];
  batch.current_state = current_state;
  batch.next_state = next_state;
  batch.action = action;
  batch.reward = reward;
  self.poolcount = math.min( self.poolcount + 1, max_experience_pool );
  self.insertindex = self.insertindex + 1 > max_experience_pool and 1 or self.insertindex + 1;
  table.insert(self.partionexperiences, batch);--插入到最新的部分经验池中
end

--是需要进行训练
function qlearning:isNeedTrainning(step)
  return step % training_frequency == 0;--按照一定的频率更新
end

--根据迭代次数衰减
function qlearning:GetExplorationRate(episode)
  return math.max(exploration_min, math.pow(exploration_rate, episode));
end

local clock = os.clock
function sleep(n)  -- seconds
  local t0 = clock()
  while clock() - t0 <= n do end
end

--得到动作
function qlearning:GetAction(explorate, state)
    local o = state:clone();
    o[{{},{}}]:mul(self.meanstd[2]);
    o[{{},{}}]:add(self.meanstd[1]);
    print(o[{{},{}}]);
   --if math.random() > explorate then
    self.neuronnet:evaluate();
    local out = self.neuronnet:forward(state);--通过神经网络
    print(out:view(1,4));
    local y, i = torch.max(out, 1);
    print(i[1]);
    return i[1];
  --[[else
    return math.random(action_count());
  end]]--
end

--得到一批训练数据
function qlearning:GetExperienceBatchs(partdata)
  local batchs = {};
  local simplecount = #partdata;
  self.neuronnet:evaluate();--得到历史状态需要使用这个模式
  assert( simplecount % training_package_size == 0 );
  for i=1, simplecount, training_package_size do
    local batchreward = torch.Tensor(training_package_size, action_count()):zero();
    local batchcurrent = torch.Tensor(training_package_size, get_pac_state_size());
    local batchcaculate = torch.Tensor(training_package_size+1, get_pac_state_size());--+1为了一次计算
    local batchaction = torch.Tensor(training_package_size, action_count()):zero();
    local batchinvaction = torch.Tensor(training_package_size, action_count()):fill(1);
    for j=1, training_package_size do--合批
      local index = ( i - 1 ) + j;
      local exps = partdata[index];
      batchcurrent[{ j, {}, {}, {} }]:copy(exps.current_state);
      batchcaculate[{ j, {}, {}, {} }]:copy(exps.current_state);
      if training_package_size == j then
        batchcaculate[{ j+1, {}, {}, {} }]:copy(exps.next_state);
      end      
      batchreward[{j, exps.action}] = exps.reward;
      batchaction[{j, exps.action}] = 1;
      batchinvaction[{j, exps.action}] = 0;
    end
    --Q(S,A) ← (1-α)*Q(S,A) + α*[R + γ*maxQ(S',a)]
    local rewardcaculatebatch = self.neuronnet:forward(batchcaculate);
    local rewardcurrent = rewardcaculatebatch[{{1, training_package_size},{}}];
    local rewardnext = rewardcaculatebatch[{{2, training_package_size+1},{}}];
    local maxinrow = torch.max(rewardnext,2);
    local expandmaxrow = torch.expand(maxinrow, maxinrow:size(1), batchaction:size(2));
    local addhistry = torch.cmul(batchaction, expandmaxrow);
    local oldmultipar = (1-experiences_learning_rate)*batchaction + batchinvaction;
    local updatereward = 
      torch.cmul(oldmultipar, rewardcurrent)
      + torch.cmul(experiences_learning_rate*batchaction,
        (batchreward+experiences_discount*addhistry));
    local pack =
    {
      input = batchcurrent,
      target = updatereward,
    }
    table.insert(batchs, pack);
  end
  return batchs;
end

function qlearning:_doTrainning(partdata)
  local batchs = self:GetExperienceBatchs(partdata);
  --训练
  local loss = 0;
  local scale = 1 / #batchs;
  self.neuronnet:training();
  self.neuronnet:zeroGradParameters();
  for i, data in ipairs(batchs) do
    self.neuronnet:forward(data.input);
    loss = loss + self.criterion:forward(self.neuronnet.output, data.target);--计算误差
    local grad = self.criterion:backward(self.neuronnet.output, data.target);--计算梯度
    self.neuronnet:backward(data.input, grad, scale)
  end
  self.neuronnet:updateParameters(learning_rate);
  return loss * scale;
end


--部分训练，经过一定的step后训练一次,将过去的新状态拿来训练
function qlearning:PortionTraining(stepcount)
  if self:isNeedTrainning(stepcount) then
    local loss = self:_doTrainning(self.partionexperiences);
    LOG(string.format("partion trainning loss %.4f", loss));
    self.partionexperiences = {};--清理
  end  
end

function qlearning:FullTraining()
  local sgd_simple_count = training_batch_size * training_package_size;
  local scale = max_experience_pool / sgd_simple_count;
  local shuffledIndices = torch.randperm(max_experience_pool, 'torch.LongTensor')
  local loss = 0;
  for i=1, max_experience_pool, sgd_simple_count do
    local partdata = {}
    for j=1, sgd_simple_count do
      local index = i - 1 + j;
      index = shuffledIndices[index];
      table.insert(partdata, self.experiences[index]);      
    end
    loss = loss + self:_doTrainning(partdata) * scale;
  end  
  LOG(string.format("full training loss %.4f", loss));
end

function qlearning:Trainning()
  _COROUTINES();
  for episode = 1, 1000000 do
    local episodereward = 0
    local explorate = self:GetExplorationRate(episode);
    WARNING(string.format("Begin %d episode explorate %.3f", episode, explorate));
    for stepcount=1, max_experience_pool do
      local action = self:GetAction(explorate, self.current_state);
      local reward, isterminal = keypressed_playing(action);
      print("reward "..reward);
      print("-------------------------------");
      episodereward = episodereward + reward;
      local next_state = get_pac_state(self.meanstd);
      self:InsertExperience(self.current_state, next_state, action, reward);
      self.current_state = next_state;
      self:PortionTraining(stepcount);      
      sleep(1);
    end
    self:FullTraining();
    WARNING(string.format("end %d episode reward %d", episode, episodereward));
    self:Save();
  end  
end

function qlearning:Save()
  LOG("save "..self.savefile);
  self.neuronnet:clearState();
  torch.save(self.savefile, self.neuronnet);  
end

function qlearning:Load()
  LOG("Load "..self.savefile);
  self.neuronnet = torch.load(self.savefile);  
end


return qlearning;