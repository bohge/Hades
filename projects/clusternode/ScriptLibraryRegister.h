#pragma once
#include "ClusternodeApplicationWarp.h"


namespace hsna
{
	//-------------------------------------------------------------------------------------------------------
	static void ScriptLibraryRegister()
	{
		hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
		Register_ClusternodeApplication(*sys);
	}
}