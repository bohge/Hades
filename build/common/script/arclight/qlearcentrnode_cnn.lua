require "arclight";
require "utility"
require "pacman"
require "torch";
require "dpnn"
require "rnn"
require "nn"

local training_times = 20;
local training_batch_size = 1000   --一次随机梯度下降计算的个数
local training_frequency = training_batch_size --每经过多少帧训练一次
local max_experience_pool = training_batch_size * 2000 --经验池大小
local experiences_discount = 0.98 --经验递减
local learning_rate = 0.0001        --学习速率
local exploration_min = 0.1       --最低探索率
local exploration_rate = 0.99  --探索率随迭代次数的衰减

local qlearcentrnode = torch.class('arclight.qlearcentrnode');
qlearcentrnode.clusternodes = nil;
qlearcentrnode.nodescount = nil;
qlearcentrnode.collectcount = nil;
qlearcentrnode.resumer = nil;

qlearcentrnode.partionexperiences = nil; --最新的部分经验
qlearcentrnode.recevcaculatedexp = nil;--接收到的计算后的经验数据
qlearcentrnode.experiences = nil;  --经验池
qlearcentrnode.insertindex = nil;  --插入位置
qlearcentrnode.poolcount = nil;    --池总经验大小
qlearcentrnode.current_state = nil; --游戏状态
qlearcentrnode.meanstd = nil;
qlearcentrnode.episode = nil; --经过的迭代次数

qlearcentrnode.adamstate = nil;--adam状态

