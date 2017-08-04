__include 'macro.case'
-- a case statement for Lua
function testcase(k)
	case (k)
	  is 10,11: return 1
	  is 12: return 2
	  is 13 .. 16: return 3
	  else return 4
	endcase
end

assert(testcase(1)==4)
assert(testcase(13)==3)
assert(testcase(12)==2)
assert(testcase(11)==1)
assert(testcase(15)==3)
print 'fine!'


