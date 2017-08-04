#include "script/IScriptSystem.h"
#include "ThreadData.h"




namespace hc
{
	RTTI_TYPE_IMPLEMENT(ThreadData)
	//---------------------------------------------------------------------------------------------------------
	ThreadData::ThreadData(void)
	{
		InputArgument = IScriptSystem::Instance()->AllocArguments();
		ResultArgument = IScriptSystem::Instance()->AllocArguments();
	}
	//---------------------------------------------------------------------------------------------------------
	ThreadData::~ThreadData(void)
	{
		IScriptSystem::Instance()->FreeArguments(InputArgument);
		IScriptSystem::Instance()->FreeArguments(ResultArgument);
	}
}
