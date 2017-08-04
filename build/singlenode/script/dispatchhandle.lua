g_Application = nil;


local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local Type = require "protocol/msg_generic_job_capnp".MsgValueType;
local msg_generic_job = require "protocol/msg_generic_job_capnp";

local ffi = require "ffi";


local DispatchHandle = {};
DispatchHandle.Handler = {};
DispatchHandle.MsgID = MsgID["MID_GENERIC_JOB"];

function DispatchHandle:RegisteHandle(id, instance, handler)    
  assert( nil ~= id );
  assert( nil ~= handler );
  assert( nil == self.Handler[id] );
  self.Handler[id] =
  {
    Instance = instance,
    Function = handler,
  }    
end


function DispatchHandle:_ValueType_Value(value)
  if Type.VT_STRING == Type[value.valuetype] then
    return value.stringvalue;
  elseif Type.VT_NUMBER == Type[value.valuetype] then
    return value.numbervalue;
  elseif Type.VT_BOOL == Type[value.valuetype] then
    if nil == value.boolvalue then
       return false;
    else
       return value.boolvalue;
    end
  elseif Type.VT_UINT64 == Type[value.valuetype] then
    return value.uint64value;
  else
    assert(false);--unkonw type;
  end
  return nil;
end


function DispatchHandle:_AssemblyTable(valuepairs)
  if nil ~= valuepairs then
    local res = {};
    for k, v in pairs(valuepairs) do    
      res[v.key] = self:_ValueType_Value(v.value);
    end
    return res;
  end
  return nil;
end

function DispatchHandle:OnRequest()
  local inputinfo = Utility_CapnpDecoder( msg_generic_job.MsgGenericMessage, buffer );
  
  local mesid = GenID[inputinfo.id];
  local handler = self.Handler[mesid];
  local datas = self:_AssemblyTable(inputinfo.pairs);
  assert(nil ~= handler);
  handler.Function(handler.Instance,  datas);
end


function DispatchHandle:_Value_ValueType(value)
  if nil ~= value then
    local vt = type(value);
    local res = {};
    if "string" == vt then
      res.valuetype = Type.VT_STRING;
      res.stringvalue = value;
    elseif "number" == vt then
      res.valuetype = Type.VT_NUMBER;
      res.numbervalue = value;
    elseif "boolean" == vt then
      res.valuetype = Type.VT_BOOL;
      res.boolvalue = value;
    elseif "cdata" == vt then
      if ffi.istype("uint64_t", value) then
        res.valuetype = Type.VT_UINT64;
        res.uint64value = value;
      else
        assert(false);
      end      
    else
      assert(false);--unkonw type;
    end
    return res;
  end
  return nil;
end

function DispatchHandle:_DessemblyTable(datas)
  if nil ~= datas then
    local res = {};
    for k, v in pairs(datas) do    
      local valuepair = {};
      valuepair.key = k;
      valuepair.value = self:_Value_ValueType(v);
      table.insert(res, valuepair)
    end
    return res;
  end
  return nil;
end

function DispatchHandle:Dispatch(msgid, gid, datas)  
  local gmsg = {};
  gmsg.id = msgid;
  gmsg.pairs = self:_DessemblyTable(datas);
  local msg = Message();
  local buffer = msg:Data();
  local size = msg_generic_job.MsgGenericMessage.calc_size(gmsg);
  msg_generic_job.MsgGenericMessage.serialize(gmsg, buffer, size);
  
  msg:SetID(self.MsgID);
  msg:SetLength(size);
  g_Application:DispatchRequest(gid, msg);
end

return DispatchHandle;