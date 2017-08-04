-- list comprehensions
-- L{i for i=1,5}
local errmsg = "syntax: L{<expr> for <loop-part>}"
local error = macro.error
local grab_parameters = macro.grab_parameters

macro.define('L',{'expr','loop_part',handle_parms=true},
    ' ((function() local t = {}; for loop_part do t[#t+1] = expr end; return t end)())',
    function(ls)
        local _,t = ls.getter()
        if t ~= '{' then error(errmsg) end
        local expr = grab_parameters('for')
        local loop_part = grab_parameters('}','')
		if not expr or not loop_part then error(errmsg) end
        return expr[1],loop_part[1]
    end)

