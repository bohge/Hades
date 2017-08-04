local ffi = require "ffi";
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;


function CMEM_CREATE(class)
    return class:Cast(g_Request:CreateAccessorImmediately());
end

function CMEM_GET_R( class,gid )
  return class:Cast(g_Request:ReadonlyAccessorImmediately(gid));  
end
function CMEM_GET_RW( class,gid )
  return class:Cast(g_Request:ReadwriteAccessorImmediately(gid));
end
function CMEM_FREE( obj )
  local gid = obj._GID;
  g_Request:FreeAccessors(gid); 
end
function CMEM_FREE_GID( gid )
  g_Request:FreeAccessors(gid); 
end
function CMEM_CAST( class,table )
  return class:Cast(table)
end


function Utility_GetMapCount( map )
  if(map == nil) then
    return 0;
  end
  local cnt = 0 ;
  for k,v in pairs(map) do
    cnt = cnt+1;
  end
  return cnt;
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

function Print_Message(id, root)
  for k,v in pairs(MsgID) do
    if id == v then
      print("\r\nMessage name " .. tostring(k));
    end
  end
  Print_Table(root);
end


function Utility_CapnpDecoder(serializer, datatable)
  local cbuff = g_Buffer:Data();
  local len = g_Buffer:GetLength();  
  local strbuf = ffi.string(cbuff, len);
  local restable = serializer.parse(strbuf, datatable);
  return restable,cbuff;
end


function Utility_CapnpEncoder(serializer, id, datatable)
    local cbuff = g_Buffer:Data();
    local size = serializer.calc_size(datatable);
    serializer.serialize(datatable, cbuff, size);
    g_Buffer:SetID(id);
    g_Buffer:SetLength(size);
    --[[if id == 0
    or id == 1
    or id == 13
    or id == 15
    or id == 16 then
      Print_Message(id, datatable);
    end]]--        
end