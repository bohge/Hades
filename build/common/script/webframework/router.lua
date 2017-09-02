require "utility"

nativelibrary("httpserver");


local router = {};
router.Root = nil;
router.Handlers = nil;

local function urldecode(s)
  s = s:gsub('+', ' ')
       :gsub('%%(%x%x)', function(h)
                           return string.char(tonumber(h, 16))
                         end)
  return s
end

local function parseurl(s)
  local ans = {}
  for k,v in s:gmatch('([^&=?]-)=([^&=?]+)' ) do
    ans[ k ] = urldecode(v)
  end
  return ans
end

local function stringsplit(inputstr, sep)
        if sep == nil then
                sep = "%s"
        end
        local t={} ; i=1
        for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
                t[i] = str
                i = i + 1
        end
        return t
end

function router:Initialize()
  self.Handlers = {};
  self.Root = Httpserver:GetRoot();
end

--uid相关操作不是线程安全，不要同时多个服务使用
function router:OnRequest(uid)
  local function xpcall_error_handler(err)
   ERROR(tostring(err) .. "\noriginal " .. debug.traceback("", 2):sub(2));
  end
  local status, result = xpcall(
    function()
      return self:_OnRequest(uid)
    end,
    xpcall_error_handler);
  if true ~= status then
    Httpserver:Reply(uid, 500, "Internal error...");
  end
end

function router:_OnRequest(uid)
  local cmd = Httpserver:GetCommand(uid);
  local fullurl = Httpserver:GetRequsetUrl(uid);
  local urlparam = stringsplit(fullurl,"?");
  local url = urlparam[1];
  local pair = self.Handlers[url];
  if nil == pair then
    Httpserver:Reply(uid, 404, "Missing...");
  else    
    local middleware = pair.middleware;
    if nil ~= middleware then      
      if not middleware:Before(uid) then
        Httpserver:Reply(uid, 400, "Forbidden...");
        return;
      end      
    end    
    if cmd == Httpserver.POST then
      router:_OnPost(uid, url, pair.handler);
    elseif cmd == Httpserver.GET then
      router:_OnGet(uid, urlparam[2], pair.handler);
    end
  end  
end

function router:_OnGet(uid, urlparam, handler)
  local param = nil;
  if nil ~= urlparam then
    param = urlparam;
    param = parseurl(param);
  end
  if nil == handler then
    Httpserver:Reply(uid, 404, "Missing...");
  else
    if nil == handler.index then
      Httpserver:Reply(uid, 404, "Missing...");
    else
      local res = handler:index(self, param)
      Httpserver:Header(uid, "Access-Control-Allow-Origin", "*");
      Httpserver:Header(uid, "Content-type", "text/html; charset=utf-8");
      Httpserver:Reply(uid, 200, tostring(res));
    end    
  end  
end

function router:_OnPost(uid, url, handler)
  if nil == handler then
    Httpserver:Reply(uid, 404, "Missing...");
  else
    if nil == handler.create then
      Httpserver:Reply(uid, 404, "Missing...");
    else
      local res = handler:create(self, uid);
      Httpserver:Reply(uid, 200, tostring(res));
    end    
  end  
end



function router:Resource(url, name, middleware)
  local handler = require(name);
  handler:Initialize();
  if nil ~= self.Handlers[url] then
    ERROR("Router "..url.." is exist");
  else
    local pair = {};
    pair.middleware = middleware;
    pair.handler = handler;
    self.Handlers[url] = pair;  
  end
  return handler;
end

return router;