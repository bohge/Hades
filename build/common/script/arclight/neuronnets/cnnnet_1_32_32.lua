require 'torch';
require 'nn';

function MakeNeuronCriterion()
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.SpatialConvolution(1,6,5,5));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(2,2,2,2));
  neuronnet:add(nn.SpatialConvolution(6,16,5,5));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(2,2,2,2));
  neuronnet:add(nn.View(16*4*4));
  neuronnet:add(nn.Linear(16*4*4,120));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(120,84));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(84,60));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(60,30));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(30,10));
  neuronnet:add(nn.LogSoftMax());
  criterion = nn.ClassNLLCriterion();
  return neuronnet, criterion;
end