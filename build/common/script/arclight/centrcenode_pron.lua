require "arclight.centrcenode"
require "arclight";
require 'torch';
require 'nn';
require "arclight.neuronnets.residualnet_3_96_96"

local image = require "image"
local centrcenode_pron = torch.class('arclight.centrcenode_pron', 'arclight.centrcenode');

centrcenode_pron.neuronnet = nil;
centrcenode_pron.criterion = nil;
centrcenode_pron.createbuffer = nil;
centrcenode_pron.savefile = nil;

centrcenode_pron.traincount = nil;
centrcenode_pron.traindata = nil;
centrcenode_pron.varifydata = nil;

local splitcount = 10;
local imagesize = 32;
local inputChannels = 3;

function centrcenode_pron:Setup()  
  self.nodescount = 0;
  self.clusternodes = {};
  torch.manualSeed(851022);
  
  local censoredpath = IFileSystem:PathAssembly("docs:data/censored.tensor");
  local unqualifiedpath = IFileSystem:PathAssembly("docs:data/unqualified.tensor");
  local censoreddata = torch.load(censoredpath);
  local unqualifieddata = torch.load(unqualifiedpath);
  local censize = censoreddata:size(1);
  local unsize = unqualifieddata:size(1);
  local censoredlabel = torch.Tensor(censize):fill(1);
  local unqualified = torch.Tensor(unsize):fill(0);
  censoreddata = censoreddata:cat(unqualifieddata, 1);
  censoredlabel = censoredlabel:cat(unqualified, 1);
  local fullsize = censize + unsize;
  LOG("all data size is " .. fullsize);
  local shuffle = torch.randperm(fullsize)
  local mixdata = torch.Tensor(fullsize,inputChannels,imagesize,imagesize);
  local mixlable = torch.Tensor(fullsize);
  for i = 1, fullsize do
    mixdata[i]:copy(censoreddata[shuffle[i]]);
    mixlable[i] = censoredlabel[shuffle[i]];
  end    
  local testsize = fullsize * 0.05;
  testsize = math.modf(testsize / 1000) * 1000;
  local _, remain = math.modf(fullsize / 1000);
  remain = remain * 1000;
  testsize = testsize + remain;
  local trainsize = fullsize - testsize;
  LOG("train data size is " .. trainsize);
  LOG("test data size is " .. testsize);
  
  mean = mixdata[{ {}, {}, {}}]:mean()
  mixdata[{ {}, {}, {}  }]:add(-mean)
  stdv = mixdata[{ {}, {}, {}  }]:std()
  mixdata[{ {}, {}, {}  }]:div(stdv)
  LOG("mean " .. mean .. " stdv " .. stdv);
  
  local splitdatatable = mixdata:split(trainsize,1);
  local traindata = splitdatatable[1];
  local varifydata = splitdatatable[2];
  local splitlabeltable = mixlable:split(trainsize,1);
  local trainlabel = splitlabeltable[1];
  local varifylabel = splitlabeltable[2];
  
  traindata = traindata:split(splitcount,1);
  trainlabel = trainlabel:split(splitcount,1);
  
  varifydata = varifydata:split(1,1);
  varifylabel = varifylabel:split(1,1);
  
  assert(#traindata == #trainlabel);
  assert(#varifydata == #varifylabel);
  
  self.traindata = {}
  self.traindata.count = #traindata;
  self.traindata.size = function(self)
		return self.count;
	end
  for i=1,#traindata do
    local t = {
      torch.Tensor(splitcount,inputChannels,imagesize,imagesize):copy(traindata[i]),
      torch.Tensor(splitcount):copy(trainlabel[i])};
      --torch.Tensor(3,imagesize,imagesize):copy(traindata[i]):byte(),
      --trainlabel[i]:byte()};
    self.traindata[i] = t;
  end
  
  self.varifydata = {}
  self.varifydata.count = #varifydata;
  self.varifydata.size = function(self)
		return self.count;
	end
  for i=1,#varifydata do
    local t = {}
    table.insert(t, varifydata[i]);
    table.insert(t, varifylabel[i]);
    self.varifydata[i] = t;
  end
  
  torch.setdefaulttensortype('torch.DoubleTensor')  
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.savefile = IFileSystem:PathAssembly("docs:save/pron_net.tensor");

  LOG('neuron network is ' .. self.neuronnet:__tostring());
  LOG('criterion is ' .. self.criterion:__tostring());
  LOG('save file path is ' .. self.savefile);
  
  
  --[[for i=1,400 do
    f = self.traindata[i][2][2] == 1 and "1" or "0"
    test = self.traindata[i][1][2];
    test = test:mul(stdv);
    test = test:add(mean);
    p = IFileSystem:PathAssembly("docs:save/"..f.."/"..i..".png");
    print(p);
    image.save(p, test);
  end]]--

  
  collectgarbage();
end

function centrcenode_pron:_TraindataSize()
  return self.traincount;
end

function centrcenode_pron:_GetTraindata(index)
  return self.traindata[index];
end

function centrcenode_pron:Verification()
  LOG("predict test set");
  local errorcount = 0;
  for i=1, self.varifydata:size() do
    local example = self.varifydata[i]
    local input = example[1]
    local target = example[2]
    local res = self.neuronnet:forward(input);
    if res[1][1] > 0.5 then
      res = 1;
    else
      res = 0;
    end    
    if res ~= target[1] then
      errorcount = errorcount + 1;
    end
  end
  errorcount = errorcount / self.varifydata:size();
  LOG("res " .. 1 - errorcount);  
end

return centrcenode_pron;