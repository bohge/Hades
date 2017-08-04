#include "LibeventHttpserverWrap.hpp"

namespace hlhsp
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_All(const eastl::string& name, hc::IScriptSystem& sys)
	{
		Register_LibeventHttpserverFunction(name, sys);
	}
}