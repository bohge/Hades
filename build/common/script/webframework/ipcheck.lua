nativelibrary("httpserver");
require "utility"


local ipcheck = {};
ipcheck.ips = nil;
function ipcheck:Setip(ip)
  self.ips = self.ips or {};
  table.insert( self.ips, ip );
end

function ipcheck:_isAllow(ip)
  local allow = false;
  for _, legal in ipairs(self.ips) do
    --LOG(ip .." ? ".. legal);
    if legal == ip then
      allow = true;
    end    
  end
  return allow;
end

function ipcheck:Before(uid)
  local ip = Httpserver:GetAddress(uid);  
  if self:_isAllow(ip) then
    return true;
  end
  ERROR("unkown source "..ip);
  return false;
end

return ipcheck;