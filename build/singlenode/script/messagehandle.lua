
local Messagehandle = {};
Messagehandle.Handler = {};

function Messagehandle:RegisteHandle(handler)
  if nil ~= handler.Initialize then
    handler:Initialize();
  end  
  local id = handler.MsgID;
  assert( nil ~= id );
  self.Handler[id] = handler;
end

function Messagehandle:HandleMessage(room)
  local mesid = g_Buffer:GetID();
  local handler = self.Handler[mesid];
  --print("gid " .. tostring(gid) .. " message id "..tostring(mesid));
  if nil ~= handler then
    return handler:OnRequest(room);
  else
    print("uncapture message id "..tostring(mesid));
  end
end

return Messagehandle;