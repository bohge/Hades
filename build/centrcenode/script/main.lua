--_DEBUG = true;
if _DEBUG then
  local debuger = require "debug/mobdebug";
  debuger.start();
  _COROUTINES = debuger.on;
else
  _COROUTINES = function() end;
end
  
  
nativelibrary("nativesystem");

require "arclight"
require "utility"



local main = {};
main.RPCServer = nil;
main.centrcenode = nil;
main.defualthandles = nil;
main.remotehandler = nil;

function main:Initialize()
  self.RPCServer = IServerSystem:Create(IServerSystem.ST_COMMON_TL);
  --初始化默认消息接受处理器
  self.defualthandles = {};
  for i=1, 4 do
    local handle = IServicesSystem:Create("defualthandle");
    table.insert(self.defualthandles, handle);
    self.RPCServer:PushDefualtHandler(handle);
  end
  
  --将服务监听与用户监听设置为同一个服务器可以做到rpc与用户连接一体的结构
  --将服务监听与用户监听设置为不同服务器可以做到分层结构
  --设置rpc接受者要在一切IServicesSystem:Create之前调用
  IServicesSystem:SetServer(self.RPCServer);
  
  self.RPCServer:Start(2, 9999);
  
  --创建中心服务
  self.centrcenode = IServicesSystem:Create("arclight.t_pddqn_qlearcentrnode_cnn");
  self.centrcenode:Setup();
  
  --初始化远程服务体系
  self.remotehandler = IServicesSystem:Create("remotehandle");
  self.remotehandler:Initialize(self.centrcenode);
  IServicesSystem:SetHandler(self.remotehandler);
  
  IServicesSystem:ScriptInitialized();
end

function main:OnCommandline(command)
  --  -go rate=0.001 epsion=100000
  --  rate=0.002
  --  -stop
  --  ...
  tout = arclight.commandparser(command);
  if tout["go"] then
    local lr = tout["rate"] or 0.01;
    local epsion = tout["epsion"] or 1000000;
    self.centrcenode:Trainning(lr, epsion);
  elseif tout["episode"] then
    local episode = tout["episode"];
    self.centrcenode:SetEpisode(episode);
  elseif tout["rate"] then
    local max = tout["max"];
    local min = tout["min"];
    self.centrcenode:SetLearningRate(max, min);
  elseif tout["freq"] then
    local t = tout["freq"];
    self.centrcenode:SetSwitchFrequency(t);   
  elseif tout["times"] then
    local t = tout["times"];
    self.centrcenode:SetTrainingTimes(t);    
  elseif tout["stop"] then
    self.centrcenode:Stop();
  elseif tout["verify"] then
    self.centrcenode:Verification();
  elseif tout["save"] then
    self.centrcenode:Save();
  elseif tout["load"] then
    self.centrcenode:Load();
  elseif tout["ending"] then
    self.centrcenode:LittleBigEnding();
  elseif tout["ser"] then
    self.centrcenode:TorchSerialize();
  end  
end

return main;