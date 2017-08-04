local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerFace = {};
PlayerFace.MsgID = MsgID["MID_PLAYER_FACE"];

function PlayerFace:Initialize()
end


function PlayerFace:OnRequest()  
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerFace, buffer );
  
  local playerinforesult = 
  {
    playerid = gid,
    yaw = inputinfo.yaw,
    pitch = inputinfo.pitch,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerFaceResult,
    MsgID["MID_PLAYER_FACE_RESULT"],
    buffer, playerinforesult);
  g_Server:Broadcast(gid, buffer);
end

return PlayerFace;