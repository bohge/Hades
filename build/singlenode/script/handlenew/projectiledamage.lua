local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local ProjectileDamage = {};
ProjectileDamage.MsgID = MsgID.MID_MSG_PLAYER_PROJECTILE_DAMAGE;

function ProjectileDamage:Initialize()
end


function ProjectileDamage:OnRequest(room)  
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerProjectileDamage, buffer );

  local Player = room:GetPlayerByGID( gid, request);     
  Player:ProjectileDamage();
 


  local outputinfo = 
  {
    weapontype = inputinfo.weapontype,
    damageplayers = inputinfo.damageplayers
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerProjectileDamageResult,
    MsgID["MID_MSG_PLAYER_PROJECTILE_DAMAGE_RESULT"],
    outputinfo);
   room:BroadcastMsgToOhterPlayers(gid,buffer);
end

return ProjectileDamage;