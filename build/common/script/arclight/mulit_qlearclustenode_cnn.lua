require "arclight";
require "utility"
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
  for i=1,self.nodescount do    
    local caculater = IServicesSystem:Create("arclight.mulit_qlearcaculater_cnn");
    caculater:Setup(seed+i, myself);
    table.insert(self.caculaters, caculater);
  end
end

function qlearclustenode:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  for _, node in pairs(self.caculaters) do
    node:SetNeuronCriterion(neuronmaker);
  end  
  collectgarbage();
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

function qlearclustenode:SetTrainingTimes(t)
  for _, node in pairs(self.caculaters) do
    node:SetTrainingTimes(t);
  end
end

function qlearclustenode:SetSwitchFrequency(t)
  for _, node in pairs(self.caculaters) do
    node:SetSwitchFrequency(t);
  end
end

function qlearclustenode:SetCentrcenode(node)
  self.centrcenode = node;
end

function qlearclustenode:SynchronyNeuronnet(ps, pb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
  for _, node in pairs(self.caculaters) do
    node:SynchronyNeuronnet(ps, pb);
  end  
  if self.resumer then
    self.resumer:Resume();
  end
  collectgarbage();
end

--开始训练
function qlearclustenode:Trainning(its, basescale, switch_frequency, training_times, training_batch_size, max_experience_pool, experiences_discount, exploration_min, exploration_rate)
  self.loss = 0;
  self.collectcount = 0;
  self.epsiodedonecount = 0;
  self.episodereward = 0;
  basescale = basescale / self.nodescount;
  self.neuronnet:zeroGradParameters();
  for _, node in pairs(self.caculaters) do
    node:Trainning(its, basescale, switch_frequency, training_times, training_batch_size, max_experience_pool, experiences_discount, exploration_min, exploration_rate)
  end  
end

function qlearclustenode:CollectResult(outstring, loss, gs, gb)
  self.collectcount = self.collectcount + 1;
  self.loss = self.loss + loss;
  local ow, ogw = self.neuronnet:getParameters();
  local nogw = arclight.Message2Tensor(gs, gb);
  ogw:add(nogw);  
  if self.collectcount == self.nodescount then
    local ngs, ngb = self:_GetGradBuffer();
    self.centrcenode:CollectResult(outstring, self.loss, ngs, ngb);
    self.loss = 0;
    self.collectcount = 0;
    self.neuronnet:zeroGradParameters();
  end
end

function qlearclustenode:EpsiodeDone(episode, episodereward)
  self.epsiodedonecount = self.epsiodedonecount + 1;
  self.episodereward = self.episodereward + episodereward;
  if self.epsiodedonecount == self.nodescount then
    self.centrcenode:EpsiodeDone(episode, self.episodereward);
    self.episodereward = 0;
    self.epsiodedonecount = 0;
  end
end

function qlearclustenode:_GetGradBuffer()
  local _, g = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(g);
  return struct, buffer;
end

return qlearclustenode;