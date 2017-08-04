local m1 = {}

function m1:new()
  o = {}
  setmetatable(o, self);
  self.__index = self;
  return o;
end


return m1;