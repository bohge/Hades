nativelibrary("nativesystem");


local capnp = require "arclight.cluster_capnp"
local capnpid = capnp.ClusterMessageID;
require "arclight"
require "utility"


local remotehandle = {}

function remotehandle:Initialize(centrcenode)
  self.centrcenode = centrcenode;
  LOG("remotehandle:Initialize");
end

function remotehandle:OnRemotenodeReady(gid, targethash)
  LOG("OnRemotenodeReady " .. targethash);
end

function remotehandle:OnReceiveAgent(gid, agent, msg)
  strgid = tostring(gid);
  LOG("OnReceiveAgent " .. strgid);
  --远程节点会发送来clusternode
  local msgid = msg:GetID();
  tableres = Utility_CapnpDecoder(capnp.IncomeConnect, msg);
  local count = self.centrcenode:InsertComputernode(strgid, agent);
  --远程节点准备完毕后，我们将中心节点服务发送给对方
  agent:SetCentrcenode(self.centrcenode);
  --开始测试
  LOG("connect cluster count is " .. count);
end

function remotehandle:OnRemotenodeDisconnect(gid)
  strgid = tostring(gid);
  LOG("OnRemotenodeDisconnect " .. strgid);
  self.centrcenode:RemoveComputernode(strgid);
end

return remotehandle;