--ObjectNpc 
local ObjectCharacter = require "ObjectCharacter"
local ObjectNpc = {
};
NPC_TYPE={
	CT_INVALID	= -1,
    NPC_TYPE_MONSTER = 0 , -- 怪物
    NPC_TYPE_PET = 1, --宠物
    NPC_TYPE_MOUNT = 2,-- 坐骑
}


 function ObjectNpc:new()
	o = ObjectCharacter:new();		
	o.Name = "ObjectNpc";	
	return o;	
end

function ObjectNpc:Cast(o)	
	o = ObjectCharacter:Cast(o);
	setmetatable(o,ObjectNpc);			
	return o;	
end

function ObjectNpc:Initial( ... )
	-- body
end
function ObjectNpc:GetCharacterType( ... )
	-- body
end
function ObjectNpc:GetNpcType( ... )
	-- body
end

local npc1 = ObjectNpc:new();
npc1:Test();
return ObjectNpc;
