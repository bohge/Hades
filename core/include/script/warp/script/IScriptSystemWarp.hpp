#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/IScriptSystem.h"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static int IScriptSystem_Synchronize(hc::IVirtualMachine* vm, hc::FunctionRegister* func, IScriptSystem* self, IScriptInstance* instance)
	{
		self->Synchronize(vm, instance);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IScriptSystem(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IScriptSystem::RTTI());

		{
			FunctionRegister sf;
			sf.Arguments(&IScriptInstance::RTTI());
			sf.Function(&hc::IScriptSystem_Synchronize, "Synchronize");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
		sys.SetNativeGlobal(Configure::NATIVE_LIBRARY_SYSTEM, IScriptSystem::RTTI().GetTypeName(), IScriptSystem::Instance());
	}
}