local _ENABLE_TENSOR_BOARD = false

local crayon = require "crayon"
local optim = require "optim"
require "arclight";
require "utility"
require "torch";
require "dpnn"
require "nn"


local GAME_NAME = 'CartPole-v0';
local SAVE_FREQ = 100;
local GRAD_CLIPER = 10;

local aaaccentrnode = torch.class('arclight.aaaccentrnode');
aaaccentrnode.clusternodes = nil;
aaaccentrnode.nodescount = nil;



function aaaccentrnode:Setup()
  if _ENABLE_TENSOR_BOARD then
    self.cc = crayon.CrayonClient("10.12.192.197", 8889);
    self.cc:remove_experiment("a3c_pendulum");
    self.tensorboard = self.cc:create_experiment("a3c_pendulum");
  end
  self.nodescount = 0;  
  self.clusternodes = {};
  self.optim = optim.rmsprop;
  self.ActorConfig = 
  {
    learningRate = 1e-3,
    alpha = 0.99,
    epsilon = 1e-8,
    weightDecay = 0,
    initialMean = 0,
  }
  self.CriticConfig = 
  {
    learningRate = 1e-3,
    alpha = 0.99,
    epsilon = 1e-8,
    weightDecay = 0,
    initialMean = 0,
  }
  
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/a3c_gym_discrete.lua");
  loadstring(self.neuronmaker)();
  arclight.openaigym:Setup(GAME_NAME);
  local ss = arclight.openaigym:GetStateSpace();
  local as = arclight.openaigym:GetActionSpace();
  self.ActorNeuron, self.CriticNeuron = MakeNeuronCriterion(ss.shape[1], as.n);  
  
  self.savefile = IFileSystem:PathAssembly("docs:save/a3c_cartpole_%s.tensor");
 
  LOG('actor neuron network is ' .. self.ActorNeuron:__tostring());
  LOG('critic neuron network is ' .. self.CriticNeuron:__tostring());
  LOG('save file path is ' .. self.savefile);
  LOG("initialize experiences done");
  return true;
end

--插入计算节点
function aaaccentrnode:InsertComputernode(strgid, node)
  assert(nil == self.clusternodes[strgid]);
  self.clusternodes[strgid] = node;
  self.nodescount = self.nodescount + 1;
  node:SetEnvrionment(GAME_NAME);
  node:SetNeuronCriterion(self.neuronmaker);
  return self.nodescount;
end

function aaaccentrnode:RemoveComputernode(strgid)
  assert(nil ~= self.clusternodes[strgid]);
  self.clusternodes[strgid] = nil;
  self.nodescount = self.nodescount - 1;
  return self.nodescount;
end


function aaaccentrnode:_Save(neuron, fixname)
  local fullpath = string.format(self.savefile, fixname);
  LOG("save "..fullpath);
  neuron:clearState();
  torch.save(fullpath, neuron);
  if self.applycount then
    local backpath = fullpath .. "_episode_" .. self.applycount;
    LOG("save "..backpath);
    torch.save(backpath, neuron);
  end
end

function aaaccentrnode:Save()
  self:_Save(self.ActorNeuron, "actor");
  self:_Save(self.CriticNeuron, "cirtic");
end

function aaaccentrnode:_Load(neuron, fixname)
  local fullpath = string.format(self.savefile, fixname);
  LOG("Load "..fullpath);
  neuron = torch.load(fullpath);  
end

function aaaccentrnode:Load()
  self:_Load(self.ActorNeuron, "actor");
  self:_Load(self.CriticNeuron, "cirtic");
end

function aaaccentrnode:SetLearningRate(lr)
  LOG(string.format("set learning rate %f, was %f", lr, learning_rate));
  self.learning_rate = lr;
end

function aaaccentrnode:Trainning()
  local its = 1000000;
  self.applycount = 0;
  local aps, apb, cps, cpb = self:_SerializeParameters();
  LOG("Begin trainning");
  for _, node in pairs(self.clusternodes) do
    node:SynchronyNeuronnet(aps, apb, cps, cpb);    
    node:Trainning(its);
  end  
end

function aaaccentrnode:_ApplyGradient(neuronnet, config, sb, bb)
  local ow, ogw = neuronnet:getParameters();
  arclight.Message2Tensor(sb, bb, ogw);
  
  neuronnet:gradParamClip(GRAD_CLIPER);
  
  local w_scale = torch.norm(ow);
  local g_scale = config.learningRate*torch.norm(ogw);
  local grad_scale = g_scale/w_scale;
  LOG(string.format("w_scale %.5f, g_scale %.5f, grad_scale %.5f", w_scale, g_scale, grad_scale));
  
  local feval = function()
    local loss = 0;
    return loss, ogw;
  end
  self.optim(feval, ow, config);  
end

function aaaccentrnode:ApplyGradient(asb, abb, csb, cbb)
  self:_ApplyGradient(self.ActorNeuron, self.ActorConfig, asb, abb);
  self:_ApplyGradient(self.CriticNeuron, self.CriticConfig, csb, cbb);
  LOG("-------------------------------------------");
  self.applycount = self.applycount + 1;
  if self.applycount % ( self.nodescount * SAVE_FREQ ) == 0 then
    self:Save();
  end
  return self:_SerializeParameters();
end

function aaaccentrnode:_SerializeParameters()
  local aw = self.ActorNeuron:getParameters();
  local asb, abb = arclight.Tensor2Message(aw);
  local cw = self.CriticNeuron:getParameters();
  local csb, cbb = arclight.Tensor2Message(cw);  
  return asb, abb, csb, cbb;
end

return aaaccentrnode;