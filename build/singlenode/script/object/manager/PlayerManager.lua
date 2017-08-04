--PlayerManager

local ObjectManager = require "object.manager.ObjectManager"
local ObjectPlayer = require "object.ObjectPlayer"


local PlayerManager={
	ObjMgrGID,
	PlayerMap,
	ClientIDArray,
	RoomGID,
};

PlayerManager.__index = PlayerManager;
function PlayerManager:new(o)
	o = o or {};
	setmetatable(o,PlayerManager);		
	return o;	
end

function PlayerManager:Cast(o)	
	self.__index = self;	
	setmetatable(o,self);
	return o;	
end

function PlayerManager:Init(objmgrgid,roomgid )
  	self.ObjMgrGID = objmgrgid;
  	self.RoomGID = roomgid;
	self.PlayerMap = {};
	self.ClientIDArray = {};
end

function PlayerManager:AddPlayer()	
	
	local Player = CMEM_CREATE(ObjectPlayer) 
	local PlayerMemGID = Player._GID;		
	Player:SetClientID(g_ClientID);
	
	g_Server:SetClientSpecific(g_ClientID, PlayerMemGID);	

	--添加到ObjectManger中设置ServerID	
	local ObjMgr =CMEM_GET_RW(ObjectManager,self.ObjMgrGID);
	ObjMgr:AddObject(Player);
	CMEM_FREE(ObjMgr);

	self.PlayerMap[Player:GetServerID()] = PlayerMemGID;			
	self.ClientIDArray[(#self.ClientIDArray + 1)] = g_ClientID;

	print("-----------------------PlayerManager:GetPlayerCount    " .. self:GetPlayerCount());

    return Player;

end
function PlayerManager:GetClientIDArray()	
	return self.ClientIDArray;
end
function PlayerManager:GetPlayerCount(  )	
	return Utility_GetMapCount(self.ClientIDArray);
end

function PlayerManager:Tick(deltatime)
	if(self.PlayerMap == nil) then
		return ;
	end
	for k,v in pairs(self.PlayerMap) do
		local PlayerMemGID = v;		
		local Player = CMEM_GET_R(ObjectPlayer,PlayerMemGID);
		Player:Tick(deltatime)
	end
end

function PlayerManager:Release()
  
end
function PlayerManager:GetRoom()	
	local Room = require "pvp.Room";		
	return CMEM_GET_R(Room,self.RoomGID);
end


return PlayerManager;
