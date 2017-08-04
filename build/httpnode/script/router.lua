local image = require "image"
require "arclight"
require "utility"
require 'nngraph';
require 'torch';
require 'nn';

nativelibrary("nativesystem");
nativelibrary("httpserver");


local router = {};
router.Root = nil;
router.Handlers = nil;

function router:Initialize()
  self.Handlers = {};
  self.Root = Httpserver:GetRoot();
  self:_Resource("/isonline", "controller.isonline");
end

--uid相关操作不是线程安全，不要同时多个服务使用
function router:OnRequest(uid)
  local status, res = pcall(router._OnRequest, router, uid);
  if true ~= status then
    Httpserver:Reply(uid, 500, "Oops...");
  end
end

function router:_OnRequest(uid)
  local cmd = Httpserver:GetCommand(uid);
  if cmd == Httpserver.POST then
    return router:_OnPost(uid);
  elseif cmd == Httpserver.GET then
    return router:_OnGet(uid);
  end  
end

function router:_OnGet(uid)
  local param = nil;
  local url = Httpserver:GetRequsetUrl(uid);
  print(url);
  local urlparam = arclight.stringsplit(url,"?");
  url = urlparam[1];
  if nil ~= urlparam[2] then
    param = urlparam[2];
    param = arclight.stringsplit(param,"&");
    param = arclight.commandparser(param);
  end
  local handler = self.Handlers[url];
  if nil == handler then
    Httpserver:Reply(uid, 404, "Missing...");
  else
    if nil == handler.index then
      Httpserver:Reply(uid, 404, "Missing...");
    else
      local res = handler:index(param)
      Httpserver:Reply(uid, 200, tostring(res));
    end    
  end  
end

function router:_OnPost(uid)
  local url = Httpserver:GetRequsetUrl(uid);
  local handler = self.Handlers[url];
  if nil == handler then
    Httpserver:Reply(uid, 404, "Missing...");
  else
    if nil == handler.create then
      Httpserver:Reply(uid, 404, "Missing...");
    else
      local res = handler:create();
      Httpserver:Reply(uid, 200, tostring(res));
    end    
  end  
end



function router:_Resource(url, name)
  local handler = require(name);
  handler:Initialize();
  if nil ~= self.Handlers[url] then
    ERROR("Router "..url.." is exist");
  else
    router.Handlers[url] = handler;  
  end
end

return router;