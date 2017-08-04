nativelibrary("nativesystem");
local MsgID = require "Messageid"
local uuid = require "uuid"
local ffi = require "ffi"

require "utility"


local gamepool = {}
gamepool.infos = nil;--保存全部已经连接服务器信息
gamepool.gidmapping = nil;--保存全部已经连接玩家信息GID映射
gamepool.waitinglist = nil;--等候客户端返回的携程

function gamepool:Initialize(server)
  self.server = server;
  self.infos = {};
  self.gidmapping = {};
  self.waitinglist = {};
end

function gamepool:insert(gid, gameid, gamename, channelid, channelname, serverid, servername)
  local gametable = self.infos[gameid];
  if nil == gametable then
    gametable = {}
    gametable.id = gameid;
    gametable.name = gamename;
    gametable.channle = {};
    self.infos[gameid] = gametable;
  end
  
  local channeltable = gametable.channle[channelid];
  if nil == channeltable then
    channeltable = {}
    channeltable.id = channelid;
    channeltable.name = channelname;
    channeltable.server = {};
    gametable.channle[channelid] = channeltable;
  end
  
  local server = channeltable.server[serverid];
  if nil == server then
    server = {}
    server.gid = gid;
    server.id = serverid;
    server.name = servername;
    channeltable.server[serverid] = server;
  end
  
  local sgid = tostring(gid);
  self.gidmapping[sgid] =
    {
      gameid = gameid,
      channelid = channelid,
      serverid = serverid,
    }
  LOG("insert gid " ..sgid.. " in game " .. gamename .. " channel " .. channelname .. " server " .. servername);
end

function gamepool:delete(gid)
  local sgid = tostring(gid);
  local pinfo = self.gidmapping[sgid];
  if nil == pinfo then
    WARNING("play delete befrom sending connect message");
  else
    local gametable = self.infos[pinfo.gameid];
    if nil ~= gametable then
      local channeltable = gametable.channle[pinfo.channelid];
      if nil ~= channeltable then
        local server = channeltable.server[pinfo.serverid];
        if nil ~= server then
          self.gidmapping[sgid] = nil;
          channeltable.server[pinfo.serverid] = nil;
          LOG(string.format("delete server in game %s channle %s name %s",
              gametable.name, channeltable.name, server.name));
        end          
      end
    end
  end
end

function gamepool:_sendMessage(ameid, channelid, serverid, playerid, requestmsg)
  local sendcount = 0;
  local gametable = self.infos[gameid];
  if nil == gametable then
    WARNING("Unkonw request with gameid "..gameid);
  else
    local channel = gametable.channle[channelid];
    if nil == channel then
      for _, channel in ipairs(gametable.channle) do
        for _, server in ipairs(channel.server) do
          local tosend = requestmsg:Duplicate();
          self.server:Send(server.gid, tosend);
          sendcount = sendcount + 1;
        end        
      end      
    else
      local server = channel.server[serverid];
      if nil == server then
        for _, server in ipairs(channel.server) do
          local tosend = requestmsg:Duplicate();
          self.server:Send(server.gid, tosend);
          sendcount = sendcount + 1;
        end 
      else
        self.server:Send(server.gid, requestmsg);
        sendcount = sendcount + 1;
      end
    end
  end
  return sendcount;
end

function gamepool:getPlayerInfo(gameid, channelid, serverid, playerid)
  local uuidhandle = uuid();
  local request =
  {
    playerid = playerid,
    uuid = uuidhandle,
  }
  local requestmsg = Utility_TableEncoder(MsgID.MID_REQUEST_PLAYER_INFO, request);
  local sendcount = self:_sendMessage(gameid, channelid, serverid, playerid, requestmsg);
  if 0 ~= sendcount then
    local target = self.waitinglist[uuidhandle];
    if nil ~= target then
      error("uuid is duplicate");
    else
      target = {};
      target.resumer = Context:GetServiceContext();
      target.begin = os.clock();
      target.uuid = uuidhandle;
      target.sendcount = sendcount;
      target.revicecount = 0;
      self.waitinglist[uuidhandle] = target;
      local res = target.resumer:Pasue();
      return res;
    end
  end
  return nil
end

function gamepool:OnReceviePlayerInfo(uuid, json)
  local handles = self.waitinglist[uuid];
  if handles then
    handles.revicecount = handles.revicecount + 1;
    if nil ~= json or handles.revicecount == target.sendcount then
      self.waitinglist[uuid] = nil;
      handles.resumer:Resume(json);
    end
  end
end

function gamepool:OnClock()
  local nowtime = os.clock();
  for uuid, handles in pairs(self.waitinglist) do    
    if nowtime - handles.begin > 60 then
      LOG("request overtime "..uuid);
      handles.resumer:Resume();
      self.waitinglist[uuid] = nil;
    end    
  end  
end

return gamepool;