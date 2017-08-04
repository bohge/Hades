require "arclight.centrcenode"
require "arclight";
require 'torch';
require 'nn';

local image = require "image"
local centrcenode_pron = torch.class('arclight.centrcenode_pron', 'arclight.centrcenode');

centrcenode_pron.neuronmaker = nil;
centrcenode_pron.neuronnet = nil;
centrcenode_pron.criterion = nil;
centrcenode_pron.createbuffer = nil;
centrcenode_pron.savefile = nil;

centrcenode_pron.traindata = nil;
centrcenode_pron.varifydata = nil;
centrcenode_pron.meanstd = nil;
centrcenode_pron.falsepositive = nil;
centrcenode_pron.falsenegative = nil;


function centrcenode_pron:Setup()  
  self.nodescount = 0;
  self.clusternodes = {};
  torch.manualSeed(851022);
  
  --只加载路径
  self.varifydata = IFileSystem:PathAssembly("docs:data/verifyset/data.info");
  self.traindata = IFileSystem:PathAssembly("docs:data/trainset/data.info");
  self.meanstd = IFileSystem:PathAssembly("docs:data/preprocess.table");
  self.falsepositive = IFileSystem:PathAssembly("docs:error/falsepositive/");
  self.falsenegative = IFileSystem:PathAssembly("docs:error/falsenegative/");
  self.varifydata = torch.load(self.varifydata);
  self.traindata = torch.load(self.traindata);
  self.meanstd = torch.load(self.meanstd);
  
  self.neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/residualnet_3_96_96_grap.lua");
  loadstring(self.neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.savefile = IFileSystem:PathAssembly("docs:save/pron_net.tensor");

  LOG('neuron network is ' .. self.neuronnet:__tostring());
  LOG('criterion is ' .. self.criterion:__tostring());
  LOG('save file path is ' .. self.savefile);
  LOG("data size(in batch size) is "..self:_TraindataSize());
  collectgarbage();
end

function centrcenode_pron:_TraindataSize()
  return self.traindata.size;
end

function centrcenode_pron:_GetTraindata(index)
  local toload = self.traindata.list[index];
  toload = "docs:data/trainset/" .. toload;
  toload = IFileSystem:PathAssembly(toload);
  return torch.load(toload);
end

function centrcenode_pron:_saveerror(channel, size, index, tensor, target)
  local mean = self.meanstd[1];
  local std = self.meanstd[2];
  local imagetensor = torch.Tensor(channel,size,size);
  imagetensor:copy(tensor);
  for i=1,channel do
    imagetensor[{ i,{},{} }]:mul(std[i])
    imagetensor[{ i,{},{} }]:add(mean[i])
  end
  local path = target == 1 and self.falsenegative or self.falsepositive;
  path = path..index..".jpg";
  image.save(path, imagetensor);
  LOG("save to "..path);
end

function centrcenode_pron:Verification()
  self.neuronnet:clearState();
  self.neuronnet:evaluate();
  
  LOG("predict test set");
  local totalcount = 0;
  local errorcount = 0;
  for _, p in ipairs(self.varifydata.list) do
    toload = "docs:data/verifyset/" .. p;
    toload = IFileSystem:PathAssembly(toload);
    LOG(toload);
    local varifydata = torch.load(toload);
    local varifylabel = varifydata[2]:double();
    varifydata = varifydata[1]:double();
    local restensor = self.neuronnet:forward(varifydata);
    totalcount = totalcount + restensor:size(1);
    for j=1, restensor:size(1) do
      local res = restensor[j][1];
      res = res < 0.5 and 0 or 1;
      if res ~= varifylabel[j] then        
        errorcount = errorcount + 1;
        LOG("traget is " ..varifylabel[j].. " but predicted "..restensor[j][1]);
        self:_saveerror(3, 96, errorcount, varifydata[j], varifylabel[j]);
      end 
    end
    LOG("error count="..errorcount.." total="..totalcount);
  end
  errorcount = errorcount / totalcount;
  LOG("res " .. 1 - errorcount);
end

return centrcenode_pron;