local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local GlobalDataList = require "globaldatalist";
local GlobalPlayerList = require "globalplayerlist";


local LoginHandle = {};
LoginHandle.MsgID = MsgID["MID_LOGIN"];

function LoginHandle:Initialize()
end

function LoginHandle:OnRequest(room)
  print("LoginHandle:OnRequest ")
  
  local isloginsucess = true;--userinfo.username and userinfo.password;
  local timevalue = ITimerSystem:GetTimevalue();
  local loginres = 
  {
      success = isloginsucess,
      playerid = gid,
      queueid = 0,
      time = ITimerSystem:GetTimevalue()
    }
  Utility_CapnpEncoder(
      msg_capn.MsgLoginResult,
      MsgID["MID_LOGIN"],
      loginres);
    g_Server:SendMessage(g_ClientID, g_Buffer);

   

 --[[ 
  g_Request:ReadwriteAccessor(
    GlobalDataList.PlayerListAccessor.GID,
    GlobalPlayerList._HandleSelf);
  
 
  print("88888888  ")
  if g_Request:GetAccessors() then
    print("88888888 111111111 ")
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
      loginres);
    g_Server:SendMessage(g_ClientID, g_Buffer);
    end
    ]] --
    --[[
    local test = {};
    buffer, strbuf = Utility.GetCapnpStalyBuffer(); 
    msg_login.MsgLoginResult.parse(strbuf, test);
    msg_login.MsgLoginResult.parse(strbuf, test);
    ]]--
  
end

return LoginHandle;