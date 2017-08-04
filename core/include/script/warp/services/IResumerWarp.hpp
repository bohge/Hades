#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "services/IServicesSystem.h"
#include "services/IResumer.h"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	typedef eastl::pair<uint64, hc::ISession*> MachineUserData;
	static int IResumer_Pasue(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::IResumer* self)
	{
		self->Pasue();
		MachineUserData* md =
			static_cast<MachineUserData*>(vm->GetUserData());
		return md->second->PasueCoroutine();
	}
	//-------------------------------------------------------------------------------------------------------
	static int IResumer_Resume(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::IResumer* self, hc::IArguments* arg)
	{
		self->Resume(arg);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IResumer(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IResumer::RTTI());

		{
			FunctionRegister sf;
			sf.Function(&hc::IResumer_Pasue, "Pasue");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Arguments(&hc::IArguments::RTTI());
			sf.Function(&hc::IResumer_Resume, "Resume");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}