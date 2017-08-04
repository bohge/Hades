require "arclight";
require "utility"
require "pacman"
require "torch";
require "dpnn"
require "rnn"
require "nn"

local learning_rate = 0.001        --学习速率
local training_times = 10;
local training_package_size = 16  --每一个数据集的第一维大小
local training_batch_size = 100   --一次随机梯度下降计算的个数
local max_experience_pool = training_package_size * training_batch_size * 20 --经验池大小
local experiences_discount = 0.98 --经验递减
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
  
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dqn_resnet_1_22_19.lua");
  loadstring(self.neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.neuronnet:float();
  self.criterion:float();
  
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

--开始训练
function qlearcentrnode:Trainning(lr, its)
  self.episode = 1;
  local scale = 1 / self.nodescount;
  for _, node in pairs(self.clusternodes) do
    node:Trainning(its, scale, training_times, training_package_size, training_batch_size, max_experience_pool, experiences_discount, exploration_min, exploration_rate);
  end
  self.episode = 1;
  self.epsiodedonecount = 0;
  while true do
    self.loss = 0;
    self.collectcount = 0;
    self.neuronnet:zeroGradParameters();
    self.resumer = Context:GetServiceContext();
    stepsize, grad_scale = self.resumer:Pasue();
    LOG(string.format("trainning episode %d grad scale %.5f learning rate %.5f loss %.4f", self.episode, grad_scale, stepsize, self.loss));
    local ps,pb = self:_SerializeParameters();
    for _, node in pairs(self.clusternodes) do
      node:SynchronyNeuronnet(ps, pb);
    end
  end
end

function qlearcentrnode:EpsiodeDone(episode)
  self.episode = self.episode + 1;
  self.epsiodedonecount = self.epsiodedonecount + 1;
  if self.epsiodedonecount == self.nodescount then
    self.epsiodedonecount = 0;
    self.episode = self.episode + 1;
    self:Save();
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

function qlearcentrnode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

return qlearcentrnode;