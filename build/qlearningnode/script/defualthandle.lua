nativelibrary("nativesystem");

require "utility"

local defualthandle = {}


function defualthandle:OnReciveRequest(gid)
  LOG("OnReciveRequest");
end

function defualthandle:OnDisconnect(gid)
  LOG("OnDisconnect");
end

return defualthandle;