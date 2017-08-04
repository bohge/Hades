local m1 = require "m1";
local m2 = require "m2";
local m3 = require "m3";
local m4 = require "m4";
local m5 = require "m5";
local m6 = require "m6";

test = {};

function test:Initialize()
  self.m1 = m1:new();
  self.m2 = m2:new();
  self.m3 = m3:new();
  self.m4 = m4:new();
  self.m5 = m5:new();
  self.m6 = m6:new();
  
  meta = getmetatable(self.m1);
  print(meta);
  meta = getmetatable(self.m2);
  print(meta);
  meta = getmetatable(self.m3);
  print(meta);
  meta = getmetatable(self.m4);
  print(meta);
  meta = getmetatable(self.m5);
  print(meta);
  meta = getmetatable(self.m6);
  print(meta);
end


function test:loop()

end