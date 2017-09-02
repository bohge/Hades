#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "log/ILogSystem.h"
#include "core/Types.h"
#include "serialize/ISerializeSystem.h"
#include "timer/ITimerSystem.h"
#include "services/IAgent.h"


namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static int ITimerSystem_GetTimevalue(hc::IVirtualMachine* vm, hc::FunctionRegister* func, double* res, ITimerSystem* self)
	{
		*res = self->GetTimevalue();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ITimerSystem_RegisterTimer(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ITimerSystem* self, hc::IAgent* agent, float* freq, hc::StringBuffer* str)
	{
		eastl::string sstr(static_cast<const char*>(str->pBuffer), str->Length);
		self->RegisterTimer(agent, *freq, sstr);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_ITimerSystem(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&ITimerSystem::RTTI());

		{
			FunctionRegister sf;
			sf.Result(&Double::RTTI());
			sf.Function(&hc::ITimerSystem_GetTimevalue, "GetTimevalue");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&IAgent::RTTI(), &Float::RTTI(), &String::RTTI());
			sf.Function(&ITimerSystem_RegisterTimer, "RegisterTimer");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
		sys.SetNativeGlobal(Configure::NATIVE_LIBRARY_SYSTEM, ITimerSystem::RTTI().GetTypeName(), ITimerSystem::Instance());
	}
}