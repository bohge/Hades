__include 'macro.try'
local opt = arg[1]

function test ()
    try
		local val = 2
		if opt=='e1' then
			error 'bang!'
		end
		try
			if opt=='e2' then
				error 'whizz!'
			elseif opt=='a1' then
				opt = 'a1 handled'
				return 'hello',42
			end
			val = 2*val
		except e
			opt = 'e2 handled'
			print('fooked',e)
		end
		return 10,nil,val
	except e
		opt = 'e1 handled'
		print('error',e)
	end
	print 'only get here if error'
end

print(test())
print(opt)
