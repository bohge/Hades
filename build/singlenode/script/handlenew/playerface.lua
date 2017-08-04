local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerFace = {};
PlayerFace.MsgID = MsgID["MID_PLAYER_FACE"];

function PlayerFace:Initialize()
end


function PlayerFace:OnRequest(room)  
  
  --[[
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerFace, buffer );

  local yaw = inputinfo.yaw;
  local pitch = inputinfo.pitch;
  local Player = room:GetPlayerByGID( gid, request);    
  Player:SetFaceDir(yaw,pitch);

  local PlayerFaceResult = 
  {
    playerid = gid,
    yaw = yaw,
    pitch = pitch,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerFaceResult,
    MsgID["MID_PLAYER_FACE_RESULT"],
    PlayerFaceResult);
   room:BroadcastMsgToOhterPlayers(gid,buffer);
   ]]--
end

return PlayerFace;