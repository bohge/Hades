local m6 = {}

function m6:new()
  o = {}
  setmetatable(o, self);
  self.__index = self;
  return o;
end


return m6;