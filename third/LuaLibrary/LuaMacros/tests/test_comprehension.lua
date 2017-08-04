require 'dump'
__include 'macro.listc'

print_list(L{i for i=1,5})

print_list(L{ L{j for j = 1,5} for i = 1,5})

f = io.open('test_comprehension.lua')

l = L{ line for line in f:lines() }
print(#l)

f:close()


