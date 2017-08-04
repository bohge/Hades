local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerMoveStop = {};
PlayerMoveStop.MsgID = MsgID["MID_PLAYER_MOVE_STOP"];

function PlayerMoveStop:Initialize()
end


function PlayerMoveStop:OnRequest(room)  

  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerMoveStop, buffer );  
  local posx = inputinfo.posx;
  local posy = inputinfo.posy;
  local posz = inputinfo.posz;
   local Player = room:GetPlayerByGID( gid, request);    
  Player:EndMove(posx,posy,posz);


  local  PlayerMoveStopResult= 
  {
    playerid = gid,
    posx = posx,
    posy = posy,
    posz = posz,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerMoveStopResult,
    MsgID["MID_PLAYER_MOVE_STOP_RESULT"],
    PlayerMoveStopResult);  
  room:BroadcastMsgToOhterPlayers(gid,buffer);
end

return PlayerMoveStop;