

local ffi = require "ffi";
local GlobalPlayerList = {};

GlobalPlayerList._HandleSelf = nil;--脚本自己的handle
GlobalPlayerList.Count = 0;

function GlobalPlayerList:Initialize()
  self._HandleSelf = g_Machine:RegisteInstance(GlobalPlayerList, "GlobalPlayerList");
end


function GlobalPlayerList:NewPlayer(gid, device)
  local player = {};
  player.GID = gid;
  player.Device = device;
  local index = NativeUtility:GetIndexFromID(gid);
  self[index] = player;
  self.Count = self.Count + 1;
end

function GlobalPlayerList:RemovePlayer(gid)
  local index = NativeUtility:GetIndexFromID(gid);
  local player = self[index];
  if player and player.GID == gid then    
    self[index] = nil;
    self.Count = self.Count - 1;    
    return true;
  end
  return false;
end
function GlobalPlayerList:GetGlobalPlayerCnt( ... )
  return self.Count;
end


return GlobalPlayerList;