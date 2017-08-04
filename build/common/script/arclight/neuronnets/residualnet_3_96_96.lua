local nninit = require 'nninit';
require 'torch';
require 'nn';

local function _getResidualLayer(nChannels, nOutChannels, stride)
   --[[

   Residual layers! Implements option (A) from Section 3.3. The input
   is passed through two 3x3 convolution filters. In parallel, if the
   number of input and output channels differ or if the stride is not
   1, then the input is downsampled or zero-padded to have the correct
   size and number of channels. Finally, the two versions of the input
   are added together.

               Input
                 |
         ,-------+-----.
   Downsampling      3x3 convolution+dimensionality reduction
        |               |
        v               v
   Zero-padding      3x3 convolution
        |               |
        `-----( Add )---'
                 |
              Output
   --]]   
  nOutChannels = nOutChannels or nChannels
  stride = stride or 1
   
  -- Path 1: Convolution
  -- The first layer does the downsampling and the striding
  cnn_seq = nn.Sequential();   
  cnn_seq:add( nn.SpatialConvolution(nChannels, nOutChannels,
                                          3,3, stride,stride, 1,1)
                                            :init('weight', nninit.kaiming, {gain = 'relu'})
                                            :init('bias', nninit.constant, 0));
  cnn_seq:add( nn.SpatialBatchNormalization(nOutChannels)
                                          :init('weight', nninit.normal, 1.0, 0.002)
                                          :init('bias', nninit.constant, 0) );
  cnn_seq:add( nn.ReLU(true) );
  cnn_seq:add( nn.SpatialConvolution(nOutChannels, nOutChannels,
                                    3,3, 1,1, 1,1)
                                    :init('weight', nninit.kaiming, {gain = 'relu'})
                                    :init('bias', nninit.constant, 0) );
  cnn_seq:add( nn.SpatialBatchNormalization(nOutChannels) );
  -- Should we put Batch Normalization here? I think not, because
  -- BN would force the output to have unit variance, which breaks the residual
  -- property of the network.
  -- What about ReLU here? I think maybe not for the same reason. Figure 2
  -- implies that they don't use it here

  -- Path 2: Identity / skip connection
  skip_seq = nn.Sequential();
  if stride > 1 then
    -- optional downsampling    
    skip_seq:add( nn.SpatialAveragePooling(1, 1, stride,stride) );
  end
  if nOutChannels > nChannels then
    -- optional padding
    skip_seq:add( nn.Padding(1, (nOutChannels - nChannels), 3) );
  elseif nOutChannels < nChannels then
    -- optional narrow, ugh.
    skip_seq:add( nn.Narrow(2, 1, nOutChannels) );
    -- NOTE this BREAKS with non-batch inputs!!
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
  ------> 3, 96,96
  local neuronnet = nn.Sequential();
  ------> 64, 48,48
  neuronnet:add(nn.SpatialConvolution(3, 64, 7,7, 2,2, 3,3));
  neuronnet:add(nn.ReLU(true));
  neuronnet:add(nn.SpatialMaxPooling(3,3,  2,2,  1,1));
  ------> 64, 24,24  
  neuronnet:add(_getResidualLayer(64));
  ------> 128, 12,12
  neuronnet:add(_getResidualLayer(64, 128, 2));
  ------> 256, 6,6
  neuronnet:add(_getResidualLayer(128, 256, 2));
  ------> 512, 3,3
  neuronnet:add(_getResidualLayer(256, 512, 2));
  ------> 1000, 1,1
  neuronnet:add(nn.SpatialConvolution(512, 1000, 3,3));
  neuronnet:add(nn.ReLU(true));
  ------> 1000
  neuronnet:add(nn.Reshape(1000));
  neuronnet:add(nn.Linear(1000,1));
  neuronnet:add(nn.LogSoftMax());
  criterion = nn.BCECriterion();
  return neuronnet, criterion;
end
