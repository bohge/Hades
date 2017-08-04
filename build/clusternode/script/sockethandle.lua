nativelibrary("nativesystem");
require "utility"

local sockethandle = {}

function sockethandle:OnReciveRequest(gid)
  LOG("OnReciveRequest");
end

function sockethandle:OnConnect(gid)
  LOG("OnConnect");
end

return sockethandle;