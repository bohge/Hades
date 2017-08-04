require 'torch';
require 'nn';

function MakeNeuronCriterion()
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.SpatialConvolution(3,16,5,5));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(2,2,2,2));
  neuronnet:add(nn.SpatialConvolution(16,32,5,5));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(2,2,2,2));
  neuronnet:add(nn.View(32*5*5));
  neuronnet:add(nn.Linear(32*5*5,120));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(120,84));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(84,60));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(60,30));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(30,1));
  neuronnet:add(nn.Sigmoid());
  criterion = nn.BCECriterion();
  return neuronnet, criterion;
end