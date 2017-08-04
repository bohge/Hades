local RoomServer = {
	RoomManager,
	clientmap;
}

function RoomServer.Init( request )
	
	
  	--[[local RoomTable = g_Request:CreateAccessor();
 	if g_Request:GetAccessors() then    	
    	local Room = Room:new(RoomTable);
    	Room.ID = "Room ID = 0";
    	GlobalDataList.PlayerListAccessor.RoomGID = Room._GID;    	
    	print("RoomGID mew " ..  Room._GID)
	end
	g_Request:FreeAccessors();
	-- body
	--RoomManager.Init();]]--
end
function RoomServer:Tick( ... )
	-- body
	--RoomManager.Tick();
end
function RoomServer:Destroy( ... )
	-- body
end
return RoomServer;