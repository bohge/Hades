local DuelAggregator = require 'arclight.neuronnets.DuelAggregator'
require 'torch';
require 'nn';


function MakeNeuronCriterion()
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.SpatialConvolution(1,3,3,3,2,2,1,1));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialConvolution(3,6,3,3,2,2,1,1));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialConvolution(6,12,3,3,2,2,1,1));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialConvolution(12,24,3,3,2,2,1,1));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.View(24));  
  -- Value approximator V^(s)
  local valStream = nn.Sequential()
  valStream:add(nn.Linear(24, 24))
  valStream:add(nn.ReLU(true))
  valStream:add(nn.Linear(24, 1)) -- Predicts value for state

  -- Advantage approximator A^(s, a)
  local advStream = nn.Sequential()
  advStream:add(nn.Linear(24, 24))
  advStream:add(nn.ReLU(true))
  advStream:add(nn.Linear(24, 4)) -- Predicts action-conditional advantage

  -- Streams container
  local streams = nn.ConcatTable()
  streams:add(valStream)
  streams:add(advStream)

  -- Add dueling streams
  neuronnet:add(streams)
  -- Add dueling streams aggregator module
  neuronnet:add(DuelAggregator(4))
  neuronnet:add(nn.View(4))
  
  criterion = nn.MSECriterion();
  return neuronnet, criterion;
end