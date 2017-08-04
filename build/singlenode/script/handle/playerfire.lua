local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerFire = {};
PlayerFire.MsgID = MsgID["MID_MSG_PLAYER_FIRE"];

function PlayerFire:Initialize()
end


function PlayerFire:OnRequest()
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerFire, buffer );
  
  local playerinforesult = 
  {
    playerid = gid,
    weapontype = inputinfo.weapontype,
    islefthand = inputinfo.islefthand,
    hitplayerid = inputinfo.hitplayerid,
    damage = inputinfo.damage,
  }
  --if damage  then
   -- print("playerid " .. tostring(playerinforesult.playerid)
   --   .. " hitplayerid " .. tostring(playerinforesult.hitplayerid));
  --end
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerFireResult,
    MsgID["MID_MSG_PLAYER_FIRE_RESULT"],
    buffer, playerinforesult);
  g_Server:Broadcast(gid, buffer);
end

return PlayerFire;