nativelibrary("nativesystem");

local json = require "dkjson"
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

function Utility_StringDecoder(msg)
  local cbuff = msg:GetMessageBuffer();
  local len = msg:GetMessageLength();
  local strbuf = ffi.string(cbuff, len);
  return strbuf;
end

function Utility_StringEncoder(id, str)
  local msg = Message();
  local cbuff = msg:PrepareMessage(id, #str);
  ffi.copy(cbuff, str, #str);
  return msg;
end

function Utility_TableDecoder(msg)
  local strbuf = Utility_StringDecoder(msg)
  local obj, pos, err = json.decode(strbuf);
  if err then
    ERROR("json error:", err)
    return nil;
  end
  return obj;
end

function Utility_TableEncoder(id, obj)
  local str = json.encode(obj);
  return Utility_StringEncoder(id, str);
end

function Sleep(n)
   if n > 0 then os.execute("ping -n " .. tonumber(n + 1) .. " localhost > NUL") end
end

string.split = function(s, p)
    local rt= {}
    string.gsub(s, '[^'..p..']+', function(w) table.insert(rt, w) end )
    return rt
end