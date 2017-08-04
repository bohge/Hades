require "arclight";
require "utility"
require "pacman"
require "torch";
require "dpnn"
require "rnn"
require "nn"


local qlearclustenode = torch.class('arclight.qlearclustenode');
qlearclustenode.clusternodes = nil;
qlearclustenode.nodescount = nil;
qlearclustenode.collectcount = nil;
qlearclustenode.resumer = nil;


function qlearclustenode:Setup(seed, myself)  
  self.nodescount = 8;
  self.caculaters = {};
  math.randomseed(seed);
  torch.manualSeed(seed);  

  --初始化adam  
  self.adamax = {}
  self.adamax.beta1 = 0.9
  self.adamax.beta2 = 0.999
  self.adamax.epsilon = 1e-38
  
  for i=1,self.nodescount do    
    local caculater = IServicesSystem:Create("arclight.grad_qlearcaculatenode_cnn");
    caculater:Setup(seed+i, myself);
    table.insert(self.caculaters, caculater);
  end
end

function qlearclustenode:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  LOG('neuron network is ' .. self.neuronnet:__tostring());
  LOG('criterion is ' .. self.criterion:__tostring());
  for _, node in pairs(self.caculaters) do
    node:SetNeuronCriterion(neuronmaker);
  end
end

function qlearclustenode:SetMeanstd(meanstd)
  for _, node in pairs(self.caculaters) do
    node:SetMeanstd(meanstd);
  end  
end

function qlearclustenode:SetEpisode(e)
  for _, node in pairs(self.caculaters) do
    node:SetEpisode(e);
  end
end

function qlearclustenode:SetLearningRate(lr)
  learning_rate = lr;
end

function qlearclustenode:SetTrainingTimes(t)
  for _, node in pairs(self.caculaters) do
    node:SetTrainingTimes(t);
  end
end

function qlearclustenode:SynchronyNeuronnet(ps, pb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
  self:_SynchronyCaculaterNeuronnet(ps,pb);
end

function qlearclustenode:_SynchronyCaculaterNeuronnet(ps, pb)
  for _, node in pairs(self.caculaters) do
    node:SynchronyNeuronnet(ps, pb);
  end
end

function qlearclustenode:Resume()
  for _, node in pairs(self.caculaters) do
    node:Resume();
  end
end

function qlearclustenode:SetCentrcenode(node)
  self.centrcenode = node;
end

--开始训练
function qlearclustenode:Trainning(its, sf, tt, tbs, mep, ed, em, er)
  self.loss = 0;
  self.steploss = 0;
  self.step_size = 0;
  self.grad_scale = 0;
  self.donecount = 0;
  self.collectcount = 0;
  self.episodereward = 0;
  self.epsiodedonecount = 0;
  self.stepcollectcount = 0;  
  local scale = 1 / self.nodescount;
  self.neuronnet:zeroGradParameters();
  for _, node in pairs(self.caculaters) do
    node:Trainning(its, scale, sf, tt, tbs, mep, ed, em, er)
  end  
end

function qlearclustenode:StepLoss(loss)
  local scale = 1 / self.nodescount;
  self.stepcollectcount = self.stepcollectcount + 1;
  self.steploss = self.steploss + loss * scale;
  if self.stepcollectcount == self.nodescount then
    self.centrcenode:StepLoss(self.steploss);
    self.stepcollectcount = 0;
    self.steploss = 0;
  end
end

function qlearclustenode:CollectResult(gs, gb)
  self.collectcount = self.collectcount + 1;
  local ow, ogw = self.neuronnet:getParameters();
  local nogw = arclight.Message2Tensor(gs, gb);
  ogw:add(nogw);
  if self.collectcount == self.nodescount then
    self.collectcount = 0;
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
    local stepSize = learning_rate/biasCorrection1;
    ogw:copy(self.adamax.m):cdiv(self.adamax.u);
    
    local w_scale = torch.norm(ow);
    local g_scale = stepSize*torch.norm(ogw);
    local grad_scale = g_scale/w_scale;
    
    ow:addcdiv(-stepSize, self.adamax.m, self.adamax.u);
    
    self.grad_scale = math.max(self.grad_scale, grad_scale);
    self.step_size = math.max(self.step_size, stepSize);
    
    self.neuronnet:zeroGradParameters();
    local ps, pb = self:_SerializeParameters();
    self:_SynchronyCaculaterNeuronnet(ps, pb);
    self:Resume();
  end  
end

function qlearclustenode:EpsiodeDone(episode, episodereward, loss, str)
  local scale = 1 / self.nodescount;
  self.donecount = self.donecount + 1;  
  self.episodereward = self.episodereward + episodereward;
  self.loss = self.loss + loss * scale;
  if self.donecount == self.nodescount then    
    local mps,mpb = self:_SerializeParameters();
    self.centrcenode:EpsiodeDone(episode, self.episodereward, self.step_size, self.loss, self.grad_scale, mps, mpb, str);
    self.loss = 0;
    self.step_size = 0;
    self.grad_scale = 0;
    self.donecount = 0;
    self.episodereward = 0;
  end
end

function qlearclustenode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

return qlearclustenode;