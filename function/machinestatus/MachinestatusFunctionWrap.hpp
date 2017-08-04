#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptSystem.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"
#ifdef _PLATFORM_WINDOWS
#include "MachinestatusFunctionPlugin.h"
#else
#include "MachinestatusFunctionPluginLinux.h"
#endif
#include "core/Log.h"

#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"
#include "script/IFunctionInvoker.h"

namespace hmsfp
{

	//-------------------------------------------------------------------------------------------------------
	static int MachinestatusFunction_GetFreePhysicalmemory(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, MachinestatusFunctionPlugin* self)
	{
		*res = self->GetFreePhysicalmemory();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int MachinestatusFunction_GetTotalPhysicalmemory(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, MachinestatusFunctionPlugin* self)
	{
		*res = self->GetTotalPhysicalmemory();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int MachinestatusFunction_GetFreeVirtualmemory(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, MachinestatusFunctionPlugin* self)
	{
		*res = self->GetFreeVirtualmemory();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int MachinestatusFunction_GetTotalVirtualmemory(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, MachinestatusFunctionPlugin* self)
	{
		*res = self->GetTotalVirtualmemory();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int MachinestatusFunction_GetCpuBusypercentage(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, MachinestatusFunctionPlugin* self)
	{
		*res = self->GetCpuBusypercentage();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int MachinestatusFunction_GetNetcardReceiveSpeed(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, MachinestatusFunctionPlugin* self)
	{
		*res = self->GetNetcardReceiveSpeed();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int MachinestatusFunction_GetNetcardSendSpeed(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, MachinestatusFunctionPlugin* self)
	{
		*res = self->GetNetcardSendSpeed();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_MachinestatusFunction(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&MachinestatusFunctionPlugin::RTTI());
		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Int::RTTI());
			sf.Function(&hmsfp::MachinestatusFunction_GetFreePhysicalmemory, "GetFreePhysicalmemory");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Int::RTTI());
			sf.Function(&hmsfp::MachinestatusFunction_GetTotalPhysicalmemory, "GetTotalPhysicalmemory");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Int::RTTI());
			sf.Function(&hmsfp::MachinestatusFunction_GetFreeVirtualmemory, "GetFreeVirtualmemory");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Int::RTTI());
			sf.Function(&hmsfp::MachinestatusFunction_GetTotalVirtualmemory, "GetTotalVirtualmemory");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Float::RTTI());
			sf.Function(&hmsfp::MachinestatusFunction_GetCpuBusypercentage, "GetCpuBusypercentage");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Float::RTTI());
			sf.Function(&hmsfp::MachinestatusFunction_GetNetcardReceiveSpeed, "GetNetcardReceiveSpeed");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Float::RTTI());
			sf.Function(&hmsfp::MachinestatusFunction_GetNetcardSendSpeed, "GetNetcardSendSpeed");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(name, sc);
		sys.SetNativeGlobal(name, MachinestatusFunctionPlugin::RTTI().GetTypeName(), MachinestatusFunctionPlugin::Instance());
	}
}