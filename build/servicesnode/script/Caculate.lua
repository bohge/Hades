nativelibrary("nativesystem");
nativelibrary("application");
Caculate = {};


function Caculate:Add(a, b)
  return a + b;
end

function Caculate:Send(gid, buffer)
  g_Server:SendMessage(gid, buffer);
end


function Caculate:Print()
  --print("12345");
end