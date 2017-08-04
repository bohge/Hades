nativelibrary("nativesystem");

monkey = {};

g_Socket = nil;

function monkey.printHello(cc)
	print(cc)
	local msg = Message();
	g_Socket:Connect("10.12.26.152:9999");
	print("lua Connect done");
	g_Socket:Send(msg);
    print("lua done");
end