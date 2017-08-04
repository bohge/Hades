local capnp = require "arclight.cluster_capnp"
local capnpid = capnp.ClusterMessageID;
local ffi = require "ffi"
require "utility"

local function _NewTensor(t, dim)
  if t == "torch.ByteTensor" then
    return torch.Tensor(torch.LongStorage(dim)):byte();
  elseif t == "torch.CharTensor" then
    return torch.Tensor(torch.LongStorage(dim)):char();
  elseif t == "torch.ShortTensor" then
    return torch.Tensor(torch.LongStorage(dim)):short();
  elseif t == "torch.IntTensor" then
    return torch.Tensor(torch.LongStorage(dim)):int();
  elseif t == "torch.LongTensor" then
    return torch.Tensor(torch.LongStorage(dim)):long();
  elseif t == "torch.FloatTensor" then
    return torch.Tensor(torch.LongStorage(dim)):float();
  elseif t == "torch.DoubleTensor" then
    return torch.Tensor(torch.LongStorage(dim)):double();
  elseif t == "torch.HalfTensor" then
    return torch.Tensor(torch.LongStorage(dim)):half();
  end
end

local function Message2Tensor(struct, buffer, tensor)
  local info = Utility_CapnpDecoder(capnp.TensorTransfer, struct);
  if nil == tensor then
    tensor = _NewTensor(info.tensortype, info.dimension);
  else
    assert( info.tensortype == tensor:type() );
    assert( info.buffersize == tensor:storage():size() );
  end  
  local dst = tensor:storage():buffer();
  local src = buffer:GetMessageBuffer();
  local len = buffer:GetMessageLength();
  ffi.copy(dst, src, len);
  return tensor;
end


return Message2Tensor;