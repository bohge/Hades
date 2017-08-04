local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local msg_capn = require "protocol/msg_capn_capnp"
local Dispatcher = require "dispatchhandle";
local PLAYER_INSTANCE_STACK_INDEX = 0;

local BroadcastPlayerInfo = {};

function BroadcastPlayerInfo:Initialize()
  Dispatcher:RegisteHandle( GenID["GID_BROADCAST_POSITION"], BroadcastPlayerInfo, BroadcastPlayerInfo.OnGenericJob );
end

function BroadcastPlayerInfo:OnGenericJob( datas)  
  local csgid = g_Server:GetClientSpecific(datas.GID);
  local instancehandler = g_Machine:GetInstance();
  g_Request:ReadonlyAccessor(csgid, instancehandler);
  if g_Request:GetAccessors() then
    local playerinfo = instancehandler:GetScriptInstance();--得到请求的原始数据
    local tosendinfo =
    {
      playerid = datas.GID,
      devicetype = datas.Device,
      dir = 0;
      posx = playerinfo.Position_x,
      posy = playerinfo.Position_y,
      posz = playerinfo.Position_z,
      camp = playerinfo.Camp,
      weaponids = playerinfo.Weaponids,
      curweaponid = playerinfo.CurWeaponid
    }
    --print( "entry scene GID " .. tostring(playerinfo.GID) .." UUID " .. tostring(playerinfo.UUID) .. " devicetype " .. tostring(playerinfo.Device) );
    Utility_CapnpEncoder(
      msg_capn.MsgBroadcastPlayerInfo,
      MsgID["MID_MSG_BROADCAST_PLAYER_INFO"],
      tosendinfo);    
    g_Server:SendMessage(g_ClientID, g_Buffer);
  end
end

return BroadcastPlayerInfo;