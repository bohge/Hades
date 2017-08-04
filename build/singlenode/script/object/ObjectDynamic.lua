--ObjectDynamic 
local ObjectMap = require "object.ObjectMap"
local ObjectDynamic = {
	GridPos,
	ZoneID,
};

setmetatable(ObjectDynamic, ObjectMap)

ObjectDynamic.__index = ObjectDynamic

function ObjectDynamic:new(o)
	o = ObjectMap:new(o);			
	setmetatable(o,ObjectDynamic);	
	o.Name = "ObjectDynamic";
	return o;	
end

function ObjectDynamic:Cast(o)	
	o = ObjectMap:Cast(o);
	setmetatable(o,ObjectDynamic);			
	return o;	
end
function ObjectDynamic:Tick( DeltaTime )
	print(self.Name  .. ": Tick " )
	-- body
	--AI TICK
end
return ObjectDynamic;