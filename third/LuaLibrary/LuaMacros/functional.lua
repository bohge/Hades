-- functional.lua
__def 'for_(t,expr) for _idx,_ in ipairs(t) do expr end'
__def [[reduce_(t,init,expr) (function() local res = init
	   for _idx,_ in ipairs(t) do
	      res = res expr
	   end
	   return res end)()
	]]
__def 'sum_(t) reduce_(t,0,+ _)'
__def 'every_(t,expr) reduce_(t,true,and expr)'
__def 'any_(t,expr) reduce_(t,true,or expr)'

__def [[map_(t,expr)
	(function() local res = {}
	   for i,_ in ipairs(t) do
		res[i] = expr
	   end
	return res end)()
]]

__def 'concat_(t,delim,expr) table.concat(map_(t,expr),delim)'

__def [[filter_(t,condn)
	(function() local res = {}
	   for i,_ in ipairs(t) do
		if condn then res[#res+1] = _ end
	   end
	return res end)()
]]


t = {10,20,30}

for_(t,print(2*_)}

print('sum',sum_(t))

print('every',every_(t,_ > 20))

print('concat',concat_(t,',',tonumber(_)))

print 'filter'

s = filter_(t,_ > 10)
for_(s,print(_))


