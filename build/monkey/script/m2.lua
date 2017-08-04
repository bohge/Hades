local m2 = {}

function m2:new()
  o = {}
  setmetatable(o, self);
  self.__index = self;
  return o;
end


return m2;