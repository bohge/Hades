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
    -- optional downsampling    
    skip_seq:add( nn.SpatialAveragePooling(1, 1, stride,stride) );
  end
  if nOutChannels > nChannels then
      skip_seq:add( nn.Padding(1, (nOutChannels - nChannels), 3) );
  elseif nOutChannels < nChannels then
      skip_seq:add( nn.Narrow(2, 1, nOutChannels) );
    else
    skip_seq:add( nn.Copy() );
  end
  
  output = nn.Sequential();
  parall = nn.ConcatTable();
  parall:add(cnn_seq)
  parall:add(skip_seq);
  output:add(parall);
  output:add(nn.CAddTable());
  output:add(nn.ReLU(true));
  return output;
end

function MakeNeuronCriterion()
  local N = 3;
  local neuronnet = nn.Sequential();
  
  -- 1*22*19 -> 6*22*19
  neuronnet:add(nn.SpatialConvolution(1, 6, 3,3, 1,1, 1,1)
                :init('weight', nninit.kaiming, {gain = 'relu'})
                :init('bias', nninit.constant, 0));
  neuronnet:add(nn.SpatialBatchNormalization(6));
  neuronnet:add(nn.ReLU(true));  
  for i=1,N do neuronnet:add(_getResidualLayer(6)); end
  
  -- 6*22*19 -> 12*11*10
  neuronnet:add(_getResidualLayer(6, 12, 2));
  for i=1,N-1 do neuronnet:add(_getResidualLayer(12)); end
  
  -- 12*11*10 -> 24*6*5
  neuronnet:add(_getResidualLayer(12, 24, 2));
  for i=1,N-1 do neuronnet:add(_getResidualLayer(24)); end
  
  -- 24*6*5 -> 48*3*3
  neuronnet:add(_getResidualLayer(24, 48, 2));
  for i=1,N-1 do neuronnet:add(_getResidualLayer(48)); end
  
  neuronnet:add(nn.SpatialAveragePooling(3, 3));
  neuronnet:add(nn.Reshape(48));  
  neuronnet:add(nn.Linear(48,12));
  neuronnet:add(nn.ReLU(true));
  neuronnet:add(nn.Linear(12,4));
  criterion = nn.MSECriterion();
  return neuronnet, criterion;
end
