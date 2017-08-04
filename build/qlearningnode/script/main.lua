--_DEBUG = true;

if _DEBUG then
  local debuger = require "debug/mobdebug";
  debuger.start();
  _COROUTINES_ON = debuger.on;
  _COROUTINES_OFF = debuger.off;
else
  _COROUTINES_ON = function() end;
  _COROUTINES_OFF = function() end;
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
  local seed = os.time();
  print('Seed: ', seed);
  math.randomseed(seed);
  torch.manualSeed(seed);
  self.RPCServer = IServerSystem:Create(IServerSystem.ST_COMMON_TL);
  IServicesSystem:SetServer(self.RPCServer);
  self.RPCServer:Start(2, 9999);
  --将服务监听与用户监听设置为同一个服务器可以做到rpc与用户连接一体的结构
  --将服务监听与用户监听设置为不同服务器可以做到分层结构
  IServicesSystem:SetServer(self.RPCServer);
  --设置rpc接受者要在一切IServicesSystem:Create之前调用
  
  --创建中心服务
  --self.centrcenode = IServicesSystem:Create("arclight.centrcenode_pron_new");
  --self.centrcenode:Setup();
  
  --初始化远程服务体系
  self.remotehandler = IServicesSystem:Create("remotehandle");
  self.remotehandler:Initialize(self.centrcenode);
  IServicesSystem:SetHandler(self.remotehandler);
  
  IServicesSystem:ScriptInitialized();
  
  _COROUTINES_ON();  
  self.qlearning = require "qlearningcenter";
  self.qlearning:Initialize();
  --self.qlearning:Load();
  self.qlearning:Trainning();
  _COROUTINES_OFF();
end

function main:OnCommandline(command)
  --tout = arclight.commandparser(command);
  if "go" == command then
    self.centrcenode:Training(0.1, 100000, 200, 1e-4);
  elseif "stop" == command then
    self.centrcenode:Stop();
  elseif "verify" == command then
    self.centrcenode:Verification();
  elseif "save" == command then
    self.centrcenode:Save();
  elseif "load" == command then
    self.centrcenode:Load();
  elseif "ending" == command then
    self.centrcenode:LittleBigEnding();
  elseif "ser" == command then
    self.centrcenode:TorchSerialize();
  end  
end

return main;