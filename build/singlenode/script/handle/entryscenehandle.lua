
local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local GlobalDataList = require "globaldatalist";
local GlobalPlayerList = require "globalplayerlist";
local Dispatcher = require "dispatchhandle";


local EntryScene = {};
EntryScene.MsgID = MsgID["MID_MSG_ENTRY_SCENE_REQUEST"];
local NEW_PLAYER_INSTANCE_STACK_INDEX = 0;

function EntryScene:Initialize()
end

function EntryScene:OnRequest()
  --[[
  local buffer = buffer:Data();
  local req = {};
  req.playerid = gid;
  req.sceneid = 0;
  local size = msg_entry_scene.MsgEntrySceneRequest.calc_size(req);
  msg_entry_scene.MsgEntrySceneRequest.serialize(req, buffer, size);
  buffer:SetLength(size);
  ]]--
  
  local instancehandler = g_Machine:GetInstance();  
  g_Request:CreateAccessor(instancehandler);--创建数据访问器
  g_Request:ReadonlyAccessor(
    GlobalDataList.PlayerListAccessor.GID,
    GlobalPlayerList._HandleSelf);
  
  if g_Request:GetAccessors() then
    local newplayertable = instancehandler:GetScriptInstance();--得到请求的原始数据
    g_Server:SetClientSpecific(gid, newplayertable._GID);--保存到当前请求的专有数据表
    
    local playerinfo = Utility_CapnpDecoder( msg_capn.MsgEntrySceneRequest, buffer );
  
    --newplayertable = PlayerInfo:new(newplayertable);

    newplayertable.Position_x = 0;
    newplayertable.Position_y = 0;
    newplayertable.Position_z = 0;
    newplayertable.Direction = 0;
    newplayertable.CurWeaponid = playerinfo.curweaponid;
    newplayertable.Camp = playerinfo.camp;
    newplayertable.Weaponids = playerinfo.weaponids;
  
    --递归发送玩家位置信息
    local playerid = {};
    local devicetype = nil;
    for k, v in pairs(GlobalPlayerList) do
      if type(v) == "table" then
        if gid ~= v.GID then
          playerid.GID = v.GID;
          playerid.Device = v.Device;
          Dispatcher:Dispatch(GenID.GID_BROADCAST_POSITION, gid, playerid);
        else
          devicetype = v.Device;
        end
      end
    end

    --广播
    local newplayerinfo =
    {
      playerid = gid,
      devicetype = devicetype,
      dir = 0,
      posx = 0,
      posy = 0,
      posz = 0,
      camp = playerinfo.camp,
      weaponids = playerinfo.weaponids,
      curweaponid = playerinfo.curweaponid
    }
    Utility_CapnpEncoder(
      msg_capn.MsgBroadcastPlayerInfo,
      MsgID["MID_MSG_BROADCAST_PLAYER_INFO"],
      buffer, newplayerinfo);
    g_Server:Broadcast(gid, buffer);
  end
end

return EntryScene;