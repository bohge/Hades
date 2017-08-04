local msg_capn = require "protocol/msg_capn_capnp"

local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local PlayerList = require "globalplayerlist";
local GlobalDataList = require "globaldatalist";
local Dispatcher = require "dispatchhandle";
local ffi = require "ffi";

local ObjectManager = require "object/manager/ObjectManager"
local RoomServer = require "pvp/RoomServer"
local Room = require "pvp/Room"
local DataTable = require "datatable"
local AppInitalize = {};

local testfunc = require "testrequest"


function AppInitalize:Initialize()
  Dispatcher:RegisteHandle( GenID["GID_UPDATE_INITIALIZE"], AppInitalize, AppInitalize.OnGenericJob );
end

function AppInitalize:OnGenericJob(datas) 
  testfunc();
	print("AppInitalize:OnGenericJob") 
  g_Request:CreateAccessor(PlayerList._HandleSelf);  
  g_Request:CreateAccessor(DataTable._HandleSelf)
  g_Request:GetAccessors() 
	GlobalDataList.PlayerListAccessor.GID = PlayerList._GID;
  GlobalDataList.DataTableAccessor.GID = DataTable._GID;
  DataTable:OpenAll()
  --g_Request:FreeAccessors();
 

  local RoomTable = g_Request:CreateAccessor();
  g_Request:GetAccessors()            
  local Room = Room:new(RoomTable);      
  GlobalDataList.PlayerListAccessor.RoomGID = Room._GID;    
  Room:Init(request) ; 
      
  g_Request:FreeAccessors(Room._GID);

  IScriptSystem:Synchronize( GlobalDataList._HandleInstance);
  print("AppInitalize done");
  --Dispatcher:Dispatch(GenID.GID_GAME_RULE_INIT, nil, nil);	

end

return AppInitalize;