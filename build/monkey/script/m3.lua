local m3 = {}

function m3:new()
  o = {}
  setmetatable(o, self);
  self.__index = self;
  return o;
end


return m3;