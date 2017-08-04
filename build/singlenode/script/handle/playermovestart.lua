local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerMoveStrat = {};
PlayerMoveStrat.MsgID = MsgID["MID_PLAYER_MOVE_START"];

function PlayerMoveStrat:Initialize()
end


function PlayerMoveStrat:OnRequest()  
  local playerinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerMoveStart, buffer );
  
  local playerinforesult = 
  {
    playerid = gid,
    dir = playerinfo.dir,
    posx = playerinfo.playerinfo,
    posy = playerinfo.posy,
    posz = playerinfo.posz,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerMoveStartResult,
    MsgID["MID_PLAYER_MOVE_START_RESULT"],
    buffer, playerinforesult);  
  g_Server:Broadcast(gid, buffer);
end

return PlayerMoveStrat;