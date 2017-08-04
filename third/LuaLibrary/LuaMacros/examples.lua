require 'macro'
local def = macro.define

-- these macros are included and used by test-examples.lua

-- (1) New C style optional macro symtax (after a suggestion by Thomas Lauer)
def 'L1(x,e) function(x) return e end'
def 'T print "hello"'

-- used to have to say this
--def('L1',{'x','e'},'function(x) return e end')
--def ('T',nil,'print "hello"')

-- (2) a substitution function may return a string, instead of a token list
def ('B',nil,function() return 'print "yelp"' end)

-- (3) macros may set lexical scanners, making it possible to take action on a terminating end.
-- This is Leaf Storm's begin..end proposal;
--  fun begin .. end is expanded as fun (function(...) .. end)

def ('begin',nil,function()
    macro.set_end_scanner 'end)'
    return '(function(...)'
end)

-- the substitution function (as before) may completely take on the responsibility of reading from
-- the token stream and generating a token list which will be fed into the output stream.
--
-- Implements: foreach x in t do, which is expanded as for _,x in ipairs(t) do
-- (Look at macro/foreach.lua for a more powerful version)
def ('foreach',nil,
    function (ls)
        -- getting
        local get = ls.getter
        local _,t1,name = get()
        local _,t2 = get()
        if t2 ~= 'in' or t1 ~= '<name>' then macro.error("expecting 'in' after single loop variable") end
        -- grab everything up to do
        local args = macro.grab_parameters('do','')
        if not args then macro.error("expecting 'do'") end
        -- putting
        local subst,put = macro.subst_putter()
        local function putn (n) put('<name>',n) end

        put 'for'; putn '_'; put ','; putn(name); put 'in';
        putn('ipairs'); put '('
        put (args[1])
        put ')'; put 'do'
        return subst
    end
)

-- (4) there is a higher-order API which hides some of the details of token lists.
--
-- this implements something like Perl's qw quoting function. The single argument is a space-separated
-- list of stuff (could be anything!) and the result is a table of the quoted values.
-- http://www.perlmeme.org/howtos/perlfunc/qw_function.html
--
-- qw(alice john function <) expands to {'alice','john','function','<'}
--
-- Illustrates how you can put just about anything in a macro parameter list ;)  The default grabber
-- treats commas specially, but this grabber ignores them
--
def ('qw',{'arg'; handle_parms = macro.parameter_grabber(false,false,'<start>')},
	function(ls,arg)
		local subst,put = macro.subst_putter()
		put '{'
		for i = 1,macro.length_of(arg) do
			put('<string>',macro.value_of(arg,i))
			put ','
		end
		put '}'
		return subst
	end
)

-- The difference between value_of() and get_token(), is that the first returns a string,
-- e.g. 'function','alice','+', whereas the second returns a single token as a table,
-- e.g. {'function',false},{'<name>','alice'}, etc. The putter function is overloaded so that
-- you can either pass one or two strings (put ';' or put('<name>','alice')) or a token list.
--
-- a useful macro for bringing in symbols from a table as locals.
-- E.g. import(math,sin cos) expands to 'local sin = math.sin; local cos = math.cos'
def ('import',{'tbl','names'},
	function (ls,tbl,names)
		local subst,put = macro.subst_putter()
		for i = 1,macro.length_of(names) do
			local name = macro.get_token(names,i)
			put 'local'; put (name); put '='; put (tbl); put '.'; put (name); put ';'
		end
		return subst
	end
)

-- (5) macros may be arbitrary characters
-- lhf-style lambda notation
def ('\\', {'args','body';handle_parms = true},
	'function(args) return body end',
    function(ls) -- grab the lambda
        -- these guys return _arrays_ of token-lists. We use '' as the delim
        -- so commas don't split the results
        local args = macro.grab_parameters('(','')[1]
        local body = macro.grab_parameters(')','')[1]
        return args,body
    end
)


-- (6) new tokens can be formed, from token pairs.
-- this is David Manura's block-scope notation
macro.assign ('::','begin')


-- (7) actions can be assigned to tokens or names. These macros pass through 'undigested' and
-- exist purely for their side-effects.
local action = macro.action

local lev = 1

local function open_scope ()
    lev = lev + 1
end

local function close_scope ()
    lev = lev - 1
end

action ('function',open_scope)
action ('do',open_scope)
action ('then',open_scope)
action ('end',close_scope)


