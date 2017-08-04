#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "network/server/IServerSystem.h"
#include "network/server/ServerProxy.h"
#include "network/message/IMessage.h"
#include "network/server/IServer.h"
#include "core/Configure.h"
#include "core/Types.h"


namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static void IServerCleaner(void* rec, Object* obj)
	{
		ServerProxy* ser = static_cast<ServerProxy*>(obj);
		IServerSystem::Instance()->RecycleBin(ser);
	}
	//-------------------------------------------------------------------------------------------------------
	static int IServerSystem_ServerFactory(IVirtualMachine* vm, FunctionRegister* func, ScriptValue* res, IServerSystem* self, int* type)
	{
		ServerProxy* ser = self->ServerFactory((IServerSystem::ServerType)*type);
		res->SetValue(ser, IServerSystem::Instance(), &IServerCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IServerSystem(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IServerSystem::RTTI());

		{
			FunctionRegister sf;
			sf.Arguments(&Int::RTTI());
			sf.Result(&ServerProxy::RTTI());
			sf.Function(&hc::IServerSystem_ServerFactory, "Create");
			sc.ClassFunction(sf);
		}

		sc.ClassConstant(IServerSystem::ST_OPENSSL_TLS, "ST_OPENSSL_TLS");
		sc.ClassConstant(IServerSystem::ST_COMMON_TL, "ST_COMMON_TL");
		sys.SetNativeGlobal(Configure::NATIVE_LIBRARY_SYSTEM, IServerSystem::RTTI().GetTypeName(), IServerSystem::Instance());

		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}