local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local GlobalDataList = require "globaldatalist";
local GlobalPlayerList = require "globalplayerlist";



local LogoutHandle = {};
LogoutHandle.MsgID = MsgID["MID_LOGOUT"];

function LogoutHandle:Initialize()
end


function LogoutHandle:OnRequest()
  --[[
  local buffer = buffer:Data();
  local req = 
  {
    clientid = 0,
  };
  local size = msg_logout.MsgLogoutRequest.calc_size(req);
  msg_logout.MsgLogoutRequest.serialize(req, buffer, size);
  buffer:SetLength(size);
  ]]--
  g_Request:ReadwriteAccessor(
    GlobalDataList.PlayerListAccessor.GID,
    GlobalPlayerList._HandleSelf);
  if g_Request:GetAccessors() then
    local userinfo = Utility_CapnpDecoder( msg_capn.MsgLogoutRequest, buffer );
    
    if gid == userinfo.clientid then
      logoutres = {success = true};
      self:PlayerLeaveRequest(gid, issucces);
      Utility_CapnpEncoder(
        msg_capn.MsgLogoutResult,
        MsgID["MID_LOGOUT"],
        buffer, logoutres);
      g_Server:SendMessage(gid, buffer);      
    end
  end
end

function LogoutHandle:OnDisconnect()
  g_Request:ReadwriteAccessor(
    GlobalDataList.PlayerListAccessor.GID,
    GlobalPlayerList._HandleSelf);
  if g_Request:GetAccessors() then
    local isremove = GlobalPlayerList:RemovePlayer(gid);
    if isremove then
      local messagebuffer = nil;
      if nil ~= buffer then
        messagebuffer = buffer;
      else
        messagebuffer = Message();
      end      
      local playerleave = {};
      playerleave.playerid = gid;  
      Utility_CapnpEncoder(
        msg_capn.MsgBroadcastPlayerLeave,
        MsgID["MID_MSG_LEAVE_SCENE"],
        messagebuffer, playerleave);
      g_Server:Broadcast(gid, messagebuffer);
    end
  end
end

return LogoutHandle;