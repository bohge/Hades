
if _DEBUG then
  --local debuger = require "debug/mobdebug";
  --debuger.start();
end


nativelibrary("nativesystem");
nativelibrary("application");
nativelibrary("3dmath");

g_Application = nil;

local ffi = require "ffi";

local GlobalDataList = require "globaldatalist";
local Dispatcher = require "dispatchhandle";

update = {};

local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;

function update.Initialize(instance)  
  GlobalDataList:Initialize();
  
  --发送初始化消息
  Dispatcher:Dispatch(GenID.GID_UPDATE_INITIALIZE, nil, nil);
  
end


function update.OnTimerTriggered(sec)
 	--Dispatcher:Dispatch(GenID.GID_ROOM_TICK, nil, nil);
end