function qlearcentrnode:Setup()
  torch.manualSeed(851022);
  start_new_game();--初始化游戏
  
  self.nodescount = 0;
  self.clusternodes = {};
  
  self.insertindex = 1;
  self.poolcount = 0;
  self.partionexperiences = {};
  
    --初始化adam
  self.adamstate = {};
  self.adamstate.t = 0;
  self.adamstate.beta1 = 0.9;
  self.adamstate.beta2 = 0.995;
  self.adamstate.epsilon = 1e-7;
  
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dqn_cnn_1_22_19.lua");
  loadstring(self.neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.neuronnet:float();
  self.criterion:float();
  
  self.savefile = IFileSystem:PathAssembly("docs:save/pac_net.tensor");
  
  self.meanstd = IFileSystem:PathAssembly("comm:document/pacman.table");
  self.meanstd = torch.load(self.meanstd);
  
  --初始状态
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
  
  LOG('neuron network is ' .. self.neuronnet:__tostring());
  LOG('criterion is ' .. self.criterion:__tostring());
  LOG('save file path is ' .. self.savefile);
  LOG("initialize experiences done");
  return true;
end

--插入计算节点
function qlearcentrnode:InsertComputernode(strgid, node)
  assert(nil == self.clusternodes[strgid]);
  self.clusternodes[strgid] = node;
  self.nodescount = self.nodescount + 1;
  node:SetNeuronCriterion(self.neuronmaker);
  node:SetParameters(experiences_discount, action_count(), get_pac_state_size());
  return self.nodescount;
end

function qlearcentrnode:RemoveComputernode(strgid)
  assert(nil ~= self.clusternodes[strgid]);
  self.clusternodes[strgid] = nil;
  self.nodescount = self.nodescount - 1;
  return self.nodescount;
end

function qlearcentrnode:Save()
  LOG("save "..self.savefile);
  self.neuronnet:clearState();
  torch.save(self.savefile, self.neuronnet);  
  local adampath = self.savefile .. "_adame";
  LOG("save adam"..self.adampath);
  torch.save(adampath, self.adamstate);
  if self.episode then
    local backpath = self.savefile .. "_episode_" .. self.episode;
    LOG("save "..backpath);
    torch.save(backpath, self.neuronnet);
  end  
end

function qlearcentrnode:Load()
  LOG("Load "..self.savefile);
  self.neuronnet = torch.load(self.savefile);  
  local adampath = self.savefile .. "_adame";
  LOG("Load adam"..adampath);
  self.adamstate = torch.load(adampath);    
end

--插入经验
function qlearcentrnode:_InsertExperience(current_state, next_state, action, reward, isterminal)
  batch = self.experiences[self.insertindex];
  batch.current_state = current_state;
  batch.next_state = next_state;
  batch.action = action;
  batch.reward = reward;
  batch.isterminal = isterminal;
  self.poolcount = math.min( self.poolcount + 1, max_experience_pool );
  self.insertindex = self.insertindex + 1 > max_experience_pool and 1 or self.insertindex + 1;
  table.insert(self.partionexperiences, batch);--插入到最新的部分经验池中
end

--是需要进行训练
function qlearcentrnode:_isNeedTrainning(step)
  return step % training_frequency == 0;--按照一定的频率更新
end

--根据迭代次数衰减
function qlearcentrnode:_GetExplorationRate(episode)
  return math.max(exploration_min, math.pow(exploration_rate, episode));
end

--得到动作
function qlearcentrnode:_GetAction(explorate, state)
   if math.random() > explorate then
    self.neuronnet:evaluate();
    local out = self.neuronnet:forward(state);--通过神经网络
    LOG(tostring(out:view(1,out:size(1))));
    local y, i = torch.max(out, 1);
    return i[1];
  else
    return math.random(action_count());
  end
end

function qlearcentrnode:SetLearningRate(lr)
  LOG(string.format("set learning rate %f, was %f", lr, learning_rate));
  learning_rate = lr;
end

--开始训练
function qlearcentrnode:Trainning(lr, its)
  learning_rate = lr;
  for episode = 1, its do
    self.episode = episode;
    local episodereward = 0
    local explorate = self:_GetExplorationRate(episode);
    WARNING(string.format("Begin %d episode explorate %.3f", episode, explorate));
    for stepcount=1, max_experience_pool do
      local action = self:_GetAction(explorate, self.current_state);
      local reward, terminal = keypressed_playing(action);
      episodereward = episodereward + reward;
      local next_state = get_pac_state(self.meanstd);
      self:_InsertExperience(self.current_state, next_state, action, reward, terminal);
      self.current_state = next_state;
      self:_PortionTraining(stepcount);
    end
    self:_FullTraining();
    WARNING(string.format("end %d episode reward %d", episode, episodereward));
    self:Save();
  end  
end

--部分训练，经过一定的step后训练一次,将过去的新状态拿来训练
function qlearcentrnode:_PortionTraining(stepcount)
  if self:_isNeedTrainning(stepcount) then
    for i=1,training_times do
      local loss, stepsize, grad_scale = self:_doTrainning(self.partionexperiences);
      LOG(string.format("partion %d times trainning grad scale %.5f learning rate %.5f loss %.4f", i, grad_scale, stepsize, loss));
    end    
    self.partionexperiences = {};--清理
  end  
end

function qlearcentrnode:_doTrainning(partdata)
  self.loss = 0;
  self.collectcount = 0;
  local scale = 1 / self.nodescount;
  assert( #partdata % self.nodescount == 0 );--必须能被整出
  local sendcount = #partdata / self.nodescount;
  local ps,pb = self:_SerializeParameters();
  local nodeindex = 1;
  for _, node in pairs(self.clusternodes) do
    local tosenddatas = {};
    for j=1, sendcount do
      local index = (nodeindex-1)*sendcount+j;
      local exp = partdata[index];
      local cs, cb = arclight.Tensor2Message(exp.current_state);
      local ns, nb = arclight.Tensor2Message(exp.next_state);
      local data = {};
      data.current_state = {cs, cb}
      data.next_state = {ns, nb}
      data.action = exp.action;
      data.reward = exp.reward;
      data.isterminal = exp.isterminal;
      table.insert(tosenddatas, data);
    end
    node:SynchronyNeuronnet(ps, pb);
    node:DistributedTraning(scale, tosenddatas);
    nodeindex = nodeindex + 1;
  end  
  self.neuronnet:zeroGradParameters();
  self.resumer = Context:GetServiceContext();
  local stepSize, grad_scale = self.resumer:Pasue();
  return self.loss, stepSize, grad_scale;
end

function qlearcentrnode:CollectResult(loss, gs, gb)
  self.collectcount = self.collectcount + 1;
  self.loss = self.loss + loss;
  local ow, ogw = self.neuronnet:getParameters();
  local nogw = arclight.Message2Tensor(gs, gb);
  ogw:add(nogw);
  
  if self.collectcount == self.nodescount then
    self.adamstate.t = self.adamstate.t + 1;    
    self.adamstate.m = self.adamstate.m or torch.FloatTensor(ogw:size()):zero();
    self.adamstate.v = self.adamstate.v or torch.FloatTensor(ogw:size()):zero();
    self.adamstate.denom = self.adamstate.denom or torch.FloatTensor(ogw:size()):zero();
    self.adamstate.m:mul(self.adamstate.beta1):add(1-self.adamstate.beta1, ogw);
    self.adamstate.v:mul(self.adamstate.beta2):addcmul(1-self.adamstate.beta2, ogw, ogw);    
    self.adamstate.denom:copy(self.adamstate.v):sqrt():add(self.adamstate.epsilon);    
    local biasCorrection1 = 1 - self.adamstate.beta1^self.adamstate.t
    local biasCorrection2 = 1 - self.adamstate.beta2^self.adamstate.t
    local stepSize = learning_rate * math.sqrt(biasCorrection2)/biasCorrection1;
    ow:addcdiv(-stepSize, self.adamstate.m, self.adamstate.denom);
    
    --local stepSize = learning_rate
    --self.neuronnet:updateParameters(stepSize);
    
    local w_scale = torch.norm(ow);
    local g_scale = torch.norm(learning_rate*ogw);
    local grad_scale = g_scale/w_scale;
    self.resumer:Resume(stepSize, grad_scale);
  end  
end

function qlearcentrnode:_FullTraining()
  for i=1,training_times do
    local sgd_simple_count = training_batch_size;
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
    LOG(string.format("full %d training loss %.4f", i, loss));
  end  
end

function qlearcentrnode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

return qlearcentrnode;