--ObjectMonster 

local ObjectCharacter = require "object.ObjectCharacter"


local ObjectMonster = {
	EnergyCount,
};

setmetatable(ObjectMonster, ObjectCharacter)
ObjectMonster.__index = ObjectMonster

function ObjectMonster:new(o)
	o = ObjectCharacter:new(o);			
	setmetatable(o,ObjectMonster);	
	o.Name = "ObjectMonster";	
	return o;	
end
function ObjectMonster:Cast(o)	
	o = ObjectCharacter:Cast(o);
	setmetatable(o,ObjectMonster);			
	return o;	
end
function ObjectMonster:Init(  )
	self.Name =  "ObjectMonster";
	self.EnergyCount = 100;
end
function ObjectMonster:BeHit(weapontype,islefthand,hitplayerid,damage)
	
	--获取整个玩家。

	--self:HP = self:HP - damage;		


	--if(self:HP < 0) then
	--	--广播死亡
	--end
end


return ObjectMonster;