require "arclight";
require "utility"
require 'nngraph';
require "torch"
require 'nn'

local internalclusternode = torch.class('arclight.internalnode');
internalclusternode.centrcenode = nil;
internalclusternode.neuronnet = nil;
internalclusternode.criterion = nil;
internalclusternode.loss = nil;
internalclusternode.count = nil;

function internalclusternode:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  LOG('neuron network is ' .. self.neuronnet:__tostring());
  LOG('criterion is ' .. self.criterion:__tostring());
  collectgarbage();
end

function internalclusternode:SetCentrcenode(node)
  self.centrcenode = node;
end

function internalclusternode:SynchronyParameters(ps, pb, ss, sb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
  local osp = self.neuronnet:getSubparameters();
  arclight.Message2Tensor(ss, sb, osp);
  collectgarbage();
end

--中心节点通知计算节点开始训练
function internalclusternode:BeginTrain(its, acclamda, datasize)
  self.loss = 0;
  self.count = 0;
  local lossmutper = 1 / datasize;
  local accscale = acclamda * lossmutper;
  LOG("training " .. its .. " iterations accumuli speed is "..accscale.." data size is "..datasize);
  if nil == self.centrcenode then
    ERROR("nil centrcen node");
  else
    self.neuronnet:zeroGradParameters();
    repeat
      local trainbuffer = self.centrcenode:getTraindata();
      if next(trainbuffer) ~= nil then
        for k, v in ipairs(trainbuffer) do
          local input = arclight.Message2Tensor(v.input[1], v.input[2]):double();
          local target = arclight.Message2Tensor(v.target[1], v.target[2]):double();
          self.neuronnet:forward(input);
          self.loss = self.loss + self.criterion:forward(self.neuronnet.output, target) * lossmutper;
          LOG(tostring(target) .. "\r\n" .. tostring(self.neuronnet.output));
          local grad = self.criterion:backward(self.neuronnet.output, target);
          --LOG(tostring(grad));
          self.neuronnet:backward(input, grad, accscale);
          self.count = self.count + 1;
        end
        collectgarbage();
      end
    until(next(trainbuffer) == nil)
    LOG("distributed trainning end caculated count ".. self.count .. " loss " .. self.loss);
    local gs, gb = self:_GetGradBuffer();
    local ss, sb = self:_GetSubBuffer(self.count * accscale);
    self.centrcenode:collectResult(self.count, self.loss, gs, gb, ss, sb);
    collectgarbage();
  end
end

function internalclusternode:_GetGradBuffer()
  local _, g = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(g);
  return struct, buffer;
end

function internalclusternode:_GetSubBuffer(subscale)
  local sp = self.neuronnet:getSubparameters();
  sp:mul(subscale);
  local struct, buffer = arclight.Tensor2Message(sp);
  return struct, buffer;
end

return internalclusternode;