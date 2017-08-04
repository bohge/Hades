--ObjectNpc 
local ObjectNpc = require "ObjectNpc"
local ObjectPlayerOther = {
};

 function ObjectPlayerOther:new()
	o = ObjectNpc:new();		
	o.Name = "ObjectPlayerOther";	
	return o;	
end
function ObjectPlayerOther:Cast(o)	
	setmetatable(o,ObjectPlayerOther);			
	return o;	
end
local other1 = ObjectPlayerOther:new();
other1:Test();
return ObjectPlayerOther;