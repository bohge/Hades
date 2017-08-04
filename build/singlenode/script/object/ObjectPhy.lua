--ObjectPhy
local Object = require "object.Object"
local ObjectPhy = {
};
setmetatable(ObjectPhy, Object)

ObjectPhy.__index = ObjectPhy

function ObjectPhy:new(o)
	o = Object:new(o);			
	setmetatable(o,ObjectPhy);	
	o.Name = "ObjectPhy";
	return o;	
end
function ObjectPhy:Cast(o)	
	o = Object:Cast(o);
	setmetatable(o,ObjectPhy);			
	return o;	
end

return ObjectPhy;