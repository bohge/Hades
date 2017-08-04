#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "network/socket/SocketProxy.h"
#include "network/message/IMessage.h"
#include "services/ISession.h"
#include "core/Types.h"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static int SocketProxy_Send(hc::IVirtualMachine* vm, hc::FunctionRegister* func, SocketProxy* self, IMessage* msg)
	{
		if (msg && !msg->isValid())
		{
			ScriptServices::MachineUserData* md =
				static_cast<ScriptServices::MachineUserData*>(vm->GetUserData());
			md->second->GotError("Got null message! After message send, the message will be reset, so may you send a same message twice");
		}
		else
		{
			self->Send(msg);
		}
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SocketProxy_Connect(hc::IVirtualMachine* vm, hc::FunctionRegister* func, SocketProxy* self, StringBuffer* str)
	{
		eastl::string sstr(static_cast<const char*>(str->pBuffer), str->Length);
		self->Connect(sstr, NULL);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SocketProxy_Disconnect(hc::IVirtualMachine* vm, hc::FunctionRegister* func, SocketProxy* self)
	{
		self->Disconnect();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SocketProxy_GetSocketAddress(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, SocketProxy* self)
	{
		*res = self->GetSocketAddress();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SocketProxy_PushDefualtHandler(IVirtualMachine* vm, FunctionRegister* func, SocketProxy* self, IAgent* agent)
	{
		self->PushDefualtHandler(agent);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_SocketProxy(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&SocketProxy::RTTI());

		{
			FunctionRegister sf;
			sf.Arguments(&IMessage::RTTI());
			sf.Function(&hc::SocketProxy_Send, "Send");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::SocketProxy_Connect, "Connect");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Function(&hc::SocketProxy_Disconnect, "Disconnect");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result(&String::RTTI());
			sf.Function(&hc::SocketProxy_GetSocketAddress, "GetSocketAddress");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&IAgent::RTTI());
			sf.Function(&SocketProxy_PushDefualtHandler, "PushDefualtHandler");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}