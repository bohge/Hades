nativelibrary("nativesystem");


local capnp = require "arclight.cluster_capnp"
local capnpid = capnp.ClusterMessageID;
require "utility"

--local hostip = "10.12.26.44:9999";
local hostip = "10.12.25.224:9999";
--local hostip = "192.168.1.140:9999";

local main = {};
main.RPCServer = nil;
main.clusternode = nil;


function main:Initialize()
  self.RPCServer = IServerSystem:Create(IServerSystem.ST_COMMON_TL);
  IServicesSystem:SetServer(main.RPCServer);
  self.RPCServer:Start(1, 8080);
  
  local addr = self.RPCServer:GetServerAddress();
  local seed = NativeUtility:HashCode(addr);
  self.clusternode = IServicesSystem:Create("arclight.t_pddqn_qlearclustenode_cnn");
  self.clusternode:Setup(seed, self.clusternode);
  
  --初始化远程服务体系
  self.remotehandler = IServicesSystem:Create("remotehandle");
  local ip = self.RPCServer:GetServerAddress();
  local port = self.RPCServer:GetPort();
  self.remotehandler:Initialize(self.clusternode, ip, port);
  IServicesSystem:SetHandler(self.remotehandler);
  
  --连接host
  IServicesSystem:ConnectNode(ISocketSystem.ST_COMMON_TL, hostip);
  
  IServicesSystem:ScriptInitialized();
  
  --[[self.socket = ISocketSystem:Create(ISocketSystem.ST_COMMON_TL);
  local handle = IServicesSystem:Create("sockethandle");
  self.socket:PushDefualtHandler(handle);
  self.socket:Connect(hostip);]]--
end


return main;