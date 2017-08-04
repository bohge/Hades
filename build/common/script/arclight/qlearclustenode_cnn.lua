require "arclight";
require "utility"
require "torch";
require "dpnn"
require "rnn"
require "nn"


local qlearclustenode = torch.class('arclight.qlearclustenode');
qlearclustenode.centrcenode = nil;
qlearclustenode.neuronnet = nil;
qlearclustenode.criterion = nil;
qlearclustenode.loss = nil;
qlearclustenode.count = nil;

function qlearclustenode:Setup()
  torch.manualSeed(851022);
  local nThreads = torch.getnumthreads()
  LOG("set thread count "..tostring(nThreads));
  torch.setnumthreads(nThreads)
end

function qlearclustenode:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  LOG('neuron network is ' .. self.neuronnet:__tostring());
  LOG('criterion is ' .. self.criterion:__tostring());
  self.neuronnet:float();
  self.criterion:float();
  collectgarbage();
end

function qlearclustenode:SetCentrcenode(node)
  self.centrcenode = node;
end

function qlearclustenode:SetParameters(experiences_discount, action_count, state_d, state_w, state_h)
  self.experiences_discount = experiences_discount;
  self.action_count = action_count;
  self.state_d = state_d;
  self.state_w = state_w;
  self.state_h = state_h;
end

function qlearclustenode:SynchronyNeuronnet(ps, pb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
  collectgarbage();
end

function qlearclustenode:_RevertDatatable(datas)
  local partdata = {};
  for _, v in ipairs(datas) do
    local batch = {};
    batch.current_state = arclight.Message2Tensor(v.current_state[1], v.current_state[2]);
    batch.next_state = arclight.Message2Tensor(v.next_state[1], v.next_state[2]);
    batch.action = v.action;
    batch.reward = v.reward;
    batch.isterminal = v.isterminal;    
    table.insert(partdata, batch);
  end 
  return partdata;
end

function qlearclustenode:_GetExperienceBatchs(partdata)
    --计算经验目标
  local batchs = {};
  local simplecount = #partdata;
  self.neuronnet:evaluate();--得到历史状态需要使用这个模式
  
  local nextout = self.neuronnet:forward(partdata[1].current_state);
  for _,v in ipairs(partdata) do
    local updatereward = nextout:clone();--当前的next实际上是这一组数据的current
    nextout = self.neuronnet:forward(v.next_state);
    if v.isterminal then
      updatereward[{v.action}] = v.reward;
    else
      updatereward[{v.action}] = v.reward + self.experiences_discount * torch.max(nextout);
    end
    local pack =
    {
      input = v.current_state,
      target = updatereward,
    }
    table.insert(batchs, pack);
  end
  return batchs;
end

function qlearclustenode:_DoTraning(nodecountscale, batchs)
  local loss = 0;
  local batchsize = #batchs;
  local accscale = nodecountscale / batchsize;
  self.neuronnet:training();
  self.neuronnet:zeroGradParameters();
  for i, data in ipairs(batchs) do
    self.neuronnet:forward(data.input);
    loss = loss + self.criterion:forward(self.neuronnet.output, data.target) * accscale;--计算误差
    local grad = self.criterion:backward(self.neuronnet.output, data.target);--计算梯度
    self.neuronnet:backward(data.input, grad, accscale)
  end
  LOG(string.format("end trainning accscale %.3f local loss %.5f", accscale, loss));
  return loss;
end

function qlearclustenode:DistributedTraning(nodecountscale, datas)
  local partdata = self:_RevertDatatable(datas)
  local batchs = self:_GetExperienceBatchs(partdata);
  local loss = self:_DoTraning(nodecountscale, batchs);  
  local gs, gb = self:_GetGradBuffer();
  self.centrcenode:CollectResult(loss, gs, gb);
  collectgarbage();
end

function qlearclustenode:_GetGradBuffer()
  local _, g = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(g);
  return struct, buffer;
end

return qlearclustenode;