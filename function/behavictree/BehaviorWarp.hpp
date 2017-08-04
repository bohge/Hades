#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "Behavior.h"

namespace hbtf
{
	//-------------------------------------------------------------------------------------------------------
	static int Behavior_Update(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, Behavior* self, hc::IScriptInstance* instance)
	{
		*res = self->Update(instance);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Behavior(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&Behavior::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&hc::IScriptInstance::RTTI());
			sf.Function(&Behavior_Update, "Update");
			sc.ClassFunction(sf);
		}

		sc.ClassConstant(Behavior::BT_INVALID, "BT_INVALID");
		sc.ClassConstant(Behavior::BT_SUCCESS, "BT_SUCCESS");
		sc.ClassConstant(Behavior::BT_FAILURE, "BT_FAILURE");
		sc.ClassConstant(Behavior::BT_RUNNING, "BT_RUNNING");

		sys.SetNativeLibrary(name, sc);
	}
}