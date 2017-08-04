#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "network/server/IServer.h"
#include "script/IScriptInstance.h"
#include "ClusternodeApplication.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "network/Message.h"
#include "services/IAgent.h"
#include "core/Types.h"



namespace hsna
{
	//-------------------------------------------------------------------------------------------------------
	static int ClusternodeApplication_ScriptInitialized(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ClusternodeApplication* self)
	{
		self->ScriptInitialized();
		return 0;
	}	
	//-------------------------------------------------------------------------------------------------------
	static int ClusternodeApplication_SetClientHandle(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ClusternodeApplication* self, uint64* gid, hc::IAgent* agent)
	{
		self->SetClientHandle(*gid, agent);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int ClusternodeApplication_RegisterTimer(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ClusternodeApplication* self, hc::IAgent* agent, uint* freq, const char* name)
	{
		self->RegisterTimer(agent, *freq, name);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_ClusternodeApplication(hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&ClusternodeApplication::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Function(&ClusternodeApplication_ScriptInitialized, "ScriptInitialized");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI(), &hc::IAgent::RTTI());
			sf.Function(&ClusternodeApplication_SetClientHandle, "SetClientHandle");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::IAgent::RTTI(), &hc::UInt::RTTI(), &hc::String::RTTI());
			sf.Function(&ClusternodeApplication_RegisterTimer, "RegisterTimer");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary("application", sc);
	}
}