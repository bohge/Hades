#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "services/IServicesSystem.h"
#include "services/IAgent.h"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static void ServiceCleaner(void* rec, Object* obj)
	{
		if (NULL != obj)
		{
			IAgent* agent = static_cast<IAgent*>(obj);
			IServicesSystem::Instance()->RecycleBin(agent);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int IServicesSystem_ServiceFactory(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, IServicesSystem* self, StringBuffer* str)
	{
		eastl::string sstr(static_cast<const char*>(str->pBuffer), str->Length);
		IAgent* obj = self->ServiceFactory(sstr);
		//if (NULL != obj)//返回的时候会加载信息
		//{
		//	ClassRegister* clr = obj->GetClassInfomation();
		//	if (NULL != clr)
		//	{
		//		vm->ReferenceClass(clr);
		//	}
		//}
		res->SetValue(obj, self, &ServiceCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IServicesSystem_FixedServiceFactory(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, IServicesSystem* self, StringBuffer* str)
	{
		eastl::string sstr(static_cast<const char*>(str->pBuffer), str->Length);
		IAgent* obj = self->FixedServiceFactory(sstr);
		res->SetValue(obj, self, &ServiceCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IServicesSystem(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IServicesSystem::RTTI());
		{
			FunctionRegister sf;
			sf.Result(&IAgent::RTTI());
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::IServicesSystem_ServiceFactory, "Create");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result(&IAgent::RTTI());
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::IServicesSystem_FixedServiceFactory, "CreateFixed");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
		sys.SetNativeGlobal(Configure::NATIVE_LIBRARY_SYSTEM, IServicesSystem::RTTI().GetTypeName(), IServicesSystem::Instance());
	}
}