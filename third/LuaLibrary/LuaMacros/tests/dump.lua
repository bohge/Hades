function dump_list(t)
	io.write '{'
	for i,v in ipairs(t) do
		if type(v) == 'table' then
			dump_list(v)
		else
			io.write(v,' ')
		end
	end
	io.write '}'
end

function print_list(t)
	dump_list(t)
	print()
end
