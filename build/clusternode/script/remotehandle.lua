nativelibrary("nativesystem");

local capnp = require "arclight.cluster_capnp"
local capnpid = capnp.ClusterMessageID;
require "arclight"
require "utility"


local remotehandle = {}

function remotehandle:Initialize(clusternode, ip, port)
  self.clusternode = clusternode;
  self.clusterinfo = {};
  self.clusterinfo.ipadress = ip;
  self.clusterinfo.port = port;
  LOG("remotehandle:Initialize");
end

function remotehandle:OnRemotenodeReady(gid, targethash)
  LOG("OnRemotenodeReady " .. targethash);
  msg = Utility_CapnpEncoder(capnp.IncomeConnect, 0, self.clusterinfo);
  IServicesSystem:SendAgent(targethash, self.clusternode, msg);
end

function remotehandle:OnReceiveAgent(gid, agent, msg)
  LOG("OnReceiveAgent");
end

function remotehandle:OnRemotenodeDisconnect(gid)
  self.clusternode:SetCentrcenode(nil);
  strgid = tostring(gid);
  LOG("OnRemotenodeDisconnect " .. strgid);
  IServicesSystem:ConnectNode(ISocketSystem.ST_COMMON_TL, hostip);
end

return remotehandle;