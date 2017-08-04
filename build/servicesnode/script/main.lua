

nativelibrary("nativesystem");
nativelibrary("application");

local main = {};


g_Server = nil;

main.Client = {}

function main:Initialize()
  ILogSystem:Log("Initialize");
  g_Application:ScriptInitialized();
end

function main:OnConnect(gid)
  local clientservice = IServicesSystem:Create("Client");
  clientservice:Initialize();
  ITimerSystem:RegisterTimer(clientservice, 5, "OnTimerTriggered");
  g_Application:SetClientHandle(gid, clientservice);
  clientservice = nil;
end

function main:OnDisconnect(gid)
  print("OnDisconnect");
end


return main;