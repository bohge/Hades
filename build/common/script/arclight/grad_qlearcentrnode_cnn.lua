local _ENABLE_TENSOR_BOARD = true

local crayon = require "crayon"
require "arclight";
require "utility"
require "torch";
require "nn"

local switch_frequency = 30; --目标网络与训练网络切换频率 
local training_times = 60;
local training_batch_size = 1000   --一次随机梯度下降计算的个数
local max_experience_pool = training_batch_size * 10 --经验池大小
local experiences_discount = 0.9 --经验递减
local learning_rate = 0.0001        --学习速率
local learning_min = 0.000001
local learning_decay = 400
local exploration_min = 0.05       --最低探索率
local exploration_rate = 0.05  --探索率随迭代次数的衰减

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


function qlearcentrnode:Setup()
  if _ENABLE_TENSOR_BOARD then
    self.cc = crayon.CrayonClient("10.12.192.197", 8889);
    self.cc:remove_experiment("gdqn_cnn");
    --self.tensorboard = self.cc:open_experiment("gdqn_cnn");
    self.tensorboard = self.cc:create_experiment("gdqn_cnn");
  end
  
  
  self.episodereward = 0;
  self.nodescount = 0;
  self.clusternodes = {};
  
  self.episode = 1;
  self.insertindex = 1;
  self.poolcount = 0;
  self.partionexperiences = {};
  
  --self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dqn_cnn_lite_1_15_15.lua");
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dqn_cnn_1_22_19.lua");
  loadstring(self.neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();  
  --self.savefile = IFileSystem:PathAssembly("docs:save/pac_net_lite.tensor");  
  --self.meanstd = IFileSystem:PathAssembly("comm:document/pacman_lite.table");
  self.savefile = IFileSystem:PathAssembly("docs:save/pac_net.tensor");  
  self.meanstd = IFileSystem:PathAssembly("comm:document/pacman.table");
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
  torch.save(adampath, self.adamstate);
  if self.episode then
    local backpath = self.savefile .. "_episode_" .. self.episode .. "_reward_" .. self.episodereward;
    LOG("save "..backpath);
    torch.save(backpath, self.neuronnet);
  end  
end

function qlearcentrnode:Load()
  LOG("Load "..self.savefile);
  self.neuronnet = torch.load(self.savefile);
  self.neuronnet:double();
  --local adampath = self.savefile .. "_adame";
  --LOG("Load adam"..adampath);
  --local adam = torch.load(adampath);
  --self.adamax = adam or self.adamax;  
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

--开始训练
function qlearcentrnode:Trainning(lr, its)
  learning_rate = lr;
  LOG("begin q-learning learning rate is "..lr);
  local ps,pb = self:_SerializeParameters();
  for _, node in pairs(self.clusternodes) do
    node:SetLearningRate(learning_rate);
    node:SynchronyNeuronnet(ps, pb);
    node:Trainning(its, switch_frequency, training_times, training_batch_size, max_experience_pool, experiences_discount, exploration_min, exploration_rate);
  end  
  self.steploss = 0;
  self.stepcollectcount = 0;
  for episode = self.episode, its do
    self.loss = 0;
    self.step_size = 0;
    self.grad_scale = 0;
    self.collectcount = 0;
    self.episodereward = 0;
    
    local currentlearningRate =
      learning_min + (learning_rate-learning_min)
      *math.max( (1-(self.episode-1)/learning_decay), 0); 
      
    for _, node in pairs(self.clusternodes) do
      node:SetLearningRate(currentlearningRate);
    end
    
    WARNING(string.format("Begin %d learning rate %.8f", episode, currentlearningRate));
    local ow = self.neuronnet:getParameters();
    ow:zero();
    self.resumer = Context:GetServiceContext();
    self.resumer:Pasue();
    
    WARNING(string.format("trainning episode %d reward %d grad scale %.8f learning rate %.6f loss %.4f", self.episode, self.episodereward, self.grad_scale, self.step_size, self.loss));
    
    self:Save();
    
    local ps,pb = self:_SerializeParameters();
    for _, node in pairs(self.clusternodes) do
      node:SynchronyNeuronnet(ps, pb);
    end
    for _, node in pairs(self.clusternodes) do
      node:Resume();
    end    
        
    self.episode = self.episode + 1;
  end
end

function qlearcentrnode:EpsiodeDone(episode, episodereward, step_size, loss, grad_scale, ps, pb, str)
  local scale = 1 / self.nodescount;
  self.collectcount = self.collectcount + 1;  
  self.episodereward = self.episodereward + episodereward;
  self.loss = self.loss + loss * scale;
  self.grad_scale = math.max(self.grad_scale, grad_scale);
  self.step_size = math.max(self.step_size, step_size);
  local ow = self.neuronnet:getParameters();
  local nw = arclight.Message2Tensor(ps, pb);
  nw:mul(scale);
  ow:add(nw);
  if nil ~= str then LOG(str); end
  if self.collectcount == self.nodescount then   
    self.resumer:Resume();
    if _ENABLE_TENSOR_BOARD then
      self.tensorboard:add_scalar_value("loss", self.loss);
      self.tensorboard:add_scalar_value("reward", self.episodereward);
    end
  end
end

function qlearcentrnode:StepLoss(loss)
  local scale = 1 / self.nodescount;
  self.stepcollectcount = self.stepcollectcount + 1;
  self.steploss = self.steploss + loss * scale;
  if self.stepcollectcount == self.nodescount then
    local temploss = self.steploss;
    LOG("part loss "..temploss);
    self.stepcollectcount = 0;
    self.steploss = 0;
    if _ENABLE_TENSOR_BOARD then
      self.tensorboard:add_scalar_value("part_loss", temploss);
    end
  end
end

function qlearcentrnode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

return qlearcentrnode;