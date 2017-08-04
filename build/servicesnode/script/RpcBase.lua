nativelibrary("nativesystem");
nativelibrary("application");


local RpcBase = {};

--[[function RpcBase:Func1()
  print("base");
end]]--

function RpcBase:FuncBase1(num, str, table, str2, rpc)
  --[[print(table.a);
  print(table.b.a);
  print(table.b.b[1]);
  print(table.b.b[2]);
  print(str);
  print(num);
  print(str2);]]--
  newstr = "zxczxc";
  newstr2 = "89898";
  --sdad:asda();
  rpc:Func1();
  return newstr, newstr2;
  --return str, str2;
end


function RpcBase:FuncBase2(clientid, requesttype, buffer)
  return 2;
end

return RpcBase;