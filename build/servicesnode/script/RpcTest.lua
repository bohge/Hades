local RpcBase = require "RpcBase"


nativelibrary("nativesystem");



RpcTest = {};
RpcTest.count = 0;
RpcTest.__index = RpcTest;
setmetatable(RpcTest, RpcBase);


function RpcTest:Init()
  print("Init");
  RpcTest.Rpc = IServicesSystem:Create("Caculate");
end

function RpcTest:Add(t1, t2)
  local res = {};
  for i = #t1, 1, -1 do
    --print("table 1 " .. t1[i]);
    --print("table 1 " .. t2[i]);
    res[i] = RpcTest.Rpc:Add(t1[i], t2[i]);
  end
  return res;
end

function RpcTest:Func1()
  self.count = self.count +1;
  print(self.count);
  return 1;
end


function RpcTest:Func2(clientid, requesttype, buffer)
  return 1;
end




