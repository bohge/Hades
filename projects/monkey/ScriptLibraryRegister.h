#pragma once
#include "RequestHandleWarp.h"


namespace hmka
{
	//-------------------------------------------------------------------------------------------------------
	static void ScriptLibraryRegister()
	{
		hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
		Register_RequestHandle(*sys);
	}
}