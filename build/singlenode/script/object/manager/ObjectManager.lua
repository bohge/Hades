
--package.path = package.path .. ";../Object.lua"
--print(package.path)
local Object = require "object.Object"
local ObjectCharacter = require "object.ObjectCharacter"
--ObjectManager
local ObjectManager={
	ObjMap,
	ObjCount,
	ObjIdIndex,	
};

function ObjectManager:new(o)
	o = o or {};
	self.__index = self;	
	setmetatable(o,self);

	return o;	
end
function ObjectManager:Cast(o)	
	self.__index = self;	
	setmetatable(o,self);
	return o;	
end
function ObjectManager:Init( )	
	self.ObjCount = 0;		
	self.ObjIdIndex = 0;	
	self.ObjMap = {};

end



function ObjectManager:Tick( DeltaTime ) 
end

function ObjectManager:Release()
  for key, value in pairs (self.ObjMap) do
  		self.ObjMap[key] = nil;
  end
  self.ObjCount = 0 ;
end
function ObjectManager:GetObjectCount()
	return self.ObjCount;
end
function ObjectManager:AddObject( obj )	
	self.ObjMap[self.ObjIdIndex] = obj._GID;
	obj:SetServerID(self.ObjIdIndex);
	self.ObjIdIndex = self.ObjIdIndex + 1;
	self.ObjCount = self.ObjCount + 1;
end


function ObjectManager:FindObject(serverid)

	if(self.ObjMap == nil) then
		return nil;
	end
	local ObjGID = self.ObjMap[serverid];
	if(ObjGID == nil) then
		print("cann't find object by id  " .. id)
		return nil;
	end	
	
	return ObjGID;
	
end
function ObjectManager:DeleteObject( serverid )
	if(self.ObjMap[serverid] == nil) then
		print("cann't delete object by id " .. id)
		return;
	end
	self.ObjMap[serverid] = nil;
	self.ObjCount = self.ObjCount - 1 ;
end


return ObjectManager;
