local Container = nn.Container

function Container:outputs()
  local os = {}
  for i=1,#self.modules do
    local o = self.modules[i]:outputs()
    if o then
      table.insert(os, o)
    end
  end
  return os;
end

function Container:subparameters()
    local function tinsert(to, from)
        if type(from) == 'table' then
            for i=1,#from do
                tinsert(to,from[i])
            end
        else
            table.insert(to,from)
        end
    end
    local sp = {}
    for i=1,#self.modules do
        local msp = self.modules[i]:subparameters()
        if msp then
            tinsert(sp,msp)
        end
    end
    return sp
end