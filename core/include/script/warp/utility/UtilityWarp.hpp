#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/IScriptSystem.h"

namespace hc
{
	class NativeUtility : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(NativeUtility);
	};
	//-------------------------------------------------------------------------------------------------------
	static int Utility_GetIndexFromID(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint*res, uint64* id)
	{
		*res = Utility::GetIndexFormKey(*id);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Utility_GetUUIDFromID(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ushort* res, uint64* id)
	{
		*res = Utility::GetUUIDFormKey(*id);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Utility_HashCode(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, StringBuffer* str)
	{
		eastl::string sstr(static_cast<const char*>(str->pBuffer), str->Length);
		*res = Utility::HashCode(sstr);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Utility(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&NativeUtility::RTTI());

		{
			FunctionRegister sf;
			sf.Result(&UInt::RTTI());
			sf.Arguments(&UInt64::RTTI());
			sf.SetFunctionType(ScriptProperty::FT_STATIC);
			sf.Function(&hc::Utility_GetIndexFromID, "GetIndexFromID");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result(&UShort::RTTI());
			sf.Arguments(&UInt64::RTTI());
			sf.SetFunctionType(ScriptProperty::FT_STATIC);
			sf.Function(&hc::Utility_GetUUIDFromID, "GetUUIDFromID");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result(&UInt::RTTI());
			sf.Arguments(&String::RTTI());
			sf.SetFunctionType(ScriptProperty::FT_STATIC);
			sf.Function(&hc::Utility_HashCode, "HashCode");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}