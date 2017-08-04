--- A Lua macro module based on lhf's token filter facility.
-- See macro-defs.lua for some examples; you can define C-style lexical
-- macros which can take parameters.
-- If no parameters are provided for such a macro, then you have to supply
-- a handler function which returns
-- the arguments as token lists.
-- @class module
-- @name macro
-- Example invocation:
--- lua -lmacro -lmacro-defs test-macro.lua
-- Steve Donovan, 2007-2009
-- In the spirit of tokenf, this is hereby placed in the public domain.

-- imports
local append = table.insert
local push = append
local pop = table.remove
local ipairs,pairs = ipairs,pairs
local unpack = unpack
local find = string.find
local type = type
local co_wrap = coroutine.wrap
local yield = coroutine.yield
local print = print
local global_loadstring = loadstring
local G = _G


module("macro")

--~ verbose = 1


local macros = {}
local tlk = 0
local current_source
local current_line
local get
local macname

local keyword = {
    ["and"] = true, ["break"] = true,  ["do"] = true,
    ["else"] = true, ["elseif"] = true, ["end"] = true,
    ["false"] = true, ["for"] = true, ["function"] = true,
    ["if"] = true, ["in"] = true,  ["local"] = true, ["nil"] = true,
    ["not"] = true, ["or"] = true, ["repeat"] = true,
    ["return"] = true, ["then"] = true, ["true"] = true,
    ["until"] = true,  ["while"] = true
}

local macro_map = {['<name>'] = true}  -- testing

--- used to finish macro preprocessing in case of an error.
-- @param msg a message
-- @param line optional line number
 function error(msg,line)
    if not line then line = current_line end
    reset() -- important to be ready for new input,
    if not current_source then current_source = '<unknown>' end
    if not line then line = -1 end
    if not macname then macname = '?' end
    G.error(('%s:%s: %s %s'):format(current_source,line,macname,msg),0)
end

function assert(value,msg)
    if not value then
        msg = msg or 'macro assert failed'
        error(msg)
    else
        return value
    end
end

function assert_expecting(value1,value2)
    return assert(value1 == value2,"expecting '"..value2.."'")
end

function name(x) return {'<name>',x} end
function string(x) return {'<string>',x} end
function number(x) return {'<number>',x} end
function token(x) return {x,false} end


---- split a string into a list of strings separated by a delimiter.
-- @param s The input string
-- @param re A regular expression; defaults to spaces
-- @return a list-like table
function split(s,re)
    local i1 = 1
    local ls = {}
    if not re then re = '%s+' end
    if re == '' then return {s} end
    while true do
        local i2,i3 = s:find(re,i1)
        if not i2 then
            local last = s:sub(i1)
            if last ~= '' then append(ls,last) end
            if #ls == 1 and ls[1] == '' then
                return {}
            else
                return ls
            end
        end
        append(ls,s:sub(i1,i2-1))
        i1 = i3+1
    end
end


--- define a macro with optional parameters which expands to be the substitution.
-- args may contain a field handle_parms; it is either a custom function to grab arguments,
-- or the value 'true', in which case the argument grabber is fun.
-- @param symbol name of macro, or string of form NAME[(ARGS)] SUBST (just like C macros)
-- @param args  arguments (a list of strings, may be nil for no arguments)
-- @param substitute substitution string, token list or function
-- @param fun optional argument grabber (when args.handle_parms = true)
function define(symbol,args,substitute,fun)
    if symbol:find '%s' then
        local is1,is2 = symbol:find '%s+'
        symbol,substitute = symbol:sub(1,is1-1),symbol:sub(is2+1)
        sym,arg = symbol:match '^([_%w]+)(%b())'
        if sym then
            symbol = sym
            args = split(arg:sub(2,-2),',')
        end
        return define(symbol,args,substitute)
    end
    if type(substitute) == 'string' then
        substitute = lex(substitute)
    end
    local parms = {subst = substitute, parms = args, handler = fun}
    local handle = args and args.handle_parms
    macname = symbol
    if args and args.action then
        parms.action = args.action
    end
    if handle and not fun then
        if type(handle) == 'function' then
            parms.handler = handle
        else
            error('must provide parameter grabber')
        end
    end
    if symbol:find '^[%a_][%w_]*$' and not keyword[symbol] then
        macros[symbol] = parms
    else
        macro_map[symbol] = parms
    end
end

local function remove (t,val)
	local idx
	for i,v in ipairs(t) do
		if v == val then idx = i; break end
	end
	if idx then G.table.remove(t,idx) end
end

function action (symbol,fun)
	local parms = macro_map[symbol]
	if not parms then
		parms = {action={}}
		macro_map[symbol] = parms
	end
    append(parms.action,fun)
end

function remove_action (symbol,fun)
	local parms = macro_map[symbol]
    if not parms or not parms.action then error("'"..symbol.."' is not an action") end
	remove(parms.action,fun)
end

function assign(symbol,mname)
	symbol = lex(symbol)
	local n = length_of(symbol)
	local mac = macros[mname]
	if not mac then error("macro '"..mname.."' is not defined") end
	local t1 = symbol[1]
	if n == 1 then
		macro_map[t1] = mac
	elseif n == 2 then
		local nextt = macro_map[t1]
		if not nextt then nextt = {nextt={}}; macro_map[t1] = nextt end
		nextt.nextt[symbol[3]] = mac
	else
		error("not a symbol")
	end
end

--- remove a macro.
-- @param symbol a existing macro name
function undef(symbol)
    macros[symbol] = nil
end

--- generate a new globally unique variable.
function global()
    tlk = tlk + 1
    return ("__%dMG"):format(tlk)
end

-- token lists. these are lists like {'function',false,'<name>','fred',
-- '(',false,<number>,42,')',false} where false is a placeholder for a nil value.
local function write_nil(v)
    if v == nil then return false
    else return v end
end

local function read_nil(v)
    if not v then return nil
    else return v end
end

local function tappend(tl,token,value)
    append(tl,token)
    append(tl,write_nil(value))
end

local function token_getter(line,tokens)
    local i = 1
    local n = #tokens
    return function()
        if i > n then return false end
        local tok = tokens[i]
        local val = read_nil(tokens[i+1])
        i = i + 2
        return line,tok,val
    end
end

-- append a @token and its @value to a token list @tl
function token_append(tl,token,value)
    tappend(tl,token,value)
end

-- lexer (for massaging strings into valid token lists)

local NUMBER1 = '^%d+%.?%d*[eE][%+%-]?%d+'
local NUMBER2 = '^%d+%.?%d*'
local NUMBER3 = '^0x[%da-fA-F]+'
local IDEN = '^[%a_][%w_]*'
local WSPACE = '^%s+'
local STRING1 = "^'[^']*'"
local STRING2 = '^"[^"]*"'

local tlres

local function tdump(tok)
    tappend(tlres,tok)
end

local function vdump(tok)
    if keyword[tok] then tdump(tok)
    else tappend(tlres,'<name>',tok)
    end
end

local function ndump(tok)
    tappend(tlres,'<number>',tok)
end

local function sdump(tok)
    tappend(tlres,'<string>',tok:sub(2,-2))
end

local matches = {
    {WSPACE,nil},
    {IDEN,vdump},
    {NUMBER1,ndump},
    {NUMBER2,ndump},
    {NUMBER3,ndump},
    {STRING1,sdump},
    {STRING2,sdump},
    {'^%-%-.-\n',nil},
    {'^%[%[.+%]%]',sdump},
    {'^==',tdump},
    {'^~=',tdump},
    {'^<=',tdump},
    {'^>=',tdump},
    {'^%.%.%.',tdump},
    {'^%.%.',tdump},
    {'^.',tdump}
}

--- convert a string into a token list using a lexical scanner.
-- @param s a string
-- @returns a token list
function lex (s)
    local i1,i2,idx
    local sz = #s
    tlres = {}
    while true do
        for _,m in ipairs(matches) do
            i1,i2 = s:find(m[1],idx)
            if i1 then
                local tok = s:sub(i1,i2)
                idx = i2 + 1
                if m[2] then m[2](tok) end
                if idx > sz then return tlres
                else break end
            end
        end
    end
end

--- get a substitution/putter pair for creating a token list.
-- @returns a token list
-- @returns a put function which operates on the token list.
-- @usage subst,put = subst_putter(); put ';' -- subst is now {';',false}
function subst_putter()
    local subst = {}
    return subst,function (t,v)
        if type(t) == 'table' then
            for j,a in ipairs(t) do
                append(subst,a)
            end
        else
            tappend(subst,t,v)
        end
    end
end

--- returns a function which grabs all tokens upto some end token.
-- @param endtoken
-- @param delim
-- @param starttoken
-- @returns a token grabber
-- @see grab_parameters
function parameter_grabber(endtoken,delim,starttoken)
    return function()
        return unpack(grab_parameters(endtoken,delim,starttoken))
    end
end

--- returns a function which grabs the next token of the given type.
-- @param type a token type (e.g. '<name>','<number>', '<string>', etc.)
function next_token_grabber(type)
    return function(ls)
        local line,token,value = ls.getter()
        if token ~= type then
            error("expecting "..type..", received "..token)
        end
        return {token,value}
    end
end

--- get a token from a token list.
-- @param arg a token list
-- @param idx index within the token list
function get_token (arg,idx)
    idx = 2*idx - 1
    return {arg[idx],arg[idx+1]}
end

-- turns a token list into a string
-- @param arg a token list
-- @param idx if not nil, then just return the value of this particular token in the list.
function value_of(arg,idx)
    if idx == nil then
        local s = ''
        for i = 1,#arg,2 do
            if #s > 0 then s = s..' ' end
            if arg[i+1] then
                s = s..arg[i+1]
            else
                s = s..arg[i]
            end
        end
        return s
    else
        local t = get_token(arg,idx)
        return t[2] or t[1]
    end
end

--- length of a token list.
-- @param arg token list
function length_of (arg)
    return #arg/2
end

local function index(tbl,s)
    for i,v in ipairs(tbl) do
        if v == s then return i end
    end
    return 0
end

--- replace all formal parameters in the token list with the values.
-- @param parms formal parameters
-- @param values corresponding values
-- @param tokens a token list
-- @returns a new token list
function replace(parms,values,tokens)
    if type(tokens) == 'string' then
        tokens = lex(tokens)
    end
    local res = {}
    for i=1,#tokens,2 do
        if tokens[i] == '<name>' then
            -- is it a formal macro parameter?
            local idx = index(parms,tokens[i+1])
            if idx > 0 then
                for k,v in ipairs(values[idx]) do
                    append(res,v)
                end
            else -- otherwise just copy the token list...
                tappend(res,tokens[i],tokens[i+1])
            end
        else
            tappend(res,tokens[i],tokens[i+1])
        end
    end
    return res
end

--- grab parameters from the token stream up to some end token.
-- The opening parens (or whatever) is assumed to be already read, unless starttoken is specified.
-- A comma is regarded as a parameter delimiter,
-- but only if it is not inside parenthesis or curly braces.
-- @param endtoken the end token; if not specified, then we use starttoken, which must be defined.
-- @param delim (this may be a function taking one string argument)
-- @param starttoken if specified, the first token will be read and must be this value.
-- @returns a list of token lists.
function grab_parameters(endtoken,delim,starttoken)
--~     print('start')
    local parm_values = {}
    local level = 1 -- used to count ({ and })
    local tl = {}
	local condn,matched_token
	if type(endtoken) == 'function' then condn = endtoken end
    delim = delim == nil and ',' or delim
    if starttoken then
        local _,t,v = get()
        if not endtoken then
            endtoken = t
        else
            if t ~= starttoken then error("expecting "..starttoken) end
        end
    end
    while true do
        line,token,value=get()
        if token == endtoken and level == 1 then
            append(parm_values,tl)
            break
        elseif token == '<eof>' then return end -- watch out for an unclosed parameter list...
        if token == '(' or token == '{' then
            level = level + 1
            tappend(tl,token)
        elseif token == ')' or token == '}' then
            level = level - 1
            if level == 0 then -- finished with parm list
                append(parm_values,tl)
                break
            else
                tappend(tl,token)
            end
        elseif token == delim and level == 1 then
            append(parm_values,tl) -- a new parm
            tl = {}
        elseif level == 1 and (condn and condn(token) or token == endtoken) then
			matched_token = token
            append(parm_values,tl)
            break
        else
            tappend(tl,token,value)
        end
    end
    -- handles the odd but realistic case where the arg list is empty!
    if #parm_values == 1 and parm_values[1][1] == nil then
        return {}
    else
        return parm_values

    end
end

--- grab the next token from the stream.
-- @returns a token (i.e. a {type,value} pair)
function grab_token()
    local _,token,value = get()
    return {token,value}
end

local getters = {}
local scanners = {}
local scanner = nil

--- push a substitution into the token stream.
-- @param line line number
-- @param subst a substitution (either a token list or a string)
function insert_tokens(line,subst)
    -- it's important to push the existing token-getting function so
    -- that we can restore the old stream;  then start grabbing tokens
    -- from subst
    push(getters,get)
    if type(subst) == 'string' then
        subst = lex(subst)
    end
    get = token_getter(line,subst)
end


--- set a lexical scanner. This is a function which receives the token type and value
-- and continues to operate until it returns a substitution, which is either a string or
-- a token list. (This result can be empty, i.e. {})
-- @param fun a scanner.
function  set_scanner (fun)
    push(scanners,scanner)
    scanner = fun
end

--- set a scanner for the next matching 'end' token.
-- @param subst either a substitution, or a function returning a substitution.
function set_end_scanner (subst,lev,ignore_first)
    lev = lev or 0
    set_scanner(function(t)
        if t == 'function' or t == 'if' or t == 'do' then
            lev = lev + 1
--~ 			print('+lev',lev)
        elseif t == 'end' then
            lev = lev - 1
--~ 			print('-lev',lev)
            if lev == 0 then
				if ignore_first then
					ignore_first = false
				else
					if type(subst) == 'function' then
						return subst()
					else
						return subst
					end
				end
            end
        end
    end)
end

--- set a trigger for the next matching token.
-- @param t a token type
-- @param pass true if we want to let the matching token pass through
-- @param fun an action called when we match the token.
function set_trigger(t,pass,fun)
    set_scanner(function(tok)
        if t == tok then
            fun()
            return pass and {t} or {}
        end
    end)
end


local _lstate = {}

function _lstate:get_name()
    local _,type,value = self.getter()
    if type == '<name>' then
        return value
    end
end

function _lstate:get_token()
    local _,token,value = self.getter()
    if value == nil then
        return token
    end
end

local line_offs,line_offs_reset

local function F(getter,put)
    put("F init")
    get = getter
    line_offs = 0
    local line,token,value
    local function lstate()
        _lstate.source = current_source
        _lstate.line = line
        current_line = line
        _lstate.getter = get
        return _lstate
    end
    while true do
        line,token,value=get()
        if not line then -- one of our token stream getters has finished
            while true do -- pop stack until we get to an unfinished stream...
                get = pop(getters)
                line,token,value=get()
                if line then break end
            end
        end
        if scanner then
            local res = scanner(token,value)
            if res then
                scanner = pop(scanners)
                insert_tokens(line,res)
                line,token,value=get()
            end
        end
        local trigger,mac = macro_map[token]
        if trigger then
            if trigger==true then -- regular identifier macro
                mac = macros[value]
            else  -- must be arb token macro
				if trigger.nextt then
					local line2,token2,value2 = get()
					mac = trigger.nextt[token2]
					if not mac then
						insert_tokens(line2,{token2,value2 or false})
					end
				else
					mac = trigger
					value = token
					if mac.action then -- simple action macro, pass token through.
						local state = lstate()
						for _,a in pairs(mac.action) do
							a(state)
						end
						mac = nil
					end
				end
            end
        end
        if mac then
            macname = value
            local subst = mac.subst
            local handled
            if mac.parms then
                local parm_values
                if not mac.parms.handle_parms then
                    line,token,value=get()
                    if token ~= '(' then error("expects parameters",line)
                    else -- grab macro parameters, separated by commas
                        parm_values = grab_parameters(')',',')
                    end
                else -- otherwise we must have a handler
                    if mac.handler then
                        parm_values = {mac.handler(lstate())}
                        if not parm_values then error("unclosed parameter list",line) end
                        handled = true
                    else error("requires parameter handler",line) end
                end
                if type(subst) == 'table' then -- a token list
                    if #parm_values ~= #mac.parms then
                        error("expects "..#mac.parms..
                               " parameters, received "..#parm_values,line)
                    end
                    subst = replace(mac.parms,parm_values,subst)
                elseif type(subst) == 'function' then
                    -- can receive arb. number of args
                    subst = subst(lstate(),unpack(parm_values))
                end
            end
            if mac.handler and not handled then
                -- if there's a handler, call it anyway, but it won't grab parms.
                mac.handler(lstate())
            end
            -- if the substitution was a function, we need to call it even
            -- if there were no parameters.
            if type(subst) == 'function' then
                subst = subst(lstate())
            end
            insert_tokens(line,subst)
        else
            local eof = token == '<eof>'
            if eof then reset() end
            if line_offs_reset then
                line = line_offs_reset
                line_offs_reset = nil
            end
            put(line,token,value)
            --- stop coroutine at eof
            if eof then return end
        end
    end
end

-- on EOF or error, NB to switch to INIT_FILTER for next source file,
function reset()
    if verbose==1 then print 'resetting...' end
    G.FILTER = INIT_FILTER
end

--- this version of require() can be used inside macros.
-- It disables the token filter temporarily.
-- @param m a module name (same as require())
function require(m)
    local F = G.FILTER
    G.FILTER = nil
    local ret = G.require (m)
    G.FILTER = F
    return ret
end

define('__include',{'file',handle_parms = grab_token},
    function (ls,arg)
        local res = require(arg[2])
        return type(res)=='string' and res or {}
    end
)

define('__def',{'defn',handle_parms = grab_token},
	function (ls,defn)
		define(defn[2])
		return {}
	end
)

define('__line',{'line',handle_parms = grab_token},
    function (ls,line)
        line_offs_reset = G.tonumber(line[2]) or error("expects a number")
        return {}
    end
)

-- macro 'stringizing' function
define('_STR',{'arg'},function(ls,arg)
	return string(value_of(arg))
end)


define('__dbg',{'level',handle_parms = grab_token},
    function (ls,level)
        verbose = G.tonumber(level[2]) or error("expects a number")
        return {}
    end
)


local filters = {}

function add_source_filter (fun)
	append(filters,fun)
end

function remove_source_filter (fun)
	remove(filters,fun)
end

-- we take the drastic step of overriding global loadstring, so that
-- reset() can be called.
G.loadstring = function(line,name)
    local f,err = global_loadstring(line,name)
    if not f then reset() end
    return f,err
end

function INIT_FILTER(get,source)
	for _,f in ipairs(filters) do
		local res = f(source)
		if res then
			local getter = get
			G.FILTER = function()
				local line,token,value = getter()
				if token == '<eof>' then reset() end
				return line,token,value
			end
			return
		end
	end
    G.FILTER=co_wrap(F)
    current_source = source
    if not verbose then
        G.FILTER(get,yield)
    else
		if verbose > 0 then print('source',source) end
        G.FILTER(function()
            local line,token,value = get()
            if verbose > 1 then print('<-',line,token,value) end
            return line,token,value
        end,
        function(line,token,value)
            if verbose > 0 then print('->',line,token,value) end
            yield(line,token,value)
        end)
    end
end

reset()

