local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local GlobalDataList = require "globaldatalist";
local GlobalPlayerList = require "globalplayerlist";


local LoginHandle = {};
LoginHandle.MsgID = MsgID["MID_LOGIN"];

function LoginHandle:Initialize()
end

function LoginHandle:OnRequest()
  --[[
  local buffer = buffer:Data();
  local req = {};
  req.username = "bohge";
  req.password = "password";
  local size = msg_login.MsgLoginRequest.calc_size(req);
  msg_login.MsgLoginRequest.serialize(req, buffer, size);
  buffer:SetLength(size);
  ]]--
  g_Request:ReadwriteAccessor(
    GlobalDataList.PlayerListAccessor.GID,
    GlobalPlayerList._HandleSelf);
  
  if g_Request:GetAccessors() then
    local userinfo = Utility_CapnpDecoder( msg_capn.MsgLoginRequest, buffer );
    
    --检测userinfo
    local isloginsucess = userinfo.username and userinfo.password;
    local timevalue = ITimerSystem:GetTimevalue();
    local loginres = 
    {
      success = isloginsucess,
      playerid = gid,
      queueid = 0,
      time = ITimerSystem:GetTimevalue()
    }
    GlobalPlayerList:NewPlayer(gid, userinfo.devicetype);--创建新玩家
    local count = GlobalPlayerList:GetGlobalPlayerCnt();    
    Utility_CapnpEncoder(
      msg_capn.MsgLoginResult,
      MsgID["MID_LOGIN"],
      buffer, loginres);
    g_Server:SendMessage(gid, buffer);
    --[[
    local test = {};
    buffer, strbuf = Utility.GetCapnpStalyBuffer(); 
    msg_login.MsgLoginResult.parse(strbuf, test);
    msg_login.MsgLoginResult.parse(strbuf, test);
    ]]--
  end
end

return LoginHandle;