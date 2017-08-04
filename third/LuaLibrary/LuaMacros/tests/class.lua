function _class(base,body)
    local mt = {G = _G}
	-- inject methods of base class into our metatable....
	if base then
		for k,v in pairs(base) do
			mt[k] = v
		end
		mt._init = nil
		mt._base = base
	end
	mt.__index = mt
	setmetatable(mt,{__index=_G})
    setfenv(body,mt)
    body()

    function mt.new(...)
        local self = {}
        setmetatable(self,mt)
		if mt._init then
			mt._init(self,...)
		end
        return self
    end

    return mt
end

if macro then

	macro.define('class',{'name','base',handle_parms = true},
		'name = _class(base,function()',
		function(ls)
			local base
			local name = ls:get_name()
			if not name then macro.error 'expecting class name' end
			if ls:get_name() ~= 'is' then
				base = ls:get_name()
				if not base then macro.error 'expecting base name' end
				macro.assert_expecting(ls:get_token() ,')')
				macro.assert_expecting(ls:get_name(),'is')
			end
			if not base then
				base = macro.token('nil')
			else
				base = macro.name(base)
			end
			name = macro.name(name)
            macro.set_end_scanner ('end)')
			return name,base
		end)

	--macro.define('endclass',nil,'end)')
end


