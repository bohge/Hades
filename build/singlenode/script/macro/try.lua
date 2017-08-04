require 'macro'
local def = macro.define
function pack (...)
	return {n=select('#',...),...}
end

def ('try',nil,
 'do local res = pack(pcall(function()'
  -- try block goes here
)

def ('except',{'e',handle_parms=macro.grab_token},
	function()
		-- make sure that the 'end' after 'except' becomes 'end end' to close
		-- the extra 'do' in 'try'.
		-- we start at level 1 (before 'end))') and must ignore the
		-- first level zero.
		macro.set_end_scanner ('end end',1,true)
		return [[
	end))
	if res[1] then
		if res.n > 1 then return unpack(res,2,res.n) end
	else e = res[2]
	]]
	-- except block goes here
    end
)


return 'local pack,pcall,unpack = pack,pcall,unpack'

