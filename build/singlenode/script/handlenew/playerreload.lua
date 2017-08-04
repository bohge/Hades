local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerReload = {};
PlayerReload.MsgID = MsgID["MID_MSG_PLAYER_RELOAD"];

function PlayerReload:Initialize()
end


function PlayerReload:OnRequest(room)  
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerReload, buffer );
  local Player = room:GetPlayerByGID( gid, request);     
  Player:WeaponReload();


  local PlayerReloadResult = 
  {
    playerid = gid,
    islefthand = inputinfo.islefthand,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerReloadResult,
    MsgID["MID_MSG_PLAYER_RELOAD_RESULT"],
    PlayerReloadResult);
  room:BroadcastMsgToOhterPlayers(gid,buffer);
end

return PlayerReload;