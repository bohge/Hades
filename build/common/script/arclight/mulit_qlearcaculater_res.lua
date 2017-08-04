require "arclight";
require "utility"
require "pacman"
require "torch";
require "dpnn"
require "rnn"
require "nn"

local training_times;
local training_package_size;
local training_batch_size;
local max_experience_pool;
local experiences_discount;
local exploration_min;
local exploration_rate;

local qlearcaculater = torch.class('arclight.qlearcaculater');
qlearcaculater.resumer = nil;

qlearcaculater.partionexperiences = nil; --最新的部分经验
qlearcaculater.recevcaculatedexp = nil;--接收到的计算后的经验数据
qlearcaculater.experiences = nil;  --经验池
qlearcaculater.insertindex = nil;  --插入位置
qlearcaculater.poolcount = nil;    --池总经验大小
qlearcaculater.current_state = nil; --游戏状态
qlearcaculater.meanstd = nil;
qlearcaculater.episode = nil; --经过的迭代次数


function qlearcaculater:Setup(seed, myself)--这个函数不能返回，opt模式带来的问题
  LOG("seed is "..seed);
  math.randomseed(seed);
  torch.manualSeed(seed);
  start_new_game();--初始化游戏
  
  self.centrcenode = myself;
  
  self.insertindex = 1;
  self.poolcount = 0;
  self.partionexperiences = {};
  self.experiences = {};
end

function qlearcaculater:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.neuronnet:float();
  self.criterion:float();
  self.target_q = self.neuronnet:clone();
  collectgarbage();
end

function qlearcaculater:SetMeanstd(meanstd)
  self.meanstd = meanstd;
end

function qlearcaculater:SynchronyNeuronnet(ps, pb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
  if self.resumer then
    self.resumer:Resume();
  end
  collectgarbage();
end

--根据迭代次数衰减
function qlearcaculater:_GetExplorationRate(episode)
  return math.max(exploration_min, math.pow(exploration_rate, episode));
end

--插入经验
function qlearcaculater:_InsertExperience(current_state, next_state, action, reward, isterminal)
  batch = self.experiences[self.insertindex];
  if self.current_out == nil then
    self.current_out = self.target_q:forward(current_state):clone();
  end  
  local updatereward = self.current_out:clone();--当前的next实际上是这一组数据的current
  self.current_out = self.target_q:forward(next_state):clone();
  if isterminal then
      updatereward[{1, action}] = reward;
    else
      local maxnext, index = torch.max(self.current_out,2);
      updatereward[{1, action}] = reward + experiences_discount * maxnext[1][1];
  end
  batch.input = current_state;
  batch.target = updatereward;
  
  self.poolcount = math.min( self.poolcount + 1, max_experience_pool );
  self.insertindex = self.insertindex + 1 > max_experience_pool and 1 or self.insertindex + 1;
  table.insert(self.partionexperiences, batch);--插入到最新的部分经验池中
end

--得到动作
function qlearcaculater:_GetAction(explorate, state)
   if math.random() > explorate then
    local out = self.neuronnet:forward(state);--通过神经网络
    --LOG(tostring(out:view(1,out:size(1))));
    if nil == self.outstring then
      self.outstring = string.format("\t%.4f\t%.4f\t%.4f\t%.4f", out[{1,1}],out[{1,2}],out[{1,3}],out[{1,4}]);
    end    
    local y, i = torch.max(out, 2);
    return i[1][1];
  else
    return math.random(action_count());
  end
end

--开始训练
function qlearcaculater:Trainning(its, basescale, tt, tps, tbs, mep, ed, em, er)
  training_times = tt;
  training_package_size = tps;
  training_batch_size = tbs;
  max_experience_pool = mep;
  experiences_discount = ed;
  exploration_min = em;
  exploration_rate = er;
  self.basescale = basescale;
  for i=1, max_experience_pool do
    local batch = {};
    table.insert(self.experiences, batch);
  end
  self.current_state = get_pac_state(self.meanstd):view(1,get_pac_state_size());
  for episode = 1, its do
    local episodereward = 0
    local explorate = self:_GetExplorationRate(episode);
    WARNING(string.format("Begin %d episode explorate %.3f", episode, explorate));
    for stepcount=1, max_experience_pool do
      self.neuronnet:evaluate();
      local action = self:_GetAction(explorate, self.current_state);
      local reward, terminal = keypressed_playing(action);
      episodereward = episodereward + reward;
      local next_state = get_pac_state(self.meanstd):view(1,get_pac_state_size());
      self:_InsertExperience(self.current_state, next_state, action, reward, terminal);
      self.current_state = next_state;
    end
    self:_FullTraining();
    WARNING(string.format("end %d episode reward %d", episode, episodereward));
    self.target_q = self.neuronnet:clone();
    self.centrcenode:EpsiodeDone(episode);
  end
end

function qlearcaculater:_makeBatch(partdata)
  local batchs = {};
  local simplecount = #partdata;
  self.neuronnet:evaluate();--得到历史状态需要使用这个模式
  assert( simplecount % training_package_size == 0 );
  for i=1, simplecount, training_package_size do
    local data = {}
    data.input = torch.FloatTensor(training_package_size, get_pac_state_size());
    data.target = torch.FloatTensor(training_package_size, action_count());
    for j=1, training_package_size do--合批
      local index = ( i - 1 ) + j;
      local exps = partdata[index];
      data.input[{ j, {}, {}, {} }]:copy(exps.input);
      data.target[{ j, {} }]:copy(exps.target);
    end
    table.insert(batchs, data);
  end
  return batchs;
end

function qlearcaculater:_doTrainning(partdata)
  local loss = 0;
  local batchsize = #partdata;
  local accscale = self.basescale / batchsize;
  self.neuronnet:training();
  self.neuronnet:zeroGradParameters();
  for i, data in ipairs(partdata) do
    self.neuronnet:forward(data.input);
    loss = loss + self.criterion:forward(self.neuronnet.output, data.target) * accscale;--计算误差
    local grad = self.criterion:backward(self.neuronnet.output, data.target);--计算梯度
    self.neuronnet:backward(data.input, grad, accscale)
  end
  local gs, gb = self:_GetGradBuffer();
  self.centrcenode:CollectResult(self.outstring, loss, gs, gb);
  self.outstring = nil;
  self.resumer = Context:GetServiceContext();  
  self.resumer:Pasue();
  collectgarbage();
  return loss;
end

function qlearcaculater:_FullTraining()
  for i=1,training_times do
    local sgd_simple_count = training_package_size * training_batch_size;
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
      local batchdatas = self:_makeBatch(partdata);
      loss = loss + self:_doTrainning(batchdatas) * scale;
    end
    LOG(string.format("full %d training loss %.4f", i, loss));
  end  
end

function qlearcaculater:_GetGradBuffer()
  local _, g = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(g);
  return struct, buffer;
end

return qlearcaculater;