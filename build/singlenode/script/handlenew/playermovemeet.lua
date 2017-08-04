local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;

local NEW_PLAYER_INSTANCE_STACK_INDEX = 0;


local PlayerMoveMeet = {};
PlayerMoveMeet.MsgID = MsgID["MID_PLAYER_MOVE_MEET_OBSTACLE"];

function PlayerMoveMeet:Initialize()

end


function PlayerMoveMeet:OnRequest(room)  

  --[[
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerMoveMeetObstacle, buffer );
  local posx = inputinfo.posx;
  local posy = inputinfo.posy;
  local posz = inputinfo.posz;

  local Player = room:GetPlayerByGID( gid, request);    
  Player:MoveMeet(posx,posy,posz);


  local PlayerMoveMeetResult = 
    {
      playerid = gid,
      posx = posx,
      posy = posy,
      posz = posz,
    }
    Utility_CapnpEncoder(
      msg_capn.MsgPlayerMoveMeetObstacleResult,
      MsgID["MID_PLAYER_MOVE_MEET_OBSTACLE_RESULT"],
      PlayerMoveMeetResult);  
  room:BroadcastMsgToOhterPlayers(gid,buffer);
  ]]--
end


return PlayerMoveMeet;