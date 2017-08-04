local Module = nn.Module

function Module:getOutputs()
   local outputs = self:outputs();
   local o = Module.flatten(outputs);
   return o;
end

function Module:outputs()
  return self.output;
end

--这个函数的作用是使网络返回那些非学习但是需要保存和同步的参数
--例如BatchNormalization中的running_mean,running_std
function Module:subparameters()
  return;
end

function Module:getSubparameters()
   local subparameters = self:subparameters();
   local p = Module.flatten(subparameters);
   return p;
end

function Module:zeroSubparameters()
   local sp = self:subparameters()
   if sp then
      for i=1,#sp do
         sp[i]:zero()
      end
   end
end