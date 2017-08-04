--ObjectMap 属于地表层的物体，这类物体受场景管理，需要注册到某个地表网格上	当玩家远离该网格时会被销毁
local ObjectPhy = require "object.ObjectPhy"
local ObjectMap = {
	SceneId
};

setmetatable(ObjectMap, ObjectPhy)

ObjectMap.__index = ObjectMap

function ObjectMap:new(o)
	o = ObjectPhy:new(o);			
	setmetatable(o,ObjectMap);	
	o.Name = "ObjectMap";
	return o;	
end
function ObjectMap:Cast(o)	
	o = ObjectPhy:Cast(o);
	setmetatable(o,ObjectMap);			
	return o;	
end
function ObjectMap:SetScene( ... )
	-- body
end
function ObjectMap:GetScene( ... )
	-- body
end
return ObjectMap;