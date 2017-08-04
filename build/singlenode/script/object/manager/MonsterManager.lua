
--MonsterManager


local ObjectManager = require "object.manager.ObjectManager"
local ObjectMonster = require "object.ObjectMonster"
local DataTable = require "DataTable"
local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local Dispatcher = require "dispatchhandle";

local MonsterManager={
	ObjMgrGID,
	MonsterMap,
	SceneConfig,
	RoomGID,
};

MonsterManager.__index = MonsterManager;

function MonsterManager:new(o)
	o = o or {};
	setmetatable(o,MonsterManager);		
	return o;	
end
function MonsterManager:Cast(o)	
	self.__index = self;	
	setmetatable(o,self);
	return o;	
end
function MonsterManager:Init(objmgrgid,roomgid)
  	self.ObjMgrGID = objmgrgid;
  	self.RoomGID = roomgid;
	self.MonsterMap = {};   
	self.SceneConfig = DataTable:GetSceneConfig(1);
end

function MonsterManager:AddMonster()	

	local Monster = CMEM_CREATE(ObjectMonster);
	local MonsterMemGID = Monster._GID;	
	CMEM_FREE(Monster);
	

	--添加到ObjectManger中设置ServerID		
	local ObjMgr = CMEM_GET_RW(ObjectManager,self.ObjMgrGID)
	ObjMgr:AddObject(Monster);
	self.MonsterMap[Monster:GetServerID()] = MonsterMemGID;		
	CMEM_FREE(ObjMgr);


	local MsgPvpNotifySpawnMonsterInfo = 
    {
      serverid = self.MissionLeftTime;
      type = 0;
      speed = 0;
      hp = 0 ;
    }
    Utility_CapnpEncoder(
      msg_capn.MsgPvpNotifySpawnMonster,
      MsgID["MID_MSG_PVP_NOTIFY_SPAWN_MONSTER"],
      MsgPvpNotifySpawnMonsterInfo);    
	self.GetRoom():BroadcastMsgToPlayers(g_Buffer);
    return Monster;    

end
function MonsterManager:GetMonsterCount(  )
	return Utility_GetMapCount(self.MonsterMap);
end
function MonsterManager:Tick(DeltaTime)
	if(self.MonsterMap == nil) then
		return ;
	end
	for k,v in pairs(self.MonsterMap) do
		local MonsterMemGID = v;	
		local Monster = CMEM_GET_R(ObjectMonster,MonsterMemGID);
		Monster:Tick(DeltaTime);
	end
end

function MonsterManager:AddSceneMonsters()	
	local MonsterPositions = self.SceneConfig.MonsterPositions;
	for i=1,#MonsterPositions do
		self:AddMonster();
	end	
end
function MonsterManager:Release()
  
end
function MonsterManager:GetRoom( )		
	local Room = require "pvp.Room";
	return CMEM_GET_R(Room,self.RoomGID);
end


return MonsterManager;