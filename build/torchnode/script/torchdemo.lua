local nninit = require 'nninit';
local mnist = require "mnist";

require "torch";
require 'optim'
require 'gnuplot'




torchdemo = {}
local demosuite = torch.TestSuite();


function demosuite.SimpleDemo()
  
  torch.manualSeed(1234);
  -- choose a dimension
  N = 5
  
  print('');
  -- create a random NxN matrix
  A = torch.rand(N, N)
  print(A);
  
  -- make it symmetric positive
  A = A*A:t()
  print(A);

  -- make it definite
  c = A:add(0.001, torch.eye(N))
  print(A);
  print(c);
  
  -- add a linear term
  b = torch.rand(N)
  print(b);
  
-- create the quadratic form
  function J(x)
     return 0.5*x:dot(A*x)-b:dot(x)
  end
  print(J(torch.rand(N)))
  
  xs = torch.inverse(A)*b
  print(string.format('J(x^*) = %g', J(xs)))

  function dJ(x)
    return A*x-b
  end
  x = torch.rand(N)
  
  lr = 0.01
  --[[
  for i=1,20000 do
    x = x - dJ(x)*lr
    -- we print the value of the objective function at each iteration
    print(string.format('at iter %d J(x) = %f', i, J(x)))
  end
  ]]--
  

  
  do
    local evaluations = {}
    local time = {}
    local timer = torch.Timer()
    local neval = 0
    function JdJ(x)
      local Jx = J(x)
      neval = neval + 1
      print(string.format('after %d evaluations J(x) = %f', neval, Jx))
      table.insert(evaluations, Jx)
      table.insert(time, timer:time().real)
      return Jx, dJ(x)
    end
    state = 
    {
      verbose = false,
      maxIter = 100
    }
    x0 = torch.rand(N)
    cgx = x0:clone() -- make a copy of x0
    timer:reset()
    optim.cg(JdJ, cgx, state)
    cgtime = torch.Tensor(time)
    cgevaluations = torch.Tensor(evaluations)
    
    evaluations = {}
    time = {}
    neval = 0
    state = {
      lr = 0.1
    }

    -- we start from the same starting point than for CG
    x = x0:clone()

    -- reset the timer!
    timer:reset()

    -- note that SGD optimizer requires us to do the loop
    for i=1,1000 do
      optim.sgd(JdJ, x, state)
      table.insert(evaluations, Jx)
    end

    sgdtime = torch.Tensor(time)
    sgdevaluations = torch.Tensor(evaluations)

    
    gnuplot.figure(1)
    gnuplot.title('CG loss minimisation over time')
    gnuplot.plot(cgtime, cgevaluations)

    gnuplot.figure(2)
    gnuplot.title('SGD loss minimisation over time')
    gnuplot.plot(sgdtime, sgdevaluations)
    
    --[[
    gnuplot.pngfigure('plot.png')
    gnuplot.plot(
       {'CG',  cgtime,  cgevaluations,  '-'},
       {'SGD', sgdtime, sgdevaluations, '-'})
    gnuplot.xlabel('time (s)')
    gnuplot.ylabel('J(x)')
    gnuplot.plotflush()
    ]]--
  end
end

