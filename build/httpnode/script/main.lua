nativelibrary("nativesystem");
nativelibrary("application");
nativelibrary("httpserver");
require "arclight"
require "utility"
require "torch"


local main = {};

main.HttpHandle = nil;

function main:Initialize()
  local nThreads = torch.getnumthreads();
  Httpserver:StartServer(Httpserver.HTTP, 18080, true);
  self.HttpHandle = {}
  for i=1, 1 do
    local handle = IServicesSystem:Create("http");
    handle:Initialize();
    table.insert(self.HttpHandle, handle);
    Httpserver:PushHandle(handle);
  end
  g_Application:ScriptInitialized(nil);--不接受用户的链接
end

function main:OnCommandline(command)
  LOG("OnCommandline " .. command);
end

return main;