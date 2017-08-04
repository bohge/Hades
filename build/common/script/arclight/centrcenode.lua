require "arclight";
require "utility"
require 'gnuplot'
require "torch"
require 'optim'
require 'nn'

local centrcenode = torch.class('arclight.centrcenode');
centrcenode.clusternodes = nil;
centrcenode.nodescount = nil;

centrcenode.neuronnet = nil;
centrcenode.criterion = nil;

--远程测试网络
centrcenode.loss = nil;
centrcenode.avgloss = nil;
centrcenode.lastloss = nil;
centrcenode.learningRate = nil;
centrcenode.currentlearningRate = nil;
centrcenode.episodecount = nil;--迭代次数
centrcenode.remoteSendIndex = nil;--已经发送的数据index
centrcenode.reciveCount = nil;--已经接受的数据count
centrcenode.maxiterations = nil;--最大迭代数
centrcenode.iterationscount = nil;--迭代次数
centrcenode.errorThreshold = nil;--停止训练的误差阈值
centrcenode.collectcount = nil;--接受到的回掉的数量
centrcenode.batchcount = nil;--计算集的个数
centrcenode.batchsize = nil;--梯度下降没一个集的大小
centrcenode.remainbatchsize = nil;--梯度下降有效地集大小（考虑不足一个完整batch的情况）
centrcenode.totalbatch = nil;--全部集合个数
centrcenode.shuffledIndices = nil;--随机索引

centrcenode.adamstate = nil;--adam状态


centrcenode.plot_x = nil;
centrcenode.plot_y = nil;
   
local sendbatch = 2;

function centrcenode:_SetNeuronCriterion(node)
  node:SetNeuronCriterion(self.neuronmaker);
end


function centrcenode:InsertComputernode(strgid, node)
  assert(nil == self.clusternodes[strgid]);
  self.clusternodes[strgid] = node;
  self.nodescount = self.nodescount + 1;
  self:_SetNeuronCriterion(node);
  return self.nodescount;
end

function centrcenode:RemoveComputernode(strgid)
  assert(nil ~= self.clusternodes[strgid]);
  self.clusternodes[strgid] = nil;
  self.nodescount = self.nodescount - 1;
  return self.nodescount;
end

function centrcenode:Save()
  LOG("save "..self.savefile);
  self.neuronnet:clearState();
  torch.save(self.savefile, self.neuronnet);
  if self.avgloss then
    local backpath = self.savefile .. "-loss" .. self.avgloss;
    LOG("save "..backpath);
    torch.save(backpath, self.neuronnet);
  end  
end

function centrcenode:Load()
  LOG("load "..self.savefile);
  self.neuronnet = torch.load(self.savefile);
end

function centrcenode:Training(lr, its, batch, err)
  self.neuronnet:training();
  self.learningRate = lr;
  self.loss = err*10;
  self.avgloss = err*10;
  self.lastloss = err*10;
  self.errorThreshold = err;
  self.maxiterations = its;
  self.iterationscount = 0;
  self.reciveCount = 0;
  self.episodecount = 0;
  self.batchsize = batch;
  self.totalbatch = math.ceil(self:_TraindataSize()/batch);
  self.batchcount = self.totalbatch-1;
  self.plot_x = {};
  self.plot_y = {};
  --初始化adam
  self.adamstate = {};
  self.adamstate.t = 0;
  self.adamstate.beta1 = 0.9;
  self.adamstate.beta2 = 0.995;
  self.adamstate.epsilon = 1e-7;
  LOG("Train set, max iteration "..its
    .." data size "..self:_TraindataSize()
    .." batch size "..batch
    .." total batch count "..self.totalbatch
    .." error threshold ".. err);
  self:tranning();
end

function centrcenode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

function centrcenode:_SerializeSubparameters()
  local sp = self.neuronnet:getSubparameters();
  local struct, buffer = arclight.Tensor2Message(sp);
  return struct, buffer;
end

function centrcenode:_PlotFigure()
  gnuplot.figure(1);
  gnuplot.title('CG loss minimisation over its');
  x = torch.Tensor(self.plot_x);
  y = torch.Tensor(self.plot_y);
  gnuplot.plot(x, y);
end

function centrcenode:_DoTranning()
  local ps,pb = self:_SerializeParameters();
  local ss,sb = self:_SerializeSubparameters();
  --[[LOG("begine training " .. self.iterationscount.." iterations "
    ..(self.batchcount+1).." batch");]]--
  for k,v in pairs(self.clusternodes) do
    v:SynchronyParameters(ps, pb, ss, sb);--同步全部的参数
    v:BeginTrain(self.iterationscount, 1, self.remainbatchsize);
  end
  self.neuronnet:zeroSubparameters();
  self.neuronnet:zeroGradParameters();
  collectgarbage();
end

function centrcenode:_CountRemainbatchsize()
  local datasize = self:_TraindataSize();
  local baseindex = self.batchcount*self.batchsize;
  local remain = datasize - baseindex;
  self.remainbatchsize = remain < self.batchsize and remain or self.batchsize;
end

function centrcenode:_Episode()
  table.insert(self.plot_x, self.iterationscount);
  table.insert(self.plot_y, self.avgloss);
  self:Save();
  WARNING("end " .. self.episodecount .. " episode avg loss is "..self.avgloss);
  self.lastloss = self.avgloss;
  self.loss = 0;
  self.avgloss = 0;
  self.batchcount = 0;
  self.reciveCount = 0;
  self.collectcount = 0;
  self.remoteSendIndex = 1;
  self.episodecount = self.episodecount + 1;
  local acc = math.floor(self.iterationscount / 1000) + 1;
  self.currentlearningRate = self.learningRate / acc;
  self.shuffledIndices = torch.randperm(self:_TraindataSize(), 'torch.LongTensor')
  self:_CountRemainbatchsize();
end

function centrcenode:_Batch()
  self.loss = 0;
  self.reciveCount = 0;
  self.collectcount = 0;
  self.remoteSendIndex = 1;
  self.batchcount = self.batchcount + 1;
  self:_CountRemainbatchsize();
end

function centrcenode:Stop()
  self.iterationscount = self.maxiterations;
end

function centrcenode:tranning()
  if self.iterationscount < self.maxiterations
      and self.lastloss > self.errorThreshold then
    if self.batchcount == (self.totalbatch-1) then
      self:_Episode();
    else
      self:_Batch();
    end    
    self:_DoTranning();
  else
    self:_PlotFigure();
  end  
end

--远程相关代码
function centrcenode:getTraindata()
  local res = {};
  local datasize = self:_TraindataSize();
  local baseindex = self.batchcount*self.batchsize;
  for i=1, sendbatch do
    if self.remoteSendIndex <= self.remainbatchsize then
      local index = baseindex + self.remoteSendIndex;
      index = self.shuffledIndices[index];
      local example = self:_GetTraindata(index);
      local input = example[1];
      local target = example[2];
      local tosend = {};
      local is, ib = arclight.Tensor2Message(input);
      local ts, tb = arclight.Tensor2Message(target);
      tosend.input = {is, ib};
      tosend.target = {ts, tb};
      table.insert(res, tosend);
      self.remoteSendIndex = self.remoteSendIndex + 1;
    else
      break;
    end
  end
  collectgarbage();
  return res;
end

function centrcenode:collectResult(count, loss, gs, gb, ss, sb)
  self.loss = self.loss + loss;
  self.reciveCount = self.reciveCount + count;
  local ow, ogw = self.neuronnet:getParameters();
  local nogw = arclight.Message2Tensor(gs, gb);
  ogw:add(nogw);
  local osp = self.neuronnet:getSubparameters();
  local nosp = arclight.Message2Tensor(ss, sb);
  osp:add(nosp);
  
  self.collectcount = self.collectcount + 1;
  --[[print("collectResult count " .. count .. " loss " .. loss 
    .. " sum count " .. self.reciveCount
    .. " collecter count " .. self.collectcount);]]--
  if self.reciveCount == self.remainbatchsize
  and self.collectcount == self.nodescount then

    self.adamstate.t = self.adamstate.t + 1;    
    self.adamstate.m = self.adamstate.m or torch.Tensor(ogw:size()):zero();
    self.adamstate.v = self.adamstate.v or torch.Tensor(ogw:size()):zero();
    self.adamstate.denom = self.adamstate.denom or torch.Tensor(ogw:size()):zero();
    self.adamstate.m:mul(self.adamstate.beta1):add(1-self.adamstate.beta1, ogw);
    self.adamstate.v:mul(self.adamstate.beta2):addcmul(1-self.adamstate.beta2, ogw, ogw);    
    self.adamstate.denom:copy(self.adamstate.v):sqrt():add(self.adamstate.epsilon);    
    local biasCorrection1 = 1 - self.adamstate.beta1^self.adamstate.t
    local biasCorrection2 = 1 - self.adamstate.beta2^self.adamstate.t
    local stepSize = self.currentlearningRate * math.sqrt(biasCorrection2)/biasCorrection1;
    ow:addcdiv(-stepSize, self.adamstate.m, self.adamstate.denom);

    --local stepSize = self.currentlearningRate;
    --self.neuronnet:updateParameters(stepSize);
    
    LOG("end " .. self.iterationscount
      .. " iterations "..self.batchcount..
      " batch and step size is " .. stepSize
      .. " batch loss is " .. self.loss);
    --print(tostring(ogw));
    self.avgloss = self.avgloss + self.loss / self.totalbatch;
    self.iterationscount = self.iterationscount + 1;
    self:tranning();
  end
  collectgarbage();
end

function centrcenode:LittleBigEnding()
  local m = torch.rand(5, 5);
  print(tostring(m));
  local struct, buffer = arclight.Tensor2Message(m);
  for k,v in pairs(self.clusternodes) do
    v:LittleBigEnding(struct, buffer);
  end
end

function centrcenode:TorchSerialize()
  local m = torch.rand(5, 5);
  print(tostring(m));
  buf = torch.serialize(m, "ascii");
  for k,v in pairs(self.clusternodes) do
    v:TorchDeserialize(buf);
  end
end

return centrcenode;