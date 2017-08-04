local m5 = {}

function m5:new()
  o = {}
  setmetatable(o, self);
  self.__index = self;
  return o;
end


return m5;