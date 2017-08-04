#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "network/server/IServer.h"
#include "script/IScriptInstance.h"
#include "SinglenodeApplication.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "network/Messagepool.h"
#include "network/Message.h"
#include "core/Types.h"



namespace hsna
{
	//-------------------------------------------------------------------------------------------------------
	static int SinglenodeApplication_OnReciveMessage(hc::IVirtualMachine* vm, hc::FunctionRegister* func, SinglenodeApplication* self, uint64* id, uint* type, hc::Message* msg)
	{
		hc::Message* newmsg = hc::Messagepool::Instance()->AllocMessage();
		*newmsg = *msg;//全部脚本传递进来的msg需要拷贝复制一次
		self->OnReciveMessage(*id, (hc::CallbackType)*type, newmsg);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SinglenodeApplication_ScriptInitialized(hc::IVirtualMachine* vm, hc::FunctionRegister* func, SinglenodeApplication* self)
	{
		self->ScriptInitialized();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SinglenodeApplication_RegisterTimer(hc::IVirtualMachine* vm, hc::FunctionRegister* func, SinglenodeApplication* self, float* interval, int* funcid)
	{
		self->RegisterTimer(*interval, *funcid);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_SinglenodeApplication(hc::IScriptSystem& sys, hc::IVirtualMachine& vm)
	{
		hc::ClassRegister sc;
		sc.ClassType(&SinglenodeApplication::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI(), &hc::UInt::RTTI(), &hc::Message::RTTI());
			sf.Function(&SinglenodeApplication_OnReciveMessage, "DispatchRequest");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Function(&SinglenodeApplication_ScriptInitialized, "ScriptInitialized");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::Float::RTTI(), &hc::Function::RTTI());
			sf.Function(&SinglenodeApplication_RegisterTimer, "RegisterTimer");
			sc.ClassFunction(sf);
		}
		vm.RegisterClass(sc);
	}
}