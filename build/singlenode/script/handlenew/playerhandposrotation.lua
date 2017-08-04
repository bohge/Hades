local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerHandPositionRotation = {};
PlayerHandPositionRotation.MsgID = MsgID["MIN_MSG_PLAYER_VIVE_DEVICE_INFO"];

function PlayerHandPositionRotation:Initialize()
end


function PlayerHandPositionRotation:OnRequest(room)  
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerViveDeviceInfo, buffer );
  local lefthandpos = inputinfo.lefthandpos;
  local lefthandrotation = inputinfo.lefthandrotation;
  local righthandpos = inputinfo.righthandpos;
  local righthandrotation = inputinfo.righthandrotation;
  local headpos = inputinfo.headpos;
  local headrotation = inputinfo.headrotation;
  local Player = room:GetPlayerByGID( gid, request);    



  Player:HandPositionRotation(lefthandpos,lefthandrotation,righthandpos,righthandrotation,headpos,headrotation);


  local PlayerHandPositionRotationResult = 
  {
    playerid = gid,
    lefthandpos = lefthandpos,
    lefthandrotation = lefthandrotation,
    righthandpos = righthandpos,
    righthandrotation = righthandrotation,
    headpos = headpos,
    headrotation = headrotation,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerViveDeviceInfoResult,
    MsgID["MIN_MSG_PLAYER_VIVE_DEVICE_INFO_RESULT"],
    PlayerHandPositionRotationResult);  
  room:BroadcastMsgToOhterPlayers(gid,buffer);
  
end

return PlayerHandPositionRotation;