#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "network/server/ServerProxy.h"
#include "network/message/IMessage.h"
#include "services/ISession.h"
#include "services/IAgent.h"
#include "core/Types.h"



namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_Start(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, int* count, int* port)
	{
		self->Start(*count, *port);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_Send(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, uint64* gid, IMessage* msg)
	{
		if (msg && !msg->isValid())
		{
			ScriptServices::MachineUserData* md =
				static_cast<ScriptServices::MachineUserData*>(vm->GetUserData());
			md->second->GotError("Got null message! After message send, the message will be reset, so may you send a same message twice");
		}
		else
		{
			self->SendBuffer(*gid, msg);
		}
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_Disconnect(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, uint64* gid)
	{
		self->Disconnect(*gid);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_Broadcast(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, uint64* gid, IMessage* msg)
	{
		self->Broadcast(*gid, msg);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_GetPort(IVirtualMachine* vm, FunctionRegister* func, int* res, ServerProxy* self)
	{
		*res = self->GetPort();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_GetServerAddress(IVirtualMachine* vm, FunctionRegister* func, eastl::string* res, ServerProxy* self)
	{
		*res = self->GetServerAddress();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_SetClientHandle(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, uint64* gid, IAgent* agent)
	{
		self->SetClientHandle(*gid, agent);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_PushDefualtHandler(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, IAgent* agent)
	{
		self->PushDefualtHandler(agent);
		return 0;
	}
	////-------------------------------------------------------------------------------------------------------
	//static int ServerProxy_SetClientSpecific(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, uint64* gid, uint64* sgid)
	//{
	//	self->SetSpecific(*gid, *sgid);
	//	return 0;
	//}
	////-------------------------------------------------------------------------------------------------------
	//static int ServerProxy_GetClientSpecific(IVirtualMachine* vm, FunctionRegister* func, uint64* res, ServerProxy* self, uint64* gid)
	//{
	//	*res = self->GetSpecific(*gid);
	//	return 1;
	//}
	//-------------------------------------------------------------------------------------------------------
	static int ServerProxy_SetHeartBeat(IVirtualMachine* vm, FunctionRegister* func, ServerProxy* self, uint* idle, uint* inter, uint* count)
	{
		self->SetHeartBeat(*count, *inter, *count);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_ServerProxy(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&ServerProxy::RTTI());

		{
			FunctionRegister sf;
			sf.Arguments(&Int::RTTI(), &Int::RTTI());
			sf.Function(&ServerProxy_Start, "Start");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&UInt64::RTTI(), &IMessage::RTTI());
			sf.Function(&ServerProxy_Send, "Send");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&UInt64::RTTI());
			sf.Function(&ServerProxy_Disconnect, "Disconnect");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &Int::RTTI());
			sf.Function(&ServerProxy_GetPort, "GetPort");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &String::RTTI());
			sf.Function(&ServerProxy_GetServerAddress, "GetServerAddress");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&IAgent::RTTI());
			sf.Function(&ServerProxy_PushDefualtHandler, "PushDefualtHandler");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&UInt64::RTTI(), &IAgent::RTTI());
			sf.Function(&ServerProxy_SetClientHandle, "SetClientHandle");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Arguments(&UInt::RTTI(), &UInt::RTTI(), &UInt::RTTI());
			sf.Function(&ServerProxy_SetHeartBeat, "SetHeartBeat");
			sc.ClassFunction(sf);
		}
		//{
		//	FunctionRegister sf;
		//	sf.Arguments(&UInt64::RTTI(), &UInt64::RTTI());
		//	sf.Function(&ServerProxy_SetClientSpecific, "SetClientSpecific");
		//	sc.ClassFunction(sf);
		//}
		//{
		//	FunctionRegister sf;
		//	sf.Result( &UInt64::RTTI());
		//	sf.Arguments(&UInt64::RTTI());
		//	sf.Function(&ServerProxy_GetClientSpecific, "GetClientSpecific");
		//	sc.ClassFunction(sf);
		//}
		//{
		//	FunctionRegister sf;
		//	sf.Arguments(&UInt64::RTTI(), &IMessage::RTTI());
		//	sf.Function(&ServerProxy_Broadcast, "Broadcast");
		//	sc.ClassFunction(sf);
		//}
		sc.ClassConstant(CallbackType::SCT_CONNECT, "SCT_CONNECT");
		sc.ClassConstant(CallbackType::SCT_REQUEST, "SCT_REQUEST");
		sc.ClassConstant(CallbackType::SCT_DISCONNECT, "SCT_DISCONNECT");
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}