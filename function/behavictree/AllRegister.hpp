#include "BehavictreeWarp.hpp"
#include "BehaviorWarp.hpp"




namespace hbtf
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_All(const eastl::string& name, hc::IScriptSystem& sys)
	{
		Register_BehavictreePlugin(name, sys);
		Register_Behavior(name, sys);
	}
}