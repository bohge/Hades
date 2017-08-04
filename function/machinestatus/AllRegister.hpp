#include "MachinestatusFunctionWrap.hpp"

namespace hmsfp
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_All(const eastl::string& name, hc::IScriptSystem& sys)
	{
		Register_MachinestatusFunction(name, sys);
	}
}