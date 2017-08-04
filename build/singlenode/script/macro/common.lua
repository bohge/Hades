-- macro-base.lua
-- Useful basic C-like macros
local define = macro.define
local mstring = macro.string
local mname = macro.name
local value_of = macro.value_of

define('__FILE__',nil,function(ls) return mstring(ls.source) end)
define('__LINE__',nil,function(ls) return mstring(ls.line) end)

-- macro 'token concatenation' function
define('_CAT',{'x','y'},function(ls,x,y)
	return mname(value_of(x)..value_of(y))
end)

define('ASSERT',{'x'},'assert(x,_STR(x))')

