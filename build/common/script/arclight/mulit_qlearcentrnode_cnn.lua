local _ENABLE_TENSOR_BOARD = false;

local crayon = require "crayon"
require "arclight";
require "utility"
require "torch";
require "dpnn"
require "rnn"
require "nn"

local switch_frequency = 20; --目标网络与训练网络切换频率 
local training_times = 60;
local training_batch_size = 100   --一次随机梯度下降计算的个数
local max_experience_pool = training_batch_size * 100 --经验池大小
local experiences_discount = 0.96 --经验递减
local learning_rate = 0.001        --学习速率
local learning_min = 0.00001
local learning_decay = 400
local exploration_min = 0.05       --最低探索率
local exploration_rate = 0.98  --探索率随迭代次数的衰减

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
  if _ENABLE_TENSOR_BOARD then
    self.cc = crayon.CrayonClient("10.12.192.197", 8889);
    self.cc:remove_experiment("cnndqn");
    self.tensorboard = self.cc:create_experiment("cnndqn");
  end  
   
  self.episodereward = 0;
  self.nodescount = 0;
  self.clusternodes = {};
  
  self.episode = 1;
  self.insertindex = 1;
  self.poolcount = 0;
  self.partionexperiences = {};
  
  --初始化adam  
  self.adamax = {}
  self.adamax.beta1 = 0.9
  self.adamax.beta2 = 0.999
  self.adamax.epsilon = 1e-38
  
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dqn_cnn_lite_1_15_15.lua");
  loadstring(self.neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  
  self.savefile = IFileSystem:PathAssembly("docs:save/pac_net_lite.tensor");
  
  self.meanstd = IFileSystem:PathAssembly("comm:document/pacman_lite.table");
  self.meanstd = torch.load(self.meanstd);
 
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
  node:SetMeanstd(self.meanstd);
  node:SetNeuronCriterion(self.neuronmaker);
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
  LOG("save adam"..adampath);
  torch.save(adampath, self.adamax);
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
  self.adamax = torch.load(adampath);    
end

function qlearcentrnode:SetLearningRate(max, min)
  LOG(string.format("set learning rate max %f min %f, was max %f min %f", max, min, learning_rate, learning_min));
  learning_rate = max;
  learning_min = min;
end

function qlearcentrnode:SetEpisode(e)  
  self.episode = e;
  for _, node in pairs(self.clusternodes) do
    node:SetEpisode(e);
  end
end

function qlearcentrnode:SetTrainingTimes(t)
  LOG(string.format("set training times %d, was %d", t, training_times));
  training_times = t;
  for _, node in pairs(self.clusternodes) do
    node:SetTrainingTimes(t);
  end
end

function qlearcentrnode:SetSwitchFrequency(t)
  LOG(string.format("set switch frequency %d, was %d", t, switch_frequency));
  switch_frequency = t;
  for _, node in pairs(self.clusternodes) do
    node:SetSwitchFrequency(t);
  end
end

--开始训练
function qlearcentrnode:Trainning(lr, its)
  learning_rate = lr;
  local scale = 1 / self.nodescount;
  local ps,pb = self:_SerializeParameters();
  for _, node in pairs(self.clusternodes) do
    node:SynchronyNeuronnet(ps, pb);
    node:Trainning(its, scale, switch_frequency, training_times, training_batch_size, max_experience_pool, experiences_discount, exploration_min, exploration_rate);
  end
  self.epsiodedonecount = 0;
  while true do
    self.loss = 0;
    self.collectcount = 0;
    self.currentlearningRate =
      learning_min + (learning_rate-learning_min)
      *math.max( (1-(self.episode-1)/learning_decay), 0); 
    self.neuronnet:zeroGradParameters();
    self.resumer = Context:GetServiceContext();
    stepsize, grad_scale = self.resumer:Pasue();
    LOG(string.format("trainning episode %d grad scale %.10f learning rate %.6f loss %.4f", self.episode, grad_scale, stepsize, self.loss));
    if _ENABLE_TENSOR_BOARD then
      self.tensorboard:add_scalar_value("loss", self.loss);
    end    
    local ps,pb = self:_SerializeParameters();
    for _, node in pairs(self.clusternodes) do
      node:SynchronyNeuronnet(ps, pb);
    end
  end
end

function qlearcentrnode:EpsiodeDone(episode, episodereward)
  self.epsiodedonecount = self.epsiodedonecount + 1;  
  self.episodereward = self.episodereward + episodereward;
  if self.epsiodedonecount == self.nodescount then
    LOG("total reward "..tostring(self.episodereward));
    if _ENABLE_TENSOR_BOARD then
      self.tensorboard:add_scalar_value("reward", self.episodereward);
    end    
    self:Save();
    self.epsiodedonecount = 0;
    self.episodereward = 0
    self.episode = self.episode + 1;
  end
end

function qlearcentrnode:CollectResult(bufferstring, loss, gs, gb)
  self.collectcount = self.collectcount + 1;
  self.loss = self.loss + loss;
  local ow, ogw = self.neuronnet:getParameters();
  local nogw = arclight.Message2Tensor(gs, gb);
  ogw:add(nogw);  
  if nil ~= bufferstring then LOG(bufferstring); end
  if self.collectcount == self.nodescount then
    
    self.adamax.t = self.adamax.t or 0;
    self.adamax.m = self.adamax.m or ow.new(ogw:size()):zero();
    self.adamax.u = self.adamax.u or ow.new(ogw:size()):zero();
    self.adamax.max = self.adamax.max or ow.new(2, unpack(ogw:size():totable())):zero();
    self.adamax.t = self.adamax.t + 1;
    self.adamax.m:mul(self.adamax.beta1):add(1-self.adamax.beta1, ogw);
    self.adamax.max[1]:copy(self.adamax.u):mul(self.adamax.beta2);
    self.adamax.max[2]:copy(ogw):abs():add(self.adamax.epsilon);
    self.adamax.u:max(self.adamax.max, 1);
    local biasCorrection1 = 1 - self.adamax.beta1^self.adamax.t;
    local stepSize = self.currentlearningRate/biasCorrection1;
    ogw:copy(self.adamax.m):cdiv(self.adamax.u);
    ow:addcdiv(-stepSize, self.adamax.m, self.adamax.u);
    
    local w_scale = torch.norm(ow);
    local g_scale = stepSize*torch.norm(ogw);
    local grad_scale = g_scale/w_scale;
    self.resumer:Resume(stepSize, grad_scale);
  end  
end

function qlearcentrnode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

return qlearcentrnode;