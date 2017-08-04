#pragma once
#include "SinglenodeApplicationWarp.h"
#include "RequestHandleWarp.h"


namespace hsna
{
	static void Register_All(hc::IScriptSystem& sys, hc::IVirtualMachine& vm)
	{
		Register_RequestHandle(sys, vm);
		Register_SinglenodeApplication(sys, vm);
	}
	//-------------------------------------------------------------------------------------------------------
	static void ScriptLibraryRegister()
	{
		hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
		sys->SetNativeLibrary("application", &Register_All);
	}
}