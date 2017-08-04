__include examples

print(('hello'):sub(1,4))

function eval(fn) print(fn(42)) end

--assert((L(x,x-1))(42)),41)

assert((\x(x-1))(42),41)
assert((\x,y(x+y))(40,1),41)

fn = \x(\y(x+y))
fn = fn(40)
assert(fn(1)==41)

T B

import(math, sin cos)

print(sin(1)/cos(1))


function call(c) c() end

call begin
	print 'starting...'
	if defined then print 'dolly'
	else call begin
			print 'ending'
		end
	end
end

call ::
	print 'yay!'
end

t = qw(alice john joanie function < ? fred)
for _,name in ipairs(t) do
	print(name)
end



