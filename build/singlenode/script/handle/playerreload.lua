local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerReload = {};
PlayerReload.MsgID = MsgID["MID_MSG_PLAYER_RELOAD"];

function PlayerReload:Initialize()
end


function PlayerReload:OnRequest()  
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerReload, buffer );
  
  local playerinforesult = 
  {
    playerid = gid,
    islefthand = inputinfo.islefthand,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerReloadResult,
    MsgID["MID_MSG_PLAYER_RELOAD_RESULT"],
    buffer, playerinforesult);
  g_Server:Broadcast(gid, buffer);
end

return PlayerReload;