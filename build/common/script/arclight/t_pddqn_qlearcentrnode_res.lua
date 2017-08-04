local _ENABLE_TENSOR_BOARD = true

local crayon = require "crayon"
require "arclight";
require "utility"
require "torch";
require "nn"


local training_frequency = 32; --每经过多少step进行一次训练
local training_batch_count = 8; --一次随机梯度下降计算的个数
local training_batch_size = 32; --一次随机梯度下降计算的个数
local synchronize_step = training_frequency * 1280; --没过多少step同步
local switch_frequency = training_frequency * 320; --目标网络与训练网络切换频率 
local learn_start = 1000; --开始训练的经验个数
local max_experience_pool = 2^18 --经验池大小
local experiences_discount = 0.99 --经验递减
local exploration_min = 0.1       --最低探索率
local exploration_rate = 0.99  --探索率随迭代次数的衰减
local exploration_decay = 200;
local learning_rate = 0.0002        --学习速率
local learning_min = 0.000002
local learning_decay = 400

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
    self.cc:remove_experiment("dpddqn_res_6_32");
    self.tensorboard = self.cc:create_experiment("dpddqn_res_6_32");
  end
  
  
  self.episodereward = 0;
  self.nodescount = 0;
  self.clusternodes = {};
  
  self.episode = 1;
  self.insertindex = 1;
  self.poolcount = 0;
  self.partionexperiences = {};
  
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dueling_res_lite_1_15_15.lua");
  loadstring(self.neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  
  self.savefile = IFileSystem:PathAssembly("docs:save/pac_net_res.tensor");
  
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

function qlearcentrnode:SetLearningRate(lr)
  LOG(string.format("set learning rate %f, was %f", lr, learning_rate));
  learning_rate = lr;
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
  LOG("begin q-learning learning rate is "..learning_rate);
  local ps,pb = self:_SerializeParameters();
  for _, node in pairs(self.clusternodes) do
    node:SetLearningRate(learning_rate);
    node:SetExplorationRate(exploration_rate);
    node:SynchronyNeuronnet(ps, pb);
    node:Trainning(its, training_frequency, training_batch_count, training_batch_size, switch_frequency, learn_start, max_experience_pool, experiences_discount, synchronize_step);
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
      
    local explorationrate =
      exploration_min + (exploration_rate-exploration_min)
      *math.max( (1-(self.episode-1)/exploration_decay), 0);
        
    for _, node in pairs(self.clusternodes) do
      node:SetLearningRate(currentlearningRate);
      node:SetExplorationRate(explorationrate);
    end
    
    WARNING(string.format("Begin %d learning rate %.6f exploration rate %.4f", episode, currentlearningRate, explorationrate));
    local ow = self.neuronnet:getParameters();
    ow:zero();
    local osp = self.neuronnet:getSubparameters();
    osp:zero();
    self.resumer = Context:GetServiceContext();
    self.resumer:Pasue();
    
    WARNING(string.format("trainning episode %d reward %d", self.episode, self.episodereward));
    
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

function qlearcentrnode:EpsiodeDone(episode, episodereward, ps, pb, ss, sb, str)
  local scale = 1 / self.nodescount;
  self.collectcount = self.collectcount + 1;  
  self.episodereward = self.episodereward + episodereward;
  local ow = self.neuronnet:getParameters();
  local nw = arclight.Message2Tensor(ps, pb);
  nw:mul(scale);
  ow:add(nw);
  local osp = self.neuronnet:getSubparameters();
  local nosp = arclight.Message2Tensor(ss, sb);
  nosp:mul(scale);
  osp:add(nosp);
  if nil ~= str then LOG(str); end
  if self.collectcount == self.nodescount then   
    self.resumer:Resume();
    if _ENABLE_TENSOR_BOARD then
      self.tensorboard:add_scalar_value("reward", self.episodereward * 10);
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