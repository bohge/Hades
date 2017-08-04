#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "RemoteServicesSystem.h"
#include "services/IAgent.h"

namespace hrss
{
	//-------------------------------------------------------------------------------------------------------
	static int RemoteServicesSystem_SetServer(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RemoteServicesSystem* self, hc::ServerProxy* ser)
	{
		self->SetServerProxy(ser);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RemoteServicesSystem_SetHandler(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RemoteServicesSystem* self, hc::IAgent* agent)
	{
		self->SetHandler(agent);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RemoteServicesSystem_ConnectNode(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RemoteServicesSystem* self, int* ty, hc::StringBuffer* str)
	{
		eastl::string sstr(static_cast<const char*>(str->pBuffer), str->Length);
		self->ConnectNode((hc::ISocketSystem::SocketType)*ty, sstr);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RemoteServicesSystem_SendAgent(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RemoteServicesSystem* self, int* target, hc::IAgent* agent, hc::IMessage* msg)
	{
		self->SendAgent(*target, agent, msg);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RemoteServicesSystem_ScriptInitialized(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RemoteServicesSystem* self)
	{
		self->ScriptInitialized();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_RemoteServicesSystem(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister* psc = sys.GetNativeLibraryClass(name, RemoteServicesSystem::RTTI());
		
		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::IServer::RTTI());
			sf.Function(&RemoteServicesSystem_SetServer, "SetServer");
			psc->ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::IAgent::RTTI());
			sf.Function(&RemoteServicesSystem_SetHandler, "SetHandler");
			psc->ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::Int::RTTI(), &hc::String::RTTI());
			sf.Function(&RemoteServicesSystem_ConnectNode, "ConnectNode");
			psc->ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Function(&RemoteServicesSystem_ScriptInitialized, "ScriptInitialized");
			psc->ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::Int::RTTI(), &hc::IAgent::RTTI(), &hc::IMessage::RTTI());
			sf.Function(&RemoteServicesSystem_SendAgent, "SendAgent");
			psc->ClassFunction(sf);
		}
	}
}