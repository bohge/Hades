local image = require "image"
require "arclight"
require "utility"
require 'nngraph';
require 'torch';
require 'nn';

nativelibrary("nativesystem");
nativelibrary("httpserver");




local http = {};

function http:Initialize()
  self.meanstd = IFileSystem:PathAssembly("docs:save/preprocess.table");
  self.meanstd = torch.load(self.meanstd);
  
  self.neuronnets = {};
  local inpath = IFileSystem:PathAssembly("docs:save");
  for file in paths.iterfiles(inpath) do
    if string.match(file, ".tensor") then
      local path = inpath .. "/" .. file;
      LOG("load file "..file);
      self:_LoadNeuronnet(path, file);
    end    
  end
  LOG("done");
end

function http:_LoadNeuronnet(p, file)
  p = IFileSystem:PathAssembly(p);
  net = torch.load(p, "ascii");
  net:evaluate();
  local data =
  {
    net = net,
    name = file;
  }
  table.insert(self.neuronnets, data);
end


--uid相关操作不是线程安全，不要同时多个服务使用
function http:OnRequest(uid)
  local url = Httpserver:GetRequsetUrl(uid);
  local cmd = Httpserver:GetCommand(uid);
  if cmd == Httpserver.POST and url == "/test" then
    local imagetype = Httpserver:GetType(uid, "image");
    if imagetype == "image/jpeg"
      or imagetype == "image/png" then
      local imagepath = Httpserver:GetValue(uid, "image");     
      local input = image.load(imagepath,3,'double');
      input = image.scale(input,96,96);
      input = input:view(1,3,96,96);
      local mean = self.meanstd[1];
      local std = self.meanstd[2];
      for i=1,3 do
        input[{ {},i,{},{} }]:add(-mean[i])
        input[{ {},i,{},{} }]:div(std[i])
      end
      reply = "";
      for i,data in ipairs(self.neuronnets) do
        local restensor = data.net:forward(input);
        reply =  reply ..i.. "st net: The probability that this image is pron is "..string.format("%.2f", restensor[1][1]*100).."% net name is "..data.name.."</br>";
      end      

      Httpserver:Reply(uid, 200, reply);
    else
      Httpserver:Reply(uid, 404, "error format only jpg png...");
    end
  else
    Httpserver:Reply(uid, 404, "Missing...");
  end
end

return http;