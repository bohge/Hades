local nninit = require 'nninit';
require 'nngraph';
require 'torch';
require 'nn';

local function _getResidualLayer(input, nChannels, nOutChannels, stride)
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
   local net = nn.SpatialConvolution(nChannels, nOutChannels,
                                           3,3, stride,stride, 1,1)
                                           :init('weight', nninit.kaiming, {gain = 'relu'})
                                           :init('bias', nninit.constant, 0)(input)
   net = nn.SpatialBatchNormalization(nOutChannels)
                                            :init('weight', nninit.normal, 1.0, 0.002)
                                            :init('bias', nninit.constant, 0)(net)
   net = nn.ReLU(true)(net)
   net = nn.SpatialConvolution(nOutChannels, nOutChannels,
                                      3,3, 1,1, 1,1)
                                      :init('weight', nninit.kaiming, {gain = 'relu'})
                                      :init('bias', nninit.constant, 0)(net)
   -- Should we put Batch Normalization here? I think not, because
   -- BN would force the output to have unit variance, which breaks the residual
   -- property of the network.
   -- What about ReLU here? I think maybe not for the same reason. Figure 2
   -- implies that they don't use it here

   -- Path 2: Identity / skip connection
   local skip = input
   if stride > 1 then
       -- optional downsampling
       skip = nn.SpatialAveragePooling(1, 1, stride,stride)(skip)
   end
   if nOutChannels > nChannels then
       -- optional padding
       skip = nn.Padding(1, (nOutChannels - nChannels), 3)(skip)
   elseif nOutChannels < nChannels then
       -- optional narrow, ugh.
       skip = nn.Narrow(2, 1, nOutChannels)(skip)
       -- NOTE this BREAKS with non-batch inputs!!
   end

   -- Add them together
   net = nn.SpatialBatchNormalization(nOutChannels)(net)
   net = nn.CAddTable(){net, skip}
   net = nn.ReLU(true)(net)
   -- ^ don't put a ReLU here! see http://gitxiv.com/comments/7rffyqcPLirEEsmpX

   return net
end

function MakeNeuronCriterion()
  N =3;
  input = nn.Identity()()
  ------> 1, 96,96
  model = nn.SpatialConvolution(1, 6, 3,3, 1,1, 1,1)
              :init('weight', nninit.kaiming, {gain = 'relu'})
              :init('bias', nninit.constant, 0)(input)
  model = nn.SpatialBatchNormalization(6)(model)
  model = nn.ReLU(true)(model)
  ------> 16, 48,48   First Group
  for i=1,N do   model = _getResidualLayer(model, 6)   end
  
  ------> 32, 24,24   Second Group
  model = _getResidualLayer(model, 6, 12, 2)
  for i=1,N-1 do   model = _getResidualLayer(model, 12)   end
  ------> 64, 12,12     Third Group  
  model = _getResidualLayer(model, 12, 24, 2)
  for i=1,N-1 do   model = _getResidualLayer(model, 24)   end
  
  model = _getResidualLayer(model, 24, 48, 2)
  for i=1,N-1 do   model = _getResidualLayer(model, 48)   end
  
  ------> 64, 1,1     Pooling, Linear, Softmax
  model = nn.SpatialAveragePooling(3,3)(model)
  model = nn.Reshape(48)(model)
  model = nn.Linear(48, 12)(model)
  model = nn.ReLU(true)(model);
  model = nn.Linear(12, 5)(model)
  
  model = nn.gModule({input}, {model})
  criterion = nn.MSECriterion();
  return model, criterion;
end
