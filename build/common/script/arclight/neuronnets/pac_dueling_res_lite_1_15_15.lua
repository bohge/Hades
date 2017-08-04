local DuelAggregator = require 'arclight.neuronnets.DuelAggregator'

local nninit = require 'nninit';
require 'torch';
require 'nn';

local function _getResidualLayer(nChannels, nOutChannels, stride)
  nOutChannels = nOutChannels or nChannels
  stride = stride or 1
  cnn_seq = nn.Sequential();   
  cnn_seq:add( nn.SpatialConvolution(nChannels, nOutChannels,
                                          3,3, stride,stride, 1,1)
                                            :init('weight', nninit.kaiming, {gain = 'relu'})
                                            :init('bias', nninit.constant, 0) );
  cnn_seq:add( nn.SpatialBatchNormalization(nOutChannels)
                                          :init('weight', nninit.normal, 1.0, 0.002)
                                          :init('bias', nninit.constant, 0) );
  cnn_seq:add( nn.ReLU(true) );
  cnn_seq:add( nn.SpatialConvolution(nOutChannels, nOutChannels,
                                    3,3, 1,1, 1,1)
                                    :init('weight', nninit.kaiming, {gain = 'relu'})
                                    :init('bias', nninit.constant, 0) );
  cnn_seq:add( nn.SpatialBatchNormalization(nOutChannels) );
  skip_seq = nn.Sequential();
  if stride > 1 then
    skip_seq:add( nn.SpatialAveragePooling(1, 1, stride,stride) );
  end
  if nOutChannels > nChannels then
    skip_seq:add( nn.Padding(1, (nOutChannels - nChannels), 3) );
  elseif nOutChannels < nChannels then
    skip_seq:add( nn.Narrow(2, 1, nOutChannels) );
  else
    skip_seq:add( nn.Copy() );
  end
  -- Add them together
  output = nn.Sequential();
  parall = nn.ConcatTable();
  parall:add(cnn_seq)
  parall:add(skip_seq);
  output:add(parall);
  output:add(nn.CAddTable());
  output:add(nn.ReLU(true));
  -- ^ don't put a ReLU here! see http://gitxiv.com/comments/7rffyqcPLirEEsmpX
  return output;
end

function MakeNeuronCriterion()
  local neuronnet = nn.Sequential();
  neuronnet:add(nn.SpatialConvolution(1, 6, 3,3, 1,1, 1,1)
                :init('weight', nninit.kaiming, {gain = 'relu'})
                :init('bias', nninit.constant, 0));
  neuronnet:add(nn.SpatialBatchNormalization(6));
  neuronnet:add(nn.ReLU(true));
  neuronnet:add(_getResidualLayer(6));
  neuronnet:add(_getResidualLayer(6));
  neuronnet:add(_getResidualLayer(6, 12, 2));
  neuronnet:add(_getResidualLayer(12));
  neuronnet:add(_getResidualLayer(12));
  neuronnet:add(_getResidualLayer(12, 18, 2));
  neuronnet:add(_getResidualLayer(18));
  neuronnet:add(_getResidualLayer(18));  
  neuronnet:add(_getResidualLayer(18, 24, 2));
  neuronnet:add(_getResidualLayer(24));
  neuronnet:add(_getResidualLayer(24));
  neuronnet:add(_getResidualLayer(24, 32, 2));
  neuronnet:add(_getResidualLayer(32));
  neuronnet:add(_getResidualLayer(32));    

  neuronnet:add(nn.View(32));
  -- Value approximator V^(s)
  local valStream = nn.Sequential()
  valStream:add(nn.Linear(32, 32))
  valStream:add(nn.ReLU(true))
  valStream:add(nn.Linear(32, 1)) -- Predicts value for state

  -- Advantage approximator A^(s, a)
  local advStream = nn.Sequential()
  advStream:add(nn.Linear(32, 32))
  advStream:add(nn.ReLU(true))
  advStream:add(nn.Linear(32, 4)) -- Predicts action-conditional advantage

  -- Streams container
  local streams = nn.ConcatTable()
  streams:add(valStream)
  streams:add(advStream)

  -- Add dueling streams
  neuronnet:add(streams)
  -- Add dueling streams aggregator module
  neuronnet:add(DuelAggregator(4))
  
  criterion = nn.MSECriterion();
  return neuronnet, criterion;
end