local ObjectManager = require "object.manager.ObjectManager"
local PlayerManager = require "object.manager.PlayerManager"
local MonsterManager = require "object.manager.MonsterManager"
local ObjectPlayer = require "object.ObjectPlayer"
local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local Dispatcher = require "dispatchhandle";
local GameRulers = require "pvp.GameRulers"
local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local GlobalDataList = require "globaldatalist";
local Room={
	ObjMgrGID,	
	GameRulersGID,--和游戏业务逻辑相关的东西
	PlayerMgrGID,
	MonsterMgrGID,
}
Room.__index = Room;
 function Room:new(o)
	o = o or {};	
	setmetatable(o,Room);
	return o;	
end
function Room:Cast(o)		
	setmetatable(o,Room);
	return o;	
end
function Room:Init( request )
	
	--创建ObjectManger	
	local ObjMgr =CMEM_CREATE(ObjectManager)
	ObjMgr:Init();
  self.ObjMgrGID = ObjMgr._GID;  
  CMEM_FREE(ObjMgr);


	local PlayerMgr = CMEM_CREATE(PlayerManager);	
  	PlayerMgr:Init(self.ObjMgrGID, self._GID);  	
  CMEM_FREE(PlayerMgr);	

	local MonsterMgr =CMEM_CREATE(MonsterManager);
  	MonsterMgr:Init(self.ObjMgrGID, self._GID);  
  CMEM_FREE(MonsterMgr);	

	local GameRulers = CMEM_CREATE(GameRulers);
  	GameRulers:Init(self._GID);  	
  CMEM_FREE(GameRulers);

	
  	self.PlayerMgrGID = PlayerMgr._GID;   
	self.MonsterMgrGID = MonsterMgr._GID;   
	self.GameRulersGID = GameRulers._GID;  

	
	
	
	


	self.LastTickTime = -1;
	Dispatcher:RegisteHandle( GenID["GID_ROOM_TICK"], self, Room.Tick );
end

function Room:Tick(datas)
	local DeltaTime = self:CalcDeltaTime();
	if(DeltaTime == -1) then
		return ;
	end
  	--print("Tick  PlayerCount ===  " .. self:GetRoomPlayerCount());
	self:TickGameRulers(DeltaTime);
	self:TickPlayer(DeltaTime);
	self:TickMonster(DeltaTime);
	
end
function Room:CalcDeltaTime( )
	local Second =  ITimerSystem:GetTimevalue();    
    if(self.LastTickTime == -1) then
    	self.LastTickTime = Second;
    	return -1;
    end
    local DeltaTime = Second - self.LastTickTime;
    self.LastTickTime = Second;  
    return DeltaTime;
end
function Room:TickGameRulers(deltatime )	
	local Ruler =  CMEM_GET_RW(GameRulers,self.GameRulersGID);
	Ruler:Tick(deltatime);
	CMEM_FREE(Ruler);
end


--场景相关的东西
function Room:LoadScene( ... )
	-- body
	--config
end

function Room:AddPlayer()	
	local PlayerMgr = self:GetPlayerManager(true);
	local Player = PlayerMgr:AddPlayer();
	CMEM_FREE(PlayerMgr);
	
	local newplayerinfo =
    {
      playerid = gid,
      devicetype = 0,
      dir = 0,
      posx = 0,
      posy = 0,
      posz = 0,
      camp = 0,
      weaponids = 0,
      curweaponid = 0
    }
    Utility_CapnpEncoder(
      msg_capn.MsgBroadcastPlayerInfo,
      MsgID["MID_MSG_BROADCAST_PLAYER_INFO"],
     	newplayerinfo);
    self:BroadcastMsgToOhterPlayers(gid,buffer);
	
    return Player;
end
function Room:DeletePlayer( )
	
end

function Room:GetPlayerByGID( gid)
	local PlayerGid = g_Server:GetClientSpecific(gid); 
	local ObjTable = g_Request:ReadonlyAccessorImmediately(PlayerGid);
	local Player = ObjectPlayer:new(ObjTable); 
	return Player;		
end
function Room:BroadcastMsgToPlayers(buffer)	
	print("--------------------BroadcastMsgToPlayers------------")
	local PlayerMgr =  self:GetPlayerManager(false);	
	local ClientIDArray = PlayerMgr:GetClientIDArray();

	if(ClientIDArray == nil) then
		return ;
	end
	for i=1,#ClientIDArray do
		local ClientID = ClientIDArray[i];		
		g_Server:SendMessage(ClientID, g_Buffer);	
	end
end
function Room:BroadcastMsgToOhterPlayers(gid,buffer)	
	-- for player
	self:BroadcastMsgToPlayers(buffer);
	 
end
function Room:GetRoomPlayerCount()	
	return self:GetPlayerManager():GetPlayerCount();
end

--User相关的东西

function Room:TickPlayer( deltatime )	
	 self:GetPlayerManager():Tick(deltatime);
end

--玩家掉线消息
function Room:DropPlayer(  )
	-- body
end



--场景Monster相关的东西
function Room:LoadMonster( ... )
	-- body
end

function Room:TickMonster(deltatime )
	self:GetMonsterManager():Tick(deltatime,request);
end
function Room:FindMonster( ... )
	-- body
end

function Room:GetMonsterManager(isrw)

	isrw = isrw or false;
	if(isreadwrite) then
		return CMEM_GET_RW(MonsterManager,self.MonsterMgrGID);
	else
		return CMEM_GET_R(MonsterManager,self.MonsterMgrGID);
	end	
end
function Room:GetPlayerManager(isrw)
	isrw = isrw or false;
	if(isreadwrite) then
		return CMEM_GET_RW(PlayerManager,self.PlayerMgrGID);
	else
		return CMEM_GET_R(PlayerManager,self.PlayerMgrGID);
	end	
end


--场景机器人相关的东西
function Room:LoadRobot( ... )
	-- body
end
function Room:TickRobot( ... )
	-- body
end
function Room:Destroy( ... )
	-- body
end




return Room;