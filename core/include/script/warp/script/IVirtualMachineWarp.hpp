#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"

namespace hc
{
	////-------------------------------------------------------------------------------------------------------
	//static void IVirtualMachine_GetInstance_Cleaner(void* rec, Object* obj)
	//{
	//	if (NULL != obj)
	//	{
	//		IVirtualMachine* host = static_cast<IVirtualMachine*>(rec);
	//		host->CacheInstance(static_cast<IScriptInstance*>(obj));
	//	}
	//}
	//-------------------------------------------------------------------------------------------------------
	static void IVirtualMachine_RegisteInstance_Cleaner(void* rec, Object* obj)
	{
		if (NULL != obj)
		{
			IVirtualMachine* host = static_cast<IVirtualMachine*>(rec);
			host->ReleaseScript(static_cast<IScriptInstance*>(obj));
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int IVirtualMachine_RegisteInstance(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IVirtualMachine* self, int* refinxe)
	{
		IScriptInstance* instance = self->RegisteInstance(*refinxe);
		res->SetValue(instance, self, &IVirtualMachine_RegisteInstance_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IVirtualMachine_RegisteInstanceName(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IVirtualMachine* self, int* refinxe, StringBuffer* name)
	{
		eastl::string sname(static_cast<const char*>(name->pBuffer), name->Length);
		IScriptInstance* instance = self->RegisteInstance(*refinxe, sname);
		res->SetValue(instance, self, &IVirtualMachine_RegisteInstance_Cleaner);
		return 1;
	}
	////-------------------------------------------------------------------------------------------------------
	//static int IVirtualMachine_GetInstance(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IVirtualMachine* self)
	//{
	//	IScriptInstance* instance = self->GetInstance();
	//	res->SetValue(instance, self, &IVirtualMachine_GetInstance_Cleaner);
	//	return 1;
	//}
	//-------------------------------------------------------------------------------------------------------
	static ClassRegister Register_IVirtualMachine()
	{
		ClassRegister sc;
		sc.ClassType(&IVirtualMachine::RTTI());

		{
			FunctionRegister sf;
			sf.Result( &IScriptInstance::RTTI());
			sf.Arguments(&Instance::RTTI());
			sf.Function(&hc::IVirtualMachine_RegisteInstance, "RegisteInstance");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IScriptInstance::RTTI());
			sf.Arguments(&Instance::RTTI(), &String::RTTI());
			sf.Function(&hc::IVirtualMachine_RegisteInstanceName, "RegisteInstance");
			sc.ClassFunction(sf);
		}
		//{
		//	FunctionRegister sf;
		//	sf.Result( &IScriptInstance::RTTI());
		//	sf.Function(&hc::IVirtualMachine_GetInstance, "GetInstance");
		//	sc.ClassFunction(sf);
		//}
		return sc;
	}
}