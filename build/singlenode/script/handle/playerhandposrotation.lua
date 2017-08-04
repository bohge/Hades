local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerHandPositionRotation = {};
PlayerHandPositionRotation.MsgID = MsgID["MIN_MSG_PLAYER_VIVE_DEVICE_INFO"];

function PlayerHandPositionRotation:Initialize()
end


function PlayerHandPositionRotation:OnRequest()  
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerViveDeviceInfo, buffer );
  
  local playerinforesult = 
  {
    playerid = gid,
    lefthandpos = inputinfo.lefthandpos,
    lefthandrotation = inputinfo.lefthandrotation,
    righthandpos = inputinfo.righthandpos,
    righthandrotation = inputinfo.righthandrotation,
    headpos = inputinfo.headpos,
    headrotation = inputinfo.headrotation,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerViveDeviceInfoResult,
    MsgID["MIN_MSG_PLAYER_VIVE_DEVICE_INFO_RESULT"],
    buffer, playerinforesult);  
  g_Server:Broadcast(gid, buffer);
end

return PlayerHandPositionRotation;