require 'torch';
require 'nn';

function MakeNeuronCriterion()
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.SpatialConvolution(3, 64, 7,7, 2,2, 3,3));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(3,3,  2,2,  1,1));
  neuronnet:add(nn.SpatialConvolution(64, 128, 7,7, 2,2, 3,3));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(3,3,  2,2,  1,1));
  neuronnet:add(nn.SpatialConvolution(128, 256, 7,7, 2,2, 3,3));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(3,3,  2,2,  1,1));
  neuronnet:add(nn.SpatialConvolution(256, 512, 7,7, 2,2, 3,3));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.SpatialMaxPooling(3,3,  2,2,  1,1));  
  neuronnet:add(nn.Reshape(512));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(512,128));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(128,32));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(32,1));
  neuronnet:add(nn.sigmoid());
  criterion = nn.BCECriterion();
  return neuronnet, criterion;
end