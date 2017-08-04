local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local PlayerInfo = require "playerinfo";

local NEW_PLAYER_INSTANCE_STACK_INDEX = 0;


local PlayerMoveMeet = {};
PlayerMoveMeet.MsgID = MsgID["MID_PLAYER_MOVE_MEET_OBSTACLE"];

function PlayerMoveMeet:Initialize()

end


function PlayerMoveMeet:OnRequest()  
  local csgid = g_Server:GetClientSpecific(gid);
  
  local instancehandler = g_Machine:GetInstance();
  g_Request:ReadwriteAccessor(csgid, instancehandler);
  
  if g_Request:GetAccessors() then
    local playerinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerMoveMeetObstacle, buffer );
    
    local clientspecific = instancehandler:GetScriptInstance();--得到请求的原始数据
    
    clientspecific = PlayerInfo:new(clientspecific);
    clientspecific:SetPostion(playerinfo.posx, playerinfo.posy, playerinfo.posz);
    
    local playerinforesult = 
    {
      playerid = gid,
      posx = playerinfo.posx,
      posy = playerinfo.posy,
      posz = playerinfo.posz,
    }
    Utility_CapnpEncoder(
      msg_capn.MsgPlayerMoveMeetObstacleResult,
      MsgID["MID_PLAYER_MOVE_MEET_OBSTACLE_RESULT"],
      buffer, playerinforesult);    
    g_Server:Broadcast(gid, buffer);
  end
end


return PlayerMoveMeet;