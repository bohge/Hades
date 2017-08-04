local Scene={
	Room ,--与Room一一对应的
	ObjMgr,

}
function Scene:SetRoom( ... )
	-- body
end
function Scene:GetRoom()
end

function Scene:Init( ... )
	ObjMgr = ObjectManager:new();
end
function Scene:Tick( second )
	-- body
end
function Scene:EnterScene( user )
	-- body
end

function Scene:LeaveScene( user )
	-- body
end

function Scene:NotifyUsers( messageid,msg )
	-- body
end

function Scene:SyncUser( ... )
	-- body
end

return Scene;
