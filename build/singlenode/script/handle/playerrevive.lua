local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerRevive = {};
PlayerRevive.MsgID = MsgID["MID_MSG_PLAYER_REVIVE"];

function PlayerRevive:Initialize()
end


function PlayerRevive:OnRequest()
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerRevive, buffer );
  
  local reviveresult = 
  {
    playerid = gid,
    health = inputinfo.health
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerReviveResult,
    MsgID["MID_MSG_PLAYER_REVIVE_RESULT"],
    buffer, reviveresult);  
  g_Server:Broadcast(gid, buffer);
end

return PlayerRevive;