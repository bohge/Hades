#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "serialize/ISerializeSystem.h"
#include "services/IServicesSystem.h"
#include "services/IResumer.h"

namespace hc
{
	class Context : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(Context);
	};
	//-------------------------------------------------------------------------------------------------------
	void ResumerCleaner(void* rec, Object* obj)
	{
		IResumer* res = static_cast<IResumer*>(obj);
		ISerializeSystem::Instance()->FreeSerializable(res);
	}
	//-------------------------------------------------------------------------------------------------------
	static int Context_GetServiceContext(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		IResumer* obj = IServicesSystem::Instance()->AllocResumer(vm);
		res->SetValue(obj, NULL, &ResumerCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Context(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&Context::RTTI());
		{
			FunctionRegister sf;
			sf.Result(&IResumer::RTTI());
			sf.SetFunctionType(ScriptProperty::FT_STATIC);
			sf.Function(&hc::Context_GetServiceContext, "GetServiceContext");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}