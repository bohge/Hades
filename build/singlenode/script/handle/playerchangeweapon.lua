local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local PLAYER_INSTANCE_STACK_INDEX = 0;


local PlayerChangeWeapon = {};
PlayerChangeWeapon.MsgID = MsgID["MID_MSG_PLAYER_CHANGE_WEAPON"];

function PlayerChangeWeapon:Initialize()
end


function PlayerChangeWeapon:OnRequest()
  local instancehandler = g_Machine:GetInstance();
  local csgid = g_Server:GetClientSpecific(gid);
  g_Request:ReadwriteAccessor(csgid, instancehandler);--读写数据访问器
  if g_Request:GetAccessors() then    
    local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerChangeWeapon, buffer );    
    local playerinfo = instancehandler:GetScriptInstance();--得到请求的原始数据
    
    playerinfo.CurWeaponid = inputinfo.weaponid;
    
    local playerinforesult = 
    {
      playerid = gid,
      weaponid = inputinfo.weaponid,
    }
    Utility_CapnpEncoder(
      msg_capn.MsgPlayerChangeWeaponResult,
      MsgID["MID_MSG_PLAYER_CHANGE_WEAPON_RESULT"],
      buffer, playerinforesult);
    g_Server:Broadcast(gid, buffer);
  end
end


return PlayerChangeWeapon;