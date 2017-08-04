require 'torch';
require 'nn';

function MakeNeuronCriterion()
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.SpatialConvolution(1,6,3,3));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(2,2,2,2));
  neuronnet:add(nn.SpatialConvolution(6,12,3,3,2,2));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(2,2,2,2));
  neuronnet:add(nn.View(12));
  neuronnet:add(nn.Linear(12,4));
  criterion = nn.MSECriterion();
  return neuronnet, criterion;
end