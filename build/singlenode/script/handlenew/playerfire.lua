local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerFire = {};
PlayerFire.MsgID = MsgID["MID_MSG_PLAYER_FIRE"];

function PlayerFire:Initialize()
end


function PlayerFire:OnRequest(room)  
  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerFire, buffer );
  local weapontype = inputinfo.weapontype;
  local islefthand = inputinfo.islefthand;
  local hitplayerid = inputinfo.hitplayerid;
  local damage = inputinfo.damage;

  
  local Player = room:GetPlayerByGID( gid, request);    
  Player:Fire(weapontype,islefthand,hitplayerid,damage);

  if(hitplayerid ~= -1) then
    local PlayerBeHit = room:GetPlayerByGID( hitplayerid, request);  
    PlayerBeHit:BeHit(weapontype,islefthand,gid,damage);
  end
  
  local PlayerFireResult = 
  {
    playerid = gid,
    weapontype = weapontype,
    islefthand = islefthand,
    hitplayerid = hitplayerid,
    damage = damage,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerFireResult,
    MsgID["MID_MSG_PLAYER_FIRE_RESULT"],
    PlayerFireResult);
  room:BroadcastMsgToOhterPlayers(gid,buffer);
  
end

return PlayerFire;