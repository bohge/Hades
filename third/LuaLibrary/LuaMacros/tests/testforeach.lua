-- testforeach.lua
__include 'macro.foreach'

foreach k in {10,20,30} do print(k) end
print '---'
t = {'one','two','three','four','five'}
foreach s in t if s:find '^t' do
	print(s)
end

