local mnist = require "mnist";
local image = require "image"
require "arclight";
require "arclight.centrcenode"
require "arclight.neuronnets.residualnet_1_32_32"



local centrcenode_mnist = torch.class('arclight.centrcenode_mnist', 'arclight.centrcenode');

centrcenode_mnist.neuronnet = nil;
centrcenode_mnist.criterion = nil;

local splitcount = 10;
local imagesize = 28;
local inputChannels = 1;

function centrcenode_mnist:Setup()
  self.nodescount = 0;
  self.clusternodes = {};
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.savefile = IFileSystem:PathAssembly("docs:save/mnist_net.tensor");

  --LOG('neuron network is ' .. self.neuronnet:__tostring());
  --LOG('criterion is ' .. self.criterion:__tostring());
  --LOG('save file path is ' .. self.savefile);
  
  traindata = mnist.traindataset()
  traindata.data = traindata.data:double();
  traindata.data = traindata.data:div(255);
  traindata.label = traindata.label:add(1);
  varifydata = mnist.testdataset()
  varifydata.data = varifydata.data:double();
  varifydata.data = varifydata.data:div(255);
  varifydata.label = varifydata.label:add(1);
  
  mean = traindata.data[{ {}, {}, {}}]:mean()
  traindata.data[{ {}, {}, {}  }]:add(-mean)
  stdv = traindata.data[{ {}, {}, {}  }]:std()
  traindata.data[{ {}, {}, {}  }]:div(stdv)
  LOG("mean " .. mean .. " stdv " .. stdv);
  
  mean = varifydata.data[{ {}, {}, {}}]:mean()
  varifydata.data[{ {}, {}, {}  }]:add(-mean)
  stdv = varifydata.data[{ {}, {}, {}  }]:std()
  varifydata.data[{ {}, {}, {}  }]:div(stdv)
  LOG("mean " .. mean .. " stdv " .. stdv);
  
  trainlabel = traindata.label:view(traindata:size(), 1);
  traindata = traindata.data:view(traindata:size(), inputChannels, imagesize, imagesize);
  traindata = traindata:split(splitcount,1);
  trainlabel = trainlabel:split(splitcount,1);
  self.traindata = {}
  self.traindata.count = #traindata;
  self.traindata.size = function(self)
		return self.count;
	end
  for i=1,#traindata do
    local t = {
      torch.Tensor(splitcount,inputChannels,imagesize,imagesize):copy(traindata[i]),
      torch.Tensor(splitcount):copy(trainlabel[i])};
    self.traindata[i] = t;
  end
  
  varifylabel = varifydata.label:view(varifydata:size(), 1);
  varifydata = varifydata.data:view(varifydata:size(), inputChannels, imagesize, imagesize);
  varifydata = varifydata:split(1,1);
  varifylabel = varifylabel:split(1,1);
  self.varifydata = {}
  self.varifydata.count = #varifydata;
  self.varifydata.size = function(self)
		return self.count;
	end
  for i=1,#varifydata do
    local t = {
      torch.Tensor(1,inputChannels,imagesize,imagesize):copy(varifydata[i]),
      varifylabel[i]};
    self.varifydata[i] = t;
  end
  
  --[[
  for i=1,400 do
    f = self.traindata[i][2][2];
    test = self.traindata[i][1][2];
    p = IFileSystem:PathAssembly("docs:save/"..f.."/"..i..".png");
    print(p);
    image.save(p, test);
  end
  ]]--
  
  --输入测试数据
  --[[
  self.traindata = {}
  for i=1,10 do
    self.traindata[i] = {};
    self.traindata[i].x = torch.rand(1,28,28);
    self.traindata[i].y = math.random(11);
  end
  self.varifydata = {}
  for i=1,1 do
    self.varifydata[i] = {};
    self.varifydata[i].x = torch.rand(1,28,28);
    self.varifydata[i].y = math.random(11);
  end
  ]]--
end

function centrcenode_mnist:_GetTraindata(index)
  return self.traindata[index];
end

function centrcenode_mnist:_TraindataSize()
  return self.traindata.count;
end

function centrcenode_mnist:Verification()
  LOG("predict test set");
  local errorcount = 0;
  for i=1, self.varifydata:size() do
    local example = self.varifydata[i]
    local input = example[1]
    local target = example[2]
    local res = self.neuronnet:forward(input);
    local _, indices = torch.max(res, 2)  -- true means sort in descending order
    print(indices[1][1].." || "..target[1][1]);
    if indices[1][1] ~= target[1][1] then
      errorcount = errorcount + 1;
    end    
  end
  errorcount = errorcount / self.varifydata:size();
  LOG("res " .. 1 - errorcount);  
end

return centrcenode_mnist;