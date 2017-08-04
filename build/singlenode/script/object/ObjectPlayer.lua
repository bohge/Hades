--ObjectPlayer 

local ObjectCharacter = require "object.ObjectCharacter"


local ObjectPlayer = {
	ClientID,
	EnergyCount,
};
setmetatable(ObjectPlayer, ObjectCharacter)
ObjectPlayer.__index = ObjectPlayer
 function ObjectPlayer:new(o)
	o = ObjectCharacter:new(o);			
	setmetatable(o,ObjectPlayer);
	return o;	
end
function ObjectPlayer:Cast(o)	
	o = ObjectCharacter:Cast(o);	
	setmetatable(o,ObjectPlayer);			
	return o;	
end
function ObjectPlayer:Init()
	self.Name = "ObjectPlayer";	
	self.ClientID = "11111";
	self.EnergyCount = 0;
end

function ObjectPlayer:GetClientID( ... )
	return self.ClientID;
end
function ObjectPlayer:SetClientID( clientid )
	self.ClientID = clientid;
end

function ObjectPlayer:AddEnergy( energy )
	self.EnergyCount = self.EnergyCount + energy;
end


return ObjectPlayer;