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
nativelibrary("3dmath");

g_Server = nil;
g_Machine = nil;
g_Application = nil;


g_ClientID = nil;
g_Request = nil;
g_Buffer = nil;

local ffi = require "ffi";
local GlobalDataList = require "globaldatalist";
local PlayerList = require "globalplayerlist";
local Messagehandle = require "messagehandle";
local Dispatcher = require "dispatchhandle";
local Room = require "pvp/Room";
local DataTable = require "datatable"

local DispatcherRegisteTable = 
{
  require "update/appinitialize",
  require "update/broadcastplayerinfo",
  --require "pvp/GameRules",
  --require "handlenew/testjson"
}

local Logout = require "handlenew/logouthandle";

local MessageRegisterTable =
{
  Logout,
  require "handlenew/loginhandle",
  require "handlenew/entryscenehandle",
  require "handlenew/leavescenehandle",
  require "handlenew/playermovemeet",
  require "handlenew/playermovestart",
  require "handlenew/playermovestop",
  require "handlenew/playerface",
  require "handlenew/playerhandposrotation",
  require "handlenew/playerfire",
  require "handlenew/playerreload",
  require "handlenew/playerchangeweapon",
  require "handlenew/playerrevive",
  require "handlenew/projectiledamage",
  
  
  
  require "handlenew/simpleecho"
}



request = {};
function request.Initialize(instance)
  PlayerList:Initialize();
  GlobalDataList:Initialize();
  DataTable:Initialize();
  --初始化内部分发系统
  Messagehandle:RegisteHandle(Dispatcher);
  --注册内部分发系统响应
  for k,v in ipairs(DispatcherRegisteTable) do
    if nil ~= v.Initialize then
      v:Initialize();
  end 
  end
    
  --初始化网络响应
  for k,v in ipairs(MessageRegisterTable) do
    Messagehandle:RegisteHandle(v);
  end
end

function request.OnConnect()
  _COROUTINES();
  while true do
    coroutine.yield();
  end
end
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
function request.OnReciveRequest()
    _COROUTINES();

    while true do
      
      if(g_Buffer:GetID() == MsgID["MID_GENERIC_JOB"]) then
        Messagehandle:HandleMessage(nil); 
      else
        local RoomGID = GlobalDataList.PlayerListAccessor.RoomGID ;  
        
        local RoomTable = g_Request:ReadonlyAccessorImmediately(RoomGID);          
        local Room = Room:new(RoomTable);       
         g_Request:FreeAccessors();           
        print("RoomGID "  .. tostring(RoomGID));
        Messagehandle:HandleMessage(Room);  
  
       
       
      end
        
        coroutine.yield();
        
    end
end

function request.OnDisconnect()
  _COROUTINES();
  while true do
    Logout:OnDisconnect();
    coroutine.yield();
  end
end