//#pragma once
//#include "script/FunctionRegister.h"
//#include "script/IVirtualMachine.h"
//#include "network/server/IServer.h"
//#include "script/IScriptInstance.h"
//#include "ServicesnodeApplication.h"
//#include "script/ClassRegister.h"
//#include "script/ScriptValue.h"
//#include "network/Message.h"
//#include "services/IAgent.h"
//#include "core/Types.h"
//
//
//
//namespace hsna
//{
//	//-------------------------------------------------------------------------------------------------------
//	static int ServicesnodeApplication_SetListenner(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ServicesnodeApplication* self, hc::IServer* ser)
//	{
//		self->SetListenner(ser);
//		return 0;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	static int ServicesnodeApplication_PushDefualtHandler(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ServicesnodeApplication* self, hc::IAgent* agent)
//	{
//		self->PushDefualtHandler(agent);
//		return 0;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	static int ServicesnodeApplication_ScriptInitialized(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ServicesnodeApplication* self)
//	{
//		self->ScriptInitialized();
//		return 0;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	static int ServicesnodeApplication_SetClientHandle(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ServicesnodeApplication* self, uint64* gid, hc::IAgent* agent)
//	{
//		self->SetClientHandle(*gid, agent);
//		return 0;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	static int ServicesnodeApplication_SendMessage(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ServicesnodeApplication* self, uint64* gid, hc::Message* mes)
//	{
//		self->SendBuffer(*gid, mes);
//		return 0;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	static int ServicesnodeApplication_Disconnect(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ServicesnodeApplication* self, uint64* gid)
//	{
//		self->Disconnect(*gid);
//		return 0;
//	}
//	//-------------------------------------------------------------------------------------------------------
//	static void Register_ServicesnodeApplication(hc::IScriptSystem& sys)
//	{
//		hc::ClassRegister sc;
//		sc.ClassType(&ServicesnodeApplication::RTTI());
//		
//
//		{
//			hc::FunctionRegister sf;
//			sf.Arguments(&hc::UInt64::RTTI(), &hc::Message::RTTI());
//			sf.Function(&ServicesnodeApplication_SendMessage, "SendMessage");
//			sc.ClassFunction(sf);
//		}
//		{
//			hc::FunctionRegister sf;
//			sf.Arguments(&hc::UInt64::RTTI());
//			sf.Function(&ServicesnodeApplication_Disconnect, "Disconnect");
//			sc.ClassFunction(sf);
//		}
//		{
//			hc::FunctionRegister sf;
//			sf.Function(&ServicesnodeApplication_ScriptInitialized, "ScriptInitialized");
//			sc.ClassFunction(sf);
//		}
//		{
//			hc::FunctionRegister sf;
//			sf.Arguments(&hc::IAgent::RTTI());
//			sf.Function(&ServicesnodeApplication_PushDefualtHandler, "PushDefualtHandler");
//			sc.ClassFunction(sf);
//		}
//		{
//			hc::FunctionRegister sf;
//			sf.Arguments(&hc::IServer::RTTI());
//			sf.Function(&ServicesnodeApplication_SetListenner, "SetListenner");
//			sc.ClassFunction(sf);
//		}
//		{
//			hc::FunctionRegister sf;
//			sf.Arguments(&hc::UInt64::RTTI(), &hc::IAgent::RTTI());
//			sf.Function(&ServicesnodeApplication_SetClientHandle, "SetClientHandle");
//			sc.ClassFunction(sf);
//		}
//
//		sys.SetNativeLibrary("application", sc);
//	}
//}