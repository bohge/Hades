
local PlayerInfo = {};


--PlayerInfo.Position_x = 0;
--PlayerInfo.Position_y = 0;
--PlayerInfo.Position_z = 0;
--PlayerInfo.Direction = 0;
--PlayerInfo.Camp = 0;--阵营
--PlayerInfo.Weponids = {};--武器列表


function PlayerInfo:new(o)
  o = o or {};
  setmetatable(o, self);
  self.__index = self;
  return o;
end

function PlayerInfo:SetPostion(x, y, z)
  self.Position_x = x;
  self.Position_y = y;
  self.Position_z = z;
end

function PlayerInfo:GetPostion(x, y, z)
  return self.Position_x,
    self.Position_y,
    self.Position_z;
end

return PlayerInfo;