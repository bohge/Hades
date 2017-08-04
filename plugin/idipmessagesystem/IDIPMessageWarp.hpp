#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "services/ScriptServices.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "services/ISession.h"
#include "core/Types.h"

#include "IDIPMessage.h"

namespace hms
{

	//-------------------------------------------------------------------------------------------------------
	static int IDIPMessage_GetResult(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, IDIPMessage* self)
	{
		if (!self->isValid())
		{
			hc::ScriptServices::MachineUserData* md =
				static_cast<hc::ScriptServices::MachineUserData*>(vm->GetUserData());
			md->second->GotError("Got null IMessage! After IMessage send, the IMessage will be reset !");
			*res = 0;
		}
		else
		{
			*res = self->GetResult();
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IDIPMessage(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&IDIPMessage::RTTI(), &hc::IMessage::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::UInt::RTTI());
			sf.Function(&hms::IDIPMessage_GetResult, "GetResult");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(name, sc);
	}
}