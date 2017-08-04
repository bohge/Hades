local Dispatcher = require "dispatchhandle";

local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local GlobalPlayerList = require "globalplayerlist";
local GlobalDataList = require "globaldatalist";
local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;

--PVP游戏规则
local GameRulers={
	LastTickTime ,
	GameStatus ,
	GameStartLeftTime ,
	MissionLeftTime ,
	MeetMissionTimeCoundown ,
	TimeMonster,
	Room,

}	
GameRulers.__index = GameRulers;

GAMESTATUS={
	GS_INIT = 0 ,
	GS_GAMESTARTCOUNDOWN = 1 ,
	GS_GAMETICK =2,
	GS_GAMEOVER = 3
}



function GameRulers:Initialize()
 
end


function GameRulers:new()
	o = o or {};
	setmetatable(o,GameRulers);		
	return o;
end
function GameRulers:Cast(o)		
	setmetatable(o,GameRulers);	
	return o;	
end
-- GameRule初始化
function GameRulers:Init(roomgid)
	print("GameRulers.Init ");
	self.GameStartLeftTime = 10;
	self.GameStatus = GAMESTATUS.GS_INIT;
	self.GameStartLeftTime = 0 ;
	self.MissionLeftTime = 0;
	self.MeetMissionTimeCoundown = 0 ;	
	self.RoomGID = roomgid;

end

-- GameRulers Tick函数
function GameRulers:Tick(deltatime)  	 	
    if(self.GameStatus == GAMESTATUS.GS_INIT) then
    	self:TestPlayerCount();
    elseif(self.GameStatus == GAMESTATUS.GS_GAMESTARTCOUNDOWN) then
		self:GameStartCountdownTick(deltatime);
	elseif(self.GameStatus == GAMESTATUS.GS_GAMETICK) then
		self:GameTick(deltatime);
	else
		print("Gametick error");
	end

end
function GameRulers:GetGameRulerStatus( )
	return self.GameStatus;
end

function GameRulers:TestPlayerCount()	

	local PlayerCount = self:GetRoom():GetRoomPlayerCount();
	print("TestPlayerCount  " ..PlayerCount);	
	if(PlayerCount >=1) then 		
		self:StartGameStartCountdown();
	end
end

function GameRulers:GetRoom( )
	local Room = require "pvp.Room";
	local R = CMEM_GET_R(Room,self.RoomGID);
	CMEM_FREE(R);
	return R;
end

-- 游戏开始前倒计时
function GameRulers:StartGameStartCountdown(  )	

	self.GameStartLeftTime = 5;--倒计时默认是30s
	self.GameStatus = GAMESTATUS.GS_GAMESTARTCOUNDOWN;	
	local MonsterMgr = self:GetRoom():GetMonsterManager(true);	
	MonsterMgr:AddSceneMonsters();
	CMEM_FREE(MonsterMgr._GID);

end
function GameRulers:GameStartCountdownTick( deltaFtime )
	self.GameStartLeftTime = self.GameStartLeftTime - deltatime;
	if(self.GameStartLeftTime <=0) then
		self:StartGameTick();
	end
	print("GameRulers.GameStartCountdownTick  " .. tostring(deltatime) .. "    " .. tostring(self.GameStartLeftTime))
end




--GameTick
function GameRulers:StartGameTick()
	self.MissionLeftTime = 5*60;
	self.MeetMissionTimeCoundown = 10;
	self.GameStatus = GAMESTATUS.GS_GAMETICK;
	--todochenjiazeng
	--通知所有玩家倒计时结束，玩家开始游戏
	local MsgPvPGameStartInfo = 
    {
      time = MissionLeftTime;
    }
    Utility_CapnpEncoder(
      msg_capn.MsgPvpGameStart,
      MsgID["MID_MSG_PVP_GAME_START"],
      MsgPvPGameStartInfo);    
    self:GetRoom():BroadcastMsgToPlayers(g_Buffer);
    
	
end


function GameRulers:GameTick( deltatime)
	self.MissionLeftTime = self.MissionLeftTime - deltatime;
	

	if(self.MissionLeftTime <=0) then
		self:DefenderWin();
	else 
		
	end	


	--self:TickMonsterManger( Second,gid,request,buffer,datas);
	--self:TickMeetMissionTime( Second,gid,request,buffer,datas);

	--print("GameRulers.GameTick  " .. tostring(self.MissionLeftTime));
end


function GameRulers:TickMonsterManger(deltatime)
	self.TimeMonster = self.TimeMonster - deltatime;
	if(self.TimeMonster>0) then
		return ;
	end
	--self:GetRoom(request):GetMonsterManager(request):AddSceneMonsters(request,buffer);	
end

function GameRulers:TickMeetMissionTime( deltatime)
	self.MeetMissionTimeCoundown = self.MeetMissionTimeCoundown - deltatime;
	if(self.MeetMissionTimeCoundown>0) then
		return ;
	end
	self.MeetMissionTimeCoundown = 10;
	--通知所有玩家校对时间
	local MsgPvPGameTickTimeMeetInfo = 
    {
      time = self.MissionLeftTime;
    }
    Utility_CapnpEncoder(
      msg_capn.MsgPvpGameTickTimeMeet,
      MsgID["MID_MSG_PVP_GAME_TICK_TIME_MEET"],
      MsgPvPGameTickTimeMeetInfo);    
    self:GetRoom():BroadcastMsgToPlayers(g_Buffer);

end
function GameRulers:GameNoPlayer( ... )
	print("GaemNoPlayer-- change to InitStatus");	
	self.GameStatus = GAMESTATUS.GS_INIT;

end
function GameRulers:GameAddTime( ... )
	print("GameRulers:GameAddTime");
	--通知玩家游戏加时
end

-- 游戏结算
function GameRulers:DefenderWin( ... )
	self.GameStatus = GAMESTATUS.GS_GAMEOVER;
	print("DefenderWin");
	--todochenjiazeng
	--通知所有玩家防守方胜利
end
function GameRulers:AttackerWin( ... )
	self.GameStatus = GAMESTATUS.GS_GAMEOVER;
	print("AttackerWin");
	--todochenjiazeng
	--通知所有玩家进攻方胜利
end

function GameRulers:Release( ... )
	print("GameRulers.Release");
end
return GameRulers;
