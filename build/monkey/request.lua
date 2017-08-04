local macro = require "macro";


local _COROUTINES;
if _DEBUG then
  local debuger = require "debug/mobdebug";
  debuger.start();
  _COROUTINES = debuger.on;
else
  _COROUTINES = function()end;
end



require "utility/utility"


nativelibrary("nativesystem");
nativelibrary("application");
nativelibrary("3dmath");

g_Server = nil;
g_Machine = nil;
g_Application = nil;


g_Request = nil;
g_Buffer = nil;

local ffi = require "ffi";
local GlobalDataList = require "globaldatalist";
local PlayerList = require "globalplayerlist";
local Messagehandle = require "messagehandle";
local Dispatcher = require "dispatchhandle";

local DispatcherRegisteTable = 
{
  require "update/appinitialize",
  require "update/broadcastplayerinfo"
}

local Logout = require "handle/logouthandle";

local MessageRegisterTable =
{
  Logout,
  require "handle/loginhandle",
  require "handle/entryscenehandle",
  require "handle/leavescenehandle",
  require "handle/playermovemeet",
  require "handle/playermovestart",
  require "handle/playermovestop",
  require "handle/playerface",
  require "handle/playerhandposrotation",
  require "handle/playerfire",
  require "handle/playerreload",
  require "handle/playerchangeweapon",
  require "handle/playerrevive",
  require "handle/projectiledamage",
  
  
  
  require "handle/simpleecho"
}



request = {};
function request.Initialize(instance)
  PlayerList:Initialize();
  GlobalDataList:Initialize();
  --初始化内部分发系统
  Messagehandle:RegisteHandle(Dispatcher);
  --注册内部分发系统响应
  for k,v in ipairs(DispatcherRegisteTable) do
    Dispatcher:RegisteHandle(v);
  end
    
  --初始化网络响应
  for k,v in ipairs(MessageRegisterTable) do
    Messagehandle:RegisteHandle(v);
  end
end

function request.OnConnect(gid, request, buffer)
  _COROUTINES();
  while true do
    gid, request, buffer = coroutine.yield();
  end
end

function request.OnReciveRequest(gid, request, buffer)
  _COROUTINES();
  while true do
    Messagehandle:HandleMessage(gid, request, buffer);
    gid, request, buffer = coroutine.yield();
  end
end

function request.OnDisconnect(gid, request, buffer)
  _COROUTINES();
  while true do
    Logout:OnDisconnect(gid, request, buffer);
    gid, request, buffer = coroutine.yield();
  end
end