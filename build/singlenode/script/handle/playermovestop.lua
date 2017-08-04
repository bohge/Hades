local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerMoveStop = {};
PlayerMoveStop.MsgID = MsgID["MID_PLAYER_MOVE_STOP"];

function PlayerMoveStop:Initialize()
end


function PlayerMoveStop:OnRequest()  
  local playerinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerMoveStop, buffer );
  
  local playerinforesult = 
  {
    playerid = gid,
    posx = playerinfo.playerinfo,
    posy = playerinfo.posy,
    posz = playerinfo.posz,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerMoveStopResult,
    MsgID["MID_PLAYER_MOVE_STOP_RESULT"],
    buffer, playerinforesult);  
  g_Server:Broadcast(gid, buffer);
end

return PlayerMoveStop;