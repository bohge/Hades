#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "network/socket/ISocketSystem.h"
#include "network/socket/SocketProxy.h"
#include "network/message/IMessage.h"
#include "core/Configure.h"
#include "core/Types.h"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static void ISocketCleaner(void* rec, Object* obj)
	{
		SocketProxy* sk = static_cast<SocketProxy*>(obj);
		ISocketSystem::Instance()->RecycleBin(sk);
	}
	//-------------------------------------------------------------------------------------------------------
	static int ISocketSystem_SocketFactory(IVirtualMachine* vm, FunctionRegister* func, ScriptValue* res, ISocketSystem* self, int* type)
	{
		SocketProxy* sk = self->SocketFactory((ISocketSystem::SocketType)*type);
		res->SetValue(sk, ISocketSystem::Instance(), &ISocketCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_ISocketSystem(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&ISocketSystem::RTTI());

		{
			FunctionRegister sf;
			sf.Result(&SocketProxy::RTTI());
			sf.Arguments(&Int::RTTI());
			sf.Function(&ISocketSystem_SocketFactory, "Create");
			sc.ClassFunction(sf);
		}

		sc.ClassConstant(ISocketSystem::ST_OPENSSL_TLS, "ST_OPENSSL_TLS");
		sc.ClassConstant(ISocketSystem::ST_COMMON_TL, "ST_COMMON_TL");
		sys.SetNativeGlobal(Configure::NATIVE_LIBRARY_SYSTEM, ISocketSystem::RTTI().GetTypeName(), ISocketSystem::Instance());

		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}