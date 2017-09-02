require 'torch';
require 'nn';

local function _MakeActorNeuronCirterion(state_size, action_size)
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.Linear(state_size,200));
  neuronnet:add(nn.ReLU6());
  
  local mu_seq = nn.Sequential();
  mu_seq:add(nn.Linear(200,action_size));
  mu_seq:add(nn.Tanh());
  
  local sigma_seq = nn.Sequential();
  sigma_seq:add(nn.Linear(200,action_size));
  sigma_seq:add(nn.SoftPlus());
  
  local concat = nn.ConcatTable();
  concat:add(mu_seq);
  concat:add(sigma_seq);
  
  neuronnet:add(concat);
  return neuronnet;
end

local function _MakeCriticNeuronCirterion(state_size)
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.Linear(state_size,100));
  neuronnet:add(nn.ReLU6());
  neuronnet:add(nn.Linear(100,1));
  return neuronnet;
end


function MakeNeuronCriterion(state_size, action_size)
  local an = _MakeActorNeuronCirterion(state_size, action_size);
  local cn = _MakeCriticNeuronCirterion(state_size);
  return an, cn;
end