-- Based on the bare-bones luac in the Lua test directory
-- This allows for preprocessing modules to be loaded if necessary!
-- usage: lua luac.lua [ -x lib ] file.lua out.luac
function err(msg,code)
	io.stderr:write(msg,'\n')
	os.exit(code or 1)
end

function usage()
	err("usage: lua luac.lua file.lua out.luac")
end

if #arg < 2 then usage() end
i = 1
first = true
while i <= #arg do
	if arg[i] == '-x' then
		i = i + 1
		if i > #arg or arg[i]:sub(1,1) == '-' then err("expecting module name after -x") end
		require (arg[i])
	else
		if first then infile = arg[i] else outfile = arg[i] end
		first = false
	end
	i = i + 1
end

require 'macro'

if not infile or not outfile then usage() end

f=io.open(outfile,"wb")
if not f then err("cannot open '"..outfile.."' for writing") end
chunk,msg = loadfile(infile)
if not chunk then err(msg) end
f:write(string.dump(chunk))
f:close()