function demosuite.Mnist()
  
  net = nn.Sequential();
  net:add(nn.SpatialConvolution(1,6,5,5));
  net:add(nn.ReLU());
  net:add(nn.SpatialMaxPooling(2,2,2,2));
  net:add(nn.SpatialConvolution(6,16,5,5));
  net:add(nn.ReLU());
  net:add(nn.SpatialMaxPooling(2,2,2,2));
  net:add(nn.View(16*4*4));
  net:add(nn.Linear(16*4*4,120));
  net:add(nn.ReLU());
  net:add(nn.Linear(120,84));
  net:add(nn.ReLU());
  net:add(nn.Linear(84,60));
  net:add(nn.ReLU());
  net:add(nn.Linear(60,30));
  net:add(nn.ReLU());
  net:add(nn.Linear(30,10));
  net:add(nn.LogSoftMax());
  print('Lenet5\n' .. net:__tostring());
  
  local p = net:getParameters();
  
  criterion = nn.ClassNLLCriterion()
  trainer = nn.StochasticGradient(net, criterion)
  trainer.learningRate = 0.001
  trainer.maxIteration = 1
  
  local netbuffer = torch.serialize(net);
  local newnet = torch.deserialize(netbuffer);
  
  local cltbuffer = torch.serialize(criterion);
  local newclt = torch.deserialize(cltbuffer);
  --load mnist 
  local trainset = mnist.traindataset()
  local testset = mnist.testdataset()
  
  --normalisation
  mean = {}
  stdv  = {}
  mean = trainset.data[{ {}, {}, {}}]:mean()
  trainset.data[{ {}, {}, {}  }]:add(-mean)
  stdv = trainset.data[{ {}, {}, {}  }]:std()
  trainset.data[{ {}, {}, {}  }]:div(stdv)
  print("mean " .. mean .. " stdv " .. stdv);
  
  mean = testset.data[{ {}, {}, {}}]:mean()
  testset.data[{ {}, {}, {}  }]:add(-mean)
  stdv = testset.data[{ {}, {}, {}  }]:std()
  testset.data[{ {}, {}, {}  }]:div(stdv)
  print("mean " .. mean .. " stdv " .. stdv);
  
  --[[local trainset = {};
  local testset = {};
  for i=1,10 do
    trainset[i] = {};
    trainset[i][1] = torch.rand(1,28,28);
    trainset[i][2] = math.random(11);
  end
  trainset.size = function(self)
		return 10;
	end
  for i=1,1 do
    testset[i] = {};
    testset[i][1] = torch.rand(1,28,28);
    testset[i][2] = math.random(11);
  end
  testset.size = function(self)
		return 1;
	end
  ]]--
  
  --[[local r = torch.rand(1,28,28);
  local rs = r:storage();
  local rb = torch.serialize(r);
  local rss = rs:size();
  print(r);
  print("buffer size " .. #rb);
  --print("buffer size " .. #rss);
  
  local exp = trainset[100];
  local input = exp[1];
  r:copy(input);
  local rb = torch.serialize(r);
  print("buffer size " .. #rb);
  local s = input:storage();
  --local buffer = torch.serialize(input);
  local ss = s:size();
  local i = s:storageOffset();
  --local ss = s:string();
  print(input);
  print("buffer size " .. #buffer);
  --print("buffer size " .. #ss);]]--
  
  local timer = torch.Timer();
  print("begin training time is " .. timer:time().real .. " seconds");
  --trainer:train(trainset);
  local loss = 0;
  for i=1, trainset:size() do
    local example = trainset[i]
    local input = example[1]
    local target = example[2]
    local output = net:forward(input);
    loss = loss + criterion:forward(net.output, target);--计算误差
    local grad = criterion:updateGradInput(net.output, target);--计算梯度
    net:updateGradInput(input, grad);--计算新的梯度
    net:accGradParameters(input, grad)
  end
  net:updateParameters(0.01);
  net:zeroGradParameters();
  local avgloss = self.loss / self.traindata:size();
  
  
  print("begin done time is " .. timer:time().real .. " seconds");
  
  local o = net:getOutputs();
  local b = torch.serialize(o);
  o:fill(1);
  local o2 = net:getOutputs();
  local o3 = net.output;
  print(o3);
  print(#b);
  
  print("predict test set ");
  local errorcount = 0;
  for i=1, testset:size() do
    local example = testset[i]
    local input = example[1]
    local target = example[2]
    local res = net:forward(input);
    local _, indices = torch.max(res, 1)  -- true means sort in descending order
    if indices[1] ~= target then
      errorcount = errorcount + 1;
    end    
  end
  errorcount = errorcount / testset:size();
  print("res " .. 1 - errorcount);
end


function demosuite.linear()
  local dataset = {};
  for i=1,100 do
    local data = {};
    data[1] = torch.rand(1);
    data[2] = data[1] * 3 + 5;
    dataset[i] = data;
  end
  net = nn.Linear(1,1);
  ctn = nn.MSECriterion();
  
  for i=1,1000 do
    local loss = 0;
    net:zeroGradParameters();
    for i=1,100 do
      local exp = dataset[i];
      local x = exp[1];
      local y = exp[2];
      
      local pit = net:forward(x);
      loss = loss + ctn:forward(pit, y)/100;      
      local grad = ctn:backward(pit, y);
      net:backward(x, grad, 0.01);
    end
    net:updateParameters(0.1);    
    print("loss is " .. loss);
  end
  local p = net:getParameters();
  print(p);
end


function torchdemo.test()
  local t1 = torch.Tensor(3,3):fill(1);
  local t2 = torch.range(1,3*3):reshape(3,3);
  print(t1);
  print(t2);
  t1:add(t2);
  print(t1);
  
  local neuronnet = nn.Sequential();
  neuronnet:add( nn.SpatialConvolution(1, 16,
                                          3,3, 3,3, 1,1)
                                            :init('weight', nninit.kaiming, {gain = 'relu'})
                                            :init('bias', nninit.constant, 0) );
  neuronnet:add( nn.SpatialBatchNormalization(16)
                                          :init('weight', nninit.normal, 1.0, 0.002)
                                          :init('bias', nninit.constant, 0) );
  neuronnet:add( nn.ReLU(true) );
  neuronnet:add(nn.SpatialMaxPooling(4,4,4,4));
  neuronnet:add(nn.Reshape(16));
  neuronnet:add(nn.Linear(16,1));
  neuronnet:add(nn.Sigmoid());
  local criterion = nn.BCECriterion();
  local input = torch.Tensor(10,1,16,16);
  local target = torch.Tensor(10):fill(1);
  local output = neuronnet:forward(input);
  local loss = criterion:forward(neuronnet.output, target);
  local grad = criterion:backward(neuronnet.output, target);
  neuronnet:backward(input, grad, 1);
  neuronnet:updateParameters(0.1);
  
  local sp = neuronnet:getSubparameters();
  neuronnet:zeroSubparameters();
  
  local path = "d:\\a.model";
  neuronnet:clearState();
  torch.save(path, neuronnet);
  local newnet = torch.load(path);
  newnet:evaluate();
  output = newnet:forward(input);
end

function torchdemo.run()
  local nThreads = torch.getnumthreads()
  torch.setnumthreads(nThreads)
  mytester = torch.Tester();
  mytester:add(demosuite);
  --demosuite.Mnist();
  demosuite.SimpleDemo();
  --demosuite.linear();
  --mytester:run();
  return mytester;
end