require 'macro'

local matched_do,matched_if

local function do_or_if (t)
	matched_do = t=='do'
	matched_if = t=='if'
	return matched_do or matched_if
end
-- the substitution function (as before) may completely take on the responsibility of reading from
-- the token stream and generating a token list which will be fed into the output stream.
--
-- Implements: foreach x in t do, which is expanded as for _,x in ipairs(t) do
-- can also have foreach x in t if condn do, which expands as
--  for _,x in ipairs(t) do if condn then... An end scanner is set to balance this.
macro.define ('foreach',nil,
    function (ls)
        -- getting
        local get = ls.getter
        local _,t1,name = get()
        local _,t2 = get()
        if t2 ~= 'in' or t1 ~= '<name>' then macro.error("expecting 'in' after single loop variable") end
        -- grab everything up to do
		matched_do,matched_if = false,false
        local args = macro.grab_parameters(do_or_if,'')
        if not args then macro.error("expecting 'do'") end
        -- putting
        local subst,put = macro.subst_putter()
        local function putn (n) put('<name>',n) end

        put 'for'; putn '_'; put ','; putn(name); put 'in';
        putn('ipairs'); put '('
        put (args[1])
        put ')'; put 'do'
		if matched_if then
			args = macro.grab_parameters('do','')
			put 'if'
			put (args[1])
			put 'then'
			-- we will need to append an extra 'end' to balance this!
			-- the end scanner is going to see 'do if <condn> then', which is a +2 jump
			macro.set_end_scanner ('end end',-1)
		end
        return subst
    end
)
