#include "RemoteServicesSystemWarp.hpp"
#include "RemoteResumerWarp.hpp"



namespace hrss
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_All(const eastl::string& name, hc::IScriptSystem& sys)
	{
		Register_RemoteServicesSystem(name, sys);
		Register_RemoteResumer(name, sys);
	}
}