--ObjectStatic 注册到固定网格内的物体不会移动的物体，例如石块，树木，建筑等
local ObjectMap = require "object.ObjectMap"
local ObjectStatic = {
	GridPos,
	ZoneID,
};

setmetatable(ObjectStatic, ObjectMap)

ObjectStatic.__index = ObjectStatic

function ObjectStatic:new(o)
	o = ObjectMap:new(o);			
	setmetatable(o,ObjectStatic);	
	o.Name = "ObjectStatic";
	return o;	
end
function ObjectPlayerOther:Cast(o)	
	o = ObjectMap:Cast(o);	
	setmetatable(o,ObjectPlayerOther);			
	return o;	
end

return ObjectStatic;