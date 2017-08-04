nativelibrary("nativesystem");
local MsgID = require "Messageid"
local ffi = require "ffi"
require "utility"


local palyerpool = {}
palyerpool.infos = nil;--保存全部已经连接玩家信息
palyerpool.gidmapping = nil;--保存全部已经连接玩家信息GID映射
palyerpool.waitinglist = nil;--等候客户端返回的携程

function palyerpool:Initialize(server)
  self.server = server;
  self.infos = {};
  self.gidmapping = {};
  self.waitinglist = {};
end

function palyerpool:insert(gameid, playerid, gid)
  local gametable = self.infos[gameid];
  if nil == gametable then
    gametable = {}
    self.infos[gameid] = gametable;
  end
  gametable[playerid] = gid;
  
  local sgid = tostring(gid);
  self.gidmapping[sgid] =
    {
      gameid = gameid,
      playerid = playerid,
    }
  LOG("insert gid " ..sgid.. " in gameid " .. gameid .. " player " .. playerid);
end

function palyerpool:delete(gid)
  local sgid = tostring(gid);
  local pinfo = self.gidmapping[sgid];
  if nil == pinfo then
    WARNING("play delete befrom sending connect message");
  else    
    self.gidmapping[sgid] = nil;
    local gametable = self.infos[pinfo.gameid];
    if nil ~= gametable then
      LOG("delete gid " ..sgid.. " in gameid " .. pinfo.gameid .. " player " .. pinfo.playerid);
      local gid = gametable[pinfo.playerid];
      gametable[pinfo.playerid] = nil;
    end
  end
end

function palyerpool:isPlayerOnline(gameid, playerid)
  local gametable = self.infos[gameid];
  if nil ~= gametable then
    if nil ~= gametable[playerid] then 
      return true;
    end    
  end
  return false;
end

function palyerpool:contactPlayer(gameid, playerid, json)
  local gametable = self.infos[gameid];
  if nil ~= gametable then
    local gid = gametable[playerid]
    if nil ~= gid then
      local msg = IMessage();
      local cbuff = msg:PrepareMessage(MsgID.MID_CONTACT, #json);
      ffi.copy(cbuff, json, #json);
      self.server:SendMessage(gid, msg);
    end
  end  
end

function palyerpool:getPlayerInfo(gameid, playerid)
  local gametable = self.infos[gameid];
  if nil == gametable then
    WARNING("Unkonw player with gameid "..gameid);
  else    
    local gid = gametable[playerid]
    if nil == gid then
      WARNING("Unkonw player with playerid "..playerid);
    else      
      resumer = Context:GetServiceContext();
      local target = self.waitinglist[tostring(gid)];--保证多人同时访问一个玩家安全
      if nil == target then
        target = {};
        target.resumers = {}
        target.begin = os.clock();
        self.waitinglist[tostring(gid)] = target;
        local msg = IMessage();
        msg:PrepareMessage(MsgID.MID_REQUEST_INFO, 0);
        self.server:SendMessage(gid, msg);
      end
      table.insert(target.resumers, resumer);
      local res = resumer:Pasue();
      return res;
    end
  end
  return nil;
end

function palyerpool:OnReceviePlayerInfo(gid, json)
  local sgid = tostring(gid);
  local handles = self.waitinglist[sgid];
  if handles then
    self.waitinglist[sgid] = nil;
    for _, resumer in ipairs(handles.resumers) do
      resumer:Resume(json);
    end    
  else
    ERROR("Unkown info from gid "..tostring(gid));
  end
end

function palyerpool:OnClock()
  local nowtime = os.clock();
  for sgid, handles in pairs(self.waitinglist) do    
    if nowtime - handles.begin > 60 then
      LOG("request overtime "..sgid);
      for _, resumer in ipairs(handles.resumers) do
        resumer:Resume();
      end
      self.waitinglist[sgid] = nil;
    end    
  end  
end

return palyerpool;