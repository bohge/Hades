require 'torch';
require 'nn';

local function _MakeActorNeuronCirterion(state_size, action_size)
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.Linear(state_size,200));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(200,action_size));
  neuronnet:add(nn.SoftMax());
  return neuronnet;
end

local function _MakeCriticNeuronCirterion(state_size)
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.Linear(state_size,100));
  neuronnet:add(nn.ReLU());
  neuronnet:add(nn.Linear(100,1));
  return neuronnet;
end


function MakeNeuronCriterion(state_size, action_size)
  local an = _MakeActorNeuronCirterion(state_size, action_size);
  local cn = _MakeCriticNeuronCirterion(state_size);
  return an, cn;
end