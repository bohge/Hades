local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local PLAYER_INSTANCE_STACK_INDEX = 0;


local PlayerChangeWeapon = {};
PlayerChangeWeapon.MsgID = MsgID["MID_MSG_PLAYER_CHANGE_WEAPON"];

function PlayerChangeWeapon:Initialize()
end


function PlayerChangeWeapon:OnRequest(room)

  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerChangeWeapon, buffer );    
  local NewWeaponId = inputinfo.weaponid;

  local instancehandler = g_Machine:GetInstance();
  local csgid = g_Server:GetClientSpecific(gid);
  local Character = room:GetPlayer();

  Character:ChangeWeapon(NewWeaponId);


  local PlayerChangeWeaponResult = 
  {
    playerid = gid,
    weaponid = NewWeaponId,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerChangeWeaponResult,
    MsgID["MID_MSG_PLAYER_CHANGE_WEAPON_RESULT"],
    PlayerChangeWeaponResult);
  room:BroadcastMsgToOhterPlayers(buffer);
end


return PlayerChangeWeapon;