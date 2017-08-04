local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local GlobalDataList = require "globaldatalist";
local GlobalPlayerList = require "globalplayerlist";


local LeaveSceneHandle = {};
LeaveSceneHandle.MsgID = MsgID["MID_MSG_LEAVE_SCENE"];

function LeaveSceneHandle:Initialize()
end


function LeaveSceneHandle:OnRequest()
  --[[
  local buffer = Buffer:Data();
  local req = {};
  req.clientid = gid;
  local size = msg_leave_scene.MsgLeaveScene.calc_size(req);
  msg_leave_scene.MsgLeaveScene.serialize(req, buffer, size);
  buffer:SetLength(size);
  ]]--
  g_Request:ReadwriteAccessor(
    GlobalDataList.PlayerListAccessor.GID,
    GlobalPlayerList._HandleSelf);

  if g_Request:GetAccessors() then
    local playerinfo = Utility_CapnpDecoder( msg_capn.MsgLeaveScene, buffer );
    
    if gid == playerinfo.clientid then
      local isremoved = GlobalPlayerList:RemovePlayer(gid);    
      if isremoved then
        --向其他玩家广播消息
        local leavescene = 
        {
          playerid = gid,
        }
        Utility_CapnpEncoder(
          msg_capn.MsgBroadcastPlayerLeave,
          MsgID["MID_MSG_BROADCAST_PLAYER_LEAVE"],
          buffer, leavescene);
        g_Server:Broadcast(gid, buffer);
      end    
    end
  end
end

return LeaveSceneHandle;