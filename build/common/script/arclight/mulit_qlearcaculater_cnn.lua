require "pacmancenter"
require "arclight";
require "utility"
require "torch";
require "dpnn"
require "rnn"
require "nn"


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


function qlearcaculater:Setup(seed, clusteragent)--这个函数不能返回，opt模式带来的问题
  
  self.centrcenode = clusteragent;
  local nThreads = torch.getnumthreads()
  torch.setnumthreads(nThreads)
  LOG("seed is "..seed.."thread num is "..nThreads);
  math.randomseed(seed);
  torch.manualSeed(seed);
  start_new_game();--初始化游戏  
  
  self.episode = 1;
  self.experiences = {};
  self.caculatedexperiences = {};
end

function qlearcaculater:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  collectgarbage();
end

function qlearcaculater:SetMeanstd(meanstd)
  self.meanstd = meanstd;
end

function qlearcaculater:SetEpisode(e)
  self.episode = e;
end

function qlearcaculater:SetSwitchFrequency(t)
  training_times = t;
end

function qlearcaculater:SetTrainingTimes(t)
  switch_frequency = t;
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
  batch = {}
  batch.reward = reward;
  batch.action = action;
  batch.isterminal = isterminal;
  batch.next_state = next_state;  
  batch.current_state = current_state;
  table.insert(self.experiences, batch);
end

--得到动作
function qlearcaculater:_GetAction(explorate, state)
   if math.random() > explorate then
    local out = self.neuronnet:forward(state);--通过神经网络
    if nil == self.outstring then
      self.outstring = string.format("%.4f\t%.4f\t%.4f\t%.4f", out[1],out[2],out[3],out[4]);
    end    
    local y, i = torch.max(out, 1);
    return i[1];
  else
    return math.random(action_count());
  end
end

--开始训练
function qlearcaculater:Trainning(its, basescale, sf, tt, tbs, mep, ed, em, er)
  self.basescale = basescale;
  print(basescale);
  switch_frequency = sf;
  training_times = tt;
  training_batch_size = tbs;
  max_experience_pool = mep;
  experiences_discount = ed;
  exploration_min = em;
  exploration_rate = er;
  self.current_state = get_pac_state(self.meanstd);
  for episode = self.episode, its do
    local episodereward = 0
    self.experiences = {};
    local explorate = self:_GetExplorationRate(episode);
    WARNING(string.format("Begin %d episode explorate %.3f", episode, explorate));
    for stepcount=1, max_experience_pool do
      self.neuronnet:evaluate();
      local action = self:_GetAction(explorate, self.current_state);
      local reward, terminal = keypressed_playing(action);
      episodereward = episodereward + reward;
      local next_state = get_pac_state(self.meanstd);
      self:_InsertExperience(self.current_state, next_state, action, reward, terminal);
      self.current_state = next_state;
    end
    collectgarbage();
    self:_FullTraining();
    WARNING(string.format("end %d episode reward %d", episode, episodereward));
    self.centrcenode:EpsiodeDone(episode, episodereward);
  end
end

function qlearcaculater:_CaculateExperiences()
  self.caculatedexperiences = {};  
  local current_out = nil;
  for _,v in ipairs(self.experiences) do
    if current_out == nil then
      current_out = self.target_q:forward(v.current_state):clone();
    end  
    local updatereward = current_out:clone();--当前的next实际上是这一组数据的current
    current_out = self.target_q:forward(v.next_state):clone();
    if batch.isterminal then
        updatereward[{v.action}] = v.reward;
      else
        updatereward[{v.action}] = v.reward + experiences_discount * torch.max(current_out);
    end
    batch = { input = v.current_state, target = updatereward }
    table.insert( self.caculatedexperiences, batch );
  end
end

function qlearcaculater:_TrySwitchTargetQ(times)
  if 0 == ((times-1) % switch_frequency) then
    --LOG("_TrySwitchTargetQ "..times);
    self.target_q = self.neuronnet:clone();
    self:_CaculateExperiences();
  end    
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
  collectgarbage();
  self.resumer = Context:GetServiceContext();
  self.resumer:Pasue();
  return loss;
end

function qlearcaculater:_FullTraining()
  for i=1,training_times do
    local loss = 0;
    local scale = 1 / (max_experience_pool / training_batch_size);
    local shuffledIndices = torch.randperm(max_experience_pool, 'torch.LongTensor');
    self:_TrySwitchTargetQ(i);
    for i=1, max_experience_pool, training_batch_size do
      local partdata = {}
      for j=1, training_batch_size do
        local index = i - 1 + j;
        index = shuffledIndices[index];
        table.insert(partdata, self.caculatedexperiences[index]);
      end
      loss = loss + self:_doTrainning(partdata) * scale;
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