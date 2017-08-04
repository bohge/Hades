local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local PlayerMoveStrat = {};
PlayerMoveStrat.MsgID = MsgID["MID_PLAYER_MOVE_START"];

function PlayerMoveStrat:Initialize()
end


function PlayerMoveStrat:OnRequest(room)  

  local inputinfo = Utility_CapnpDecoder( msg_capn.MsgPlayerMoveStart, buffer );
  local dir = inputinfo.dir;
  local posx = inputinfo.posx;
  local posy = inputinfo.posy;
  local posz = inputinfo.posz;
  local Player = room:GetPlayerByGID( gid, request);     
  Player:StartMove(dir,posx,posy,posz);



  local PlayerMoveStratResult = 
  {
    playerid = gid,
    dir = dir,
    posx = posx,
    posy = posy,
    posz = posz,
  }
  Utility_CapnpEncoder(
    msg_capn.MsgPlayerMoveStartResult,
    MsgID["MID_PLAYER_MOVE_START_RESULT"],
    PlayerMoveStratResult);  
  room:BroadcastMsgToOhterPlayers(gid,buffer);
end

return PlayerMoveStrat;