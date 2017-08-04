require "utility"

local dataset = {}
dataset.infos = nil;--保存全部潜在vip玩家信息

function dataset:SetDatabase(db_rpc)
  self.infos = db_rpc:GetAllPotential();
  LOG("Loaded info from database");
end

function dataset:insert(gameid, playerid)
  local gametable = self.infos[gameid];
  if nil == gametable then
    gametable = {}
    self.infos[gameid] = gametable;
  end
  gametable[playerid] = true;
end

function dataset:delete(gameid, playerid)
  local gametable = self.infos[gameid];
  if nil ~= gametable then
    gametable[playerid] = nil;
  end  
end

function dataset:isPotential(gameid, playerid)
  local gametable = self.infos[gameid];
  if nil ~= gametable then
    if true == gametable[playerid] then 
      return true;
    end    
  end
  return false
end

return dataset;