local msg_capn = require "protocol/msg_capn_capnp"
local MsgID = require "protocol/msg_message_id_capnp".MsgMessageID;



local Simpleecho = {};
Simpleecho.MsgID = 999;

function Simpleecho:Initialize()
end


function Simpleecho:OnRequest()
  --[[local test1 = g_Machine:GetInstance();
  local test2 = g_Machine:GetInstance();
  local test3 = g_Machine:GetInstance();
  g_Request:CreateAccessor(test1);
  g_Request:CreateAccessor(test2);  
  
  if g_Request:GetAccessors() then
    local tabletest1 = test1:GetScriptInstance();
    local tabletest2 = test2:GetScriptInstance();
    
    g_Request:CreateAccessor(test3);
    if g_Request:GetAccessors() then
      print("ok");
    end
  end]]--
  g_Server:SendMessage(gid, buffer);
end

return Simpleecho;