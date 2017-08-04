#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/IFunctionInvoker.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"


namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_IFunctionInvoker(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IFunctionInvoker::RTTI());
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}