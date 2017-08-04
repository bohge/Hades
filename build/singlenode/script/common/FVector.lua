FVector= { x , y , z}

function FVector:new(o)
	o = o or {};
	setmetatable(o,self);
	self.__index = self;
	o.x = 0.0 ; 
	o.y = 0.0;
	o.z = 0.0
	return o;
end
function FVector:Set(x,y,z)
	self.x = x;
	self.y = y;
	self.z = z;
end
function FVector:SetX(x)
	self.x = x;
end
function FVector:SetY(y)
	self.y = y;
end
function FVector:SetZ(z)
	self.z = z;
end
function  FVector:GetX()
	return self.x;
end
function  FVector:GetY()
	return self.y;
end
function  FVector:GetZ()
	return self.z;
end
--[[
local l = Location:new();
l:SetPotioin(10,10,10);
print(l.x)
local l2 = Location:new();
l2:SetPotioin(102,10,10);
print(l2.x)]]--


return FVector;
