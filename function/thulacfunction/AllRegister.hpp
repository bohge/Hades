#include "ThulacFunctionWrap.hpp"

namespace htf
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_All(const eastl::string& name, hc::IScriptSystem& sys)
	{
		Register_TulacFunction(name, sys);
	}
}