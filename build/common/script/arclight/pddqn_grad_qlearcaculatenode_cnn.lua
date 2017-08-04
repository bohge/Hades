require "pacmancenter"
require "arclight";
require "utility"
require "torch";
require "nn"


local qlearcaculater = torch.class('arclight.qlearcaculater');
qlearcaculater.clusteragent = nil;



function qlearcaculater:Setup(clusteragent)
  self.clusteragent = clusteragent;
  local nThreads = torch.getnumthreads();
  torch.setnumthreads(nThreads);
  self.experiences = {};
end

function qlearcaculater:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.target_q = self.neuronnet:clone();
  collectgarbage();
end

function qlearcaculater:SetDiscount(ed)
  experiences_discount = ed;
end

function qlearcaculater:SynchronyNeuronnet(ps, pb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
end

function qlearcaculater:SwitchTargetQ()
  self.target_q = self.neuronnet:clone();
end

function qlearcaculater:CollectGarbage()
  collectgarbage();
end

function qlearcaculater:CaculateExperiences(index, css, csb, nss, nsb, action, reward, isterminal)
  self.target_q:evaluate();
  self.neuronnet:evaluate();
  
  local current_state = arclight.Message2Tensor(css, csb);
  local next_state = arclight.Message2Tensor(nss, nsb);
  
  local current_out = self.neuronnet:forward(current_state):clone();
  local updatereward = current_out:clone();
  if isterminal then
    updatereward[{action}] = reward;
  else
    local next_out = self.target_q:forward(next_state):clone();
    local next_eval = self.neuronnet:forward(next_state);--double dqn
    local _, next_action = torch.max(next_eval, 1);
    updatereward[{action}] = reward + experiences_discount * next_out[next_action[1]];
  end
  batch = { input = current_state, target = updatereward }
  table.insert( self.experiences, batch );
  local priority = math.abs(current_out[{action}] - updatereward[{action}]);
  self.clusteragent:CollectExperiences(index, priority);
end

function qlearcaculater:CaculateGradient(accscale)
  local loss = 0;
  self.neuronnet:training();
  self.neuronnet:zeroGradParameters();
  for i, data in ipairs(self.experiences) do
    self.neuronnet:forward(data.input);
    loss = loss + self.criterion:forward(self.neuronnet.output, data.target) * accscale;--计算误差
    local grad = self.criterion:backward(self.neuronnet.output, data.target);--计算梯度
    self.neuronnet:backward(data.input, grad, accscale)
  end
  self.experiences = {};
  local ss, sb = self:_GetGradBuffer();
  self.clusteragent:CollectGradient(loss, ss, sb);
end

function qlearcaculater:_GetGradBuffer()
  local _, g = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(g);
  return struct, buffer;
end

return qlearcaculater;