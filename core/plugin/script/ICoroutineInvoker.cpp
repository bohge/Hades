#include "script/ICoroutineInvoker.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(ICoroutineInvoker);
	//-------------------------------------------------------------------------------------------------------
	ICoroutineInvoker::ICoroutineInvoker(void)
		:m_Status(CS_OK)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	ICoroutineInvoker::~ICoroutineInvoker(void)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void ICoroutineInvoker::SetupResume()
	{
		ScriptProperty::Arguments args;
		_SetupResume(args);
	}
	//-------------------------------------------------------------------------------------------------------
	void ICoroutineInvoker::SetupResume(uint arg1)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		_SetupResume(args);
	}
	//-------------------------------------------------------------------------------------------------------
	void ICoroutineInvoker::SetupResume(uint arg1, uint arg2)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		_SetupResume(args);
	}
	//-------------------------------------------------------------------------------------------------------
	void ICoroutineInvoker::SetupResume(uint arg1, uint arg2, uint arg3)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		_SetupResume(args);
	}
	//-------------------------------------------------------------------------------------------------------
	void ICoroutineInvoker::SetupResume(uint arg1, uint arg2, uint arg3, uint arg4)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		args.push_back(arg4);
		_SetupResume(args);
	}
}