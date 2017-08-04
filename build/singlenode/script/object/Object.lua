
--object基类
--package.path = package.path .. ";../common/FVector.lua"
--local FVector = require "FVector"
local FVector = require "../common/FVector"

 local Object = {
 	ID,--客户端的唯一ID,在Object生成时动态生成
 	Position,--该物体在场景中的浮点位置,初始化为<0.0f, 0.0f, 0.0f>
	Rotation,--该物体在场景中的旋转角度,初始化为<0.0f, 0.0f, 0.0f>
	MsgTime, -- 最近一次接受服务器消息时间
	MemGID,
	RoomGID,
 }; 
Object.__index = Object
 function Object:new(o)
	o = o or {};
	setmetatable(o,Object);			
	o.Name = "Object";
	o.Location = FVector:new();
	return o;	
end
function Object:Cast(o)	
	setmetatable(o,Object);			
	return o;	
end

function Object:SetServerID( id)
	self.ID = id;
end
function Object:GetServerID( )
	return self.ID;
end
function Object:SetMemGID( id)
	self.MemGID = id;
end
function Object:GetMemGID( )
	return self.MemGID;
end
function Object:SetRoom( roomgid )
	self.RoomGID = roomgid;
end
function Object:GetRoom(  )
	local Room = require "pvp.Room";
	print("----------------todochenjiazeng-----------")
	return Room;
end


function Object:Tick( DeltaTime )
	--print(self.Name  .. ": Tick " )
end


function Object:SetPosition(x,y,z)
	self.Location:SetPos(x,y,z);
end
function Object:GetPosition( )
	return self.Location;
end
function Object:SetMsgTime(time)
	self.MsgTime = time;
end
function Object:GetMsgTime( )
	return self.MsgTime;
end

function Object:SetFaceDir(fdir)
	self.Rotation:SetY(fdir);
end
function Object:GetFaceDir()
	return self.Rotation:GetY();
end
function Object:GetOwnerId( )
	return -1;
end

function Object:Test()
	print("Object:Test " .. self.Name)
end


return Object;