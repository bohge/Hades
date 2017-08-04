nativelibrary("nativesystem");

local ffi = require "ffi";

function LOG(log)
  ILogSystem:Log(log);
end
function WARNING(log )
  ILogSystem:Warning(log);
end
function ERROR( log)
  ILogSystem:Error(log);
end

function Print_Table(root)
  local cache = {  [root] = "." }
  local function _dump(t,space,name)
    local temp = {}
    for k,v in pairs(t) do
      local key = tostring(k)
      if cache[v] then
        table.insert(temp,"+" .. key .. " {" .. cache[v].."}")
      elseif type(v) == "table" then
        local new_key = name .. "." .. key
        cache[v] = new_key
        table.insert(temp,"+" .. key .. _dump(v,space .. (next(t,k) and "|" or " " ).. string.rep(" ",#key),new_key))
      else
        table.insert(temp,"+" .. key .. " [" .. tostring(v).."]")
      end
    end
    return table.concat(temp,"\n"..space)
  end
  print(_dump(root, "",""))
end

function Utility_CapnpDecoder(serializer, buffer)
  local cbuff = buffer:Data();
  local len = buffer:GetMessageLength();
  local strbuf = ffi.string(cbuff, len);
  local restable = serializer.parse(strbuf);
  return restable;
end


function Utility_CapnpEncoder(serializer, id, datatable)
  local msg = IMessage();
  local cbuff = msg:Data();
  local size = serializer.calc_size(datatable);
  serializer.serialize(datatable, cbuff, size);
  msg:SetID(id);
  msg:SetMessageLength(size);
  return msg;
end
