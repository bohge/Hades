require 'torch';
require 'nn';

function MakeNeuronCriterion()
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.Reshape(22*19));
  neuronnet:add(nn.Linear(22*19,209));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(209,209));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(209,209));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(209,4));--fc
  neuronnet:add(nn.Linear(4,4));--regression
  neuronnet:add(nn.View(4));
  criterion = nn.MSECriterion();
  return neuronnet, criterion;
end