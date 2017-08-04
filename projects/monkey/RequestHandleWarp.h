#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "network/Message.h"
#include "MonkeyHandle.h"
#include "core/Types.h"


namespace hmka
{
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_CheckReceive(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, MonkeyHandle* self)
	{
		hc::Message* msg = self->CheckReceive();
		res->SetValue(msg, self, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_RequestHandle(hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&MonkeyHandle::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Message::RTTI());
			sf.Function(&RequestHandle_CheckReceive, "CheckReceive");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary("application", sc);
	}
}