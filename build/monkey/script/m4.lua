local m4 = {}

function m4:new()
  o = {}
  setmetatable(o, self);
  self.__index = self;
  return o;
end


return m4;