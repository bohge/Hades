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

function qlearclustenode:SetParameters(experiences_discount, training_package_size, action_count, state_d, state_w, state_h)
  self.experiences_discount = experiences_discount;
  self.training_package_size = training_package_size;
  self.action_count = action_count;
  self.state_d = state_d;
  self.state_w = state_w;
  self.state_h = state_h;
end

function qlearclustenode:SynchronyNeuronnet(ps, pb, ss, sb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
  local osp = self.neuronnet:getSubparameters();
  arclight.Message2Tensor(ss, sb, osp);
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
    table.insert(partdata, batch);
  end 
  return partdata;
end

function qlearclustenode:_GetExperienceBatchs(partdata)
    --计算经验目标
  local batchs = {};
  local simplecount = #partdata;
  self.neuronnet:evaluate();--得到历史状态需要使用这个模式
  assert( simplecount % self.training_package_size == 0 );
  for i=1, simplecount, self.training_package_size do
    local batchreward = torch.FloatTensor(self.training_package_size, self.action_count):zero();
    local batchcurrent = torch.FloatTensor(self.training_package_size, self.state_d, self.state_w, self.state_h):zero();
    local batchcaculate = torch.FloatTensor(self.training_package_size+1, self.state_d, self.state_w, self.state_h):zero();--+1为了一次计算
    local batchaction = torch.FloatTensor(self.training_package_size, self.action_count):zero();
    local batchinvaction = torch.FloatTensor(self.training_package_size, self.action_count):fill(1);
    for j=1, self.training_package_size do--合批
      local index = ( i - 1 ) + j;
      local exps = partdata[index];
      batchcurrent[{ j, {}, {}, {} }]:copy(exps.current_state);
      batchcaculate[{ j, {}, {}, {} }]:copy(exps.current_state);
      if self.training_package_size == j then
        batchcaculate[{ j+1, {}, {}, {} }]:copy(exps.next_state);
      end      
      batchreward[{j, exps.action}] = exps.reward;
      batchaction[{j, exps.action}] = 1;
      batchinvaction[{j, exps.action}] = 0;
    end
    --Q(S,A) ← (1-α)*Q(S,A) + α*[R + γ*maxQ(S',a)]
    local rewardcaculatebatch = self.neuronnet:forward(batchcaculate);
    local rewardcurrent = rewardcaculatebatch[{{1, self.training_package_size},{}}];
    local rewardnext = rewardcaculatebatch[{{2, self.training_package_size+1},{}}];
    local maxinrow = torch.max(rewardnext,2);
    local expandmaxrow = torch.expand(maxinrow, maxinrow:size(1), batchaction:size(2));
    local addhistry = torch.cmul(batchaction, expandmaxrow);
    local updatereward = 
      torch.cmul(batchinvaction, rewardcurrent)
      + torch.cmul(batchaction, (batchreward+self.experiences_discount*addhistry));
    local pack =
    {
      input = batchcurrent,
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
    LOG(tostring(data.target));
    loss = loss + self.criterion:forward(self.neuronnet.output, data.target) * accscale;--计算误差
    local grad = self.criterion:backward(self.neuronnet.output, data.target);--计算梯度
    self.neuronnet:backward(data.input, grad, accscale)
  end
  LOG(string.format("end trainning local loss %.5f", loss));
  return loss;
end

function qlearclustenode:DistributedTraning(nodecountscale, datas)
  local partdata = self:_RevertDatatable(datas)
  local batchs = self:_GetExperienceBatchs(partdata);
  local loss = self:_DoTraning(nodecountscale, batchs);
  
  local gs, gb = self:_GetGradBuffer();
  local ss, sb = self:_GetSubBuffer(nodecountscale);
  self.centrcenode:CollectResult(loss, gs, gb, ss, sb);
  collectgarbage();
end

function qlearclustenode:_GetGradBuffer()
  local _, g = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(g);
  return struct, buffer;
end

function qlearclustenode:_GetSubBuffer(subscale)
  local sp = self.neuronnet:getSubparameters();
  sp:mul(subscale);
  local struct, buffer = arclight.Tensor2Message(sp);
  return struct, buffer;
end

return qlearclustenode;