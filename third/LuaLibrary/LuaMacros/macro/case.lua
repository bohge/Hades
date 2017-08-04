require 'macro'
-- case.lua
-- Implementing a general case statement in Lua
--~ function testcase(k)
--~ 	case (k)
--~ 	  is 10,11: return 1
--~ 	  is 12: return 2
--~ 	  is 13 .. 16: return 3
--~ 	  else return 4
--~ 	end
--~ end
--

local strmatch = string.match
local strfind = string.find

macro.define('case',{'var'},'do local __case = var; if false then',
	function(ls)
		macro.set_end_scanner('end end',-1)
	end
)

-- looks for the particular pattern {'<number>',r1,'..',false,'<number>',r2}
-- and returns r1 and r2 if successful
-- There is a _lexical_ gotcha with r1..r2 without whitespace around '..'; Lua regards this as a malformed number
local function try_extract_range(arg)
    if arg[1] == '<number>' and arg[3]=='..' and arg[5]=='<number>' then
        return arg[2],arg[6]
    end
end

local condn = macro.lex '(r1<=__case and r2>=__case)'

macro.define('is',{'value',handle_parms = macro.parameter_grabber(':')},
    function(ls,...)
        local args = {...}
        local subst,put = macro.subst_putter()
		local replace,number = macro.replace,macro.number
		if #args == 0 then macro.error "expects values" end
		if args[1][2] == 'any' then -- 'is any' is the default case...
			put('else')
		else
			put('elseif')
			for i,arg in ipairs(args) do
				if arg[1] == '~' and arg[3] == '<string>' then
					put('<name>','__case'); put(':'); put('<name>','find'); put('(')
					put(arg[3],arg[4]); put(')')
				else
					local r1,r2 = try_extract_range(arg)
					if r1 then
						put(replace({'r1','r2'},{number(r1), number(r2)},condn))
					else
						put(arg); put('=='); put('<name>','__case')
					end
				end
				if i < #args then
					put('or')
				end
			end
			put('then')
		end
        return subst
    end
    )

