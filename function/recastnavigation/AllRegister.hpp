#include "RecastnavigationWarp.hpp"
#include "NavigationQueryWarp.hpp"




namespace hrnf
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_All(const eastl::string& name, hc::IScriptSystem& sys)
	{
		Register_Recastnavigation(name, sys);
		Register_NavigationQuery(name, sys);
	}
}