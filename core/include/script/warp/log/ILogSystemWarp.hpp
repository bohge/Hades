#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "log/ILogSystem.h"
#include "core/Types.h"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static int ILogSystem_Log(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ILogSystem* self, StringBuffer* str)
	{
		self->ScriptLog(static_cast<const char*>(str->pBuffer), str->Length);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ILogSystem_Warning(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ILogSystem* self, StringBuffer* str)
	{
		self->ScriptWarning(static_cast<const char*>(str->pBuffer), str->Length);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ILogSystem_Error(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ILogSystem* self, StringBuffer* str)
	{
		//printf("%s\r\n", str);
		//fflush(stdout);
		self->ScriptError(static_cast<const char*>(str->pBuffer), str->Length);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_ILogSystem(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&ILogSystem::RTTI());

		{
			FunctionRegister sf;
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::ILogSystem_Log, "Log");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::ILogSystem_Warning, "Warning");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::ILogSystem_Error, "Error");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
		sys.SetNativeGlobal(Configure::NATIVE_LIBRARY_SYSTEM, ILogSystem::RTTI().GetTypeName(), ILogSystem::Instance());
	}
}