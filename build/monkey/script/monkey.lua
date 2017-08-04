local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;
local ffi = require "ffi";


nativelibrary("nativesystem");
nativelibrary("application");

monkey = {};

g_Socket = nil;
monkeyhandle = nil;

function monkey:Initialize()
	g_Socket:Connect("127.0.0.1:9999");
end


function monkey:loop()
  local getmsg = nil;
  repeat
    getmsg = monkeyhandle:CheckReceive();
    if nil~=getmsg then    
      local cbuff = getmsg:Data();
      local len = getmsg:GetMessageLength();  
      local strbuf = ffi.string(cbuff, len);
      local restable = msg_capn.MsgLogoutResult.parse(strbuf);
      print(restable.success);
    end
  
  until nil == getmsg
  
  logoutres = {success = true};
  local msg = Message();
  local cbuff = msg:Data();
  local size = msg_capn.MsgLogoutResult.calc_size(logoutres);
  msg_capn.MsgLogoutResult.serialize(logoutres, cbuff, size);
  msg:SetID(MsgID["MID_LOGOUT"]);
  msg:SetMessageLength(size);
  g_Socket:Send(msg);
end


