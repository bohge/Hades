local capnp = require "arclight.cluster_capnp"
local capnpid = capnp.ClusterMessageID;
local ffi = require "ffi"
require "utility"

local function _BufferSize(tensor)
  local t = tensor.type(tensor)
  local size = tensor:storage():size();
  if t == "torch.ByteTensor" then
    return size;
  elseif t == "torch.CharTensor" then
    return size;
  elseif t == "torch.ShortTensor" then
    return size*2;
  elseif t == "torch.IntTensor" then
    return size*4;
  elseif t == "torch.LongTensor" then
    return size*4;
  elseif t == "torch.FloatTensor" then
    return size*4;
  elseif t == "torch.DoubleTensor" then
    return size*8;
  elseif t == "torch.HalfTensor" then
    return size*2;
  end
  return -1;
end

local function _TensorDimension(tensor)
  local t = {};
  for i=1,tensor:dim() do
    table.insert(t, tensor:size(i));
  end
  return t;
end


local function Tensor2Message(tensor)
  local fullsize = _BufferSize(tensor);
  
  local info = {
    tensortype = tensor:type();
    buffersize = tensor:storage():size();
    dimension = _TensorDimension(tensor);
  }
  local struct = Utility_CapnpEncoder(capnp.TensorTransfer, capnpid.CMID_TENSOR_TRANSFER, info);
  
  local buffer = tensor:storage():buffer();
  local msgbuffer = IMessage();
  local cbuff = msgbuffer:PrepareMessage(capnpid.CMID_TENSOR_TRANSFER, fullsize);
  ffi.copy(cbuff, buffer, fullsize);
  return struct, msgbuffer;
end
return Tensor2Message;