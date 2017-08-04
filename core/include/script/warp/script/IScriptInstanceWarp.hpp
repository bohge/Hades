#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"
#include "script/IFunctionInvoker.h"

namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static void IScriptInstance_SignFunciton_Cleaner(void* rec, Object* obj)
	{
		static_cast<IScriptInstance*>(rec)->UnsignInvoker(static_cast<IFunctionInvoker*>(obj));
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFunciton0(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* fid)
	{
		IFunctionInvoker* invoker = self->SignFunciton(*fid);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFunciton1(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* fid, int* arg1)
	{
		IFunctionInvoker* invoker = self->SignFunciton(*fid, *arg1);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFunciton2(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* fid, int* arg1, int* arg2)
	{
		IFunctionInvoker* invoker = self->SignFunciton(*fid, *arg1, *arg2);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFunciton3(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* fid, int* arg1, int* arg2, int* arg3)
	{
		IFunctionInvoker* invoker = self->SignFunciton(*fid, *arg1, *arg2, *arg3);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFunciton4(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* rut, int* fid, int* arg1, int* arg2, int* arg3, int* arg4)
	{
		IFunctionInvoker* invoker = self->SignFuncitonWithReturn(*fid, *arg1, *arg2, *arg3, *arg4);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFuncitonWithReturn0(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* rut, int* fid)
	{
		IFunctionInvoker* invoker = self->SignFuncitonWithReturn(*rut, *fid);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFuncitonWithReturn1(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* rut, int* fid, int* arg1)
	{
		IFunctionInvoker* invoker = self->SignFuncitonWithReturn(*rut, *fid, *arg1);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFuncitonWithReturn2(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* rut, int* fid, int* arg1, int* arg2)
	{
		IFunctionInvoker* invoker = self->SignFuncitonWithReturn(*rut, *fid, *arg1, *arg2);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFuncitonWithReturn3(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* rut, int* fid, int* arg1, int* arg2, int* arg3)
	{
		IFunctionInvoker* invoker = self->SignFuncitonWithReturn(*rut, *fid, *arg1, *arg2, *arg3);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_SignFuncitonWithReturn4(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ScriptValue* res, IScriptInstance* self, int* rut, int* fid, int* arg1, int* arg2, int* arg3, int* arg4)
	{
		IFunctionInvoker* invoker = self->SignFuncitonWithReturn(*rut, *fid, *arg1, *arg2, *arg3, *arg4);
		res->SetValue(invoker, self, &IScriptInstance_SignFunciton_Cleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IScriptInstance_GetScriptInstance(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, IScriptInstance* self)
	{
		*res = self->GetScriptInstance();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IScriptInstance(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IScriptInstance::RTTI());

		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Function::RTTI());
			sf.Function(&hc::IScriptInstance_SignFunciton0, "SignFunciton");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Function::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFunciton1, "SignFunciton");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Function::RTTI(), &Int::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFunciton2, "SignFunciton");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Function::RTTI(), &Int::RTTI(), &Int::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFunciton3, "SignFunciton");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Function::RTTI(), &Int::RTTI(), &Int::RTTI(), &Int::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFunciton4, "SignFunciton");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Int::RTTI(), &Function::RTTI());
			sf.Function(&hc::IScriptInstance_SignFuncitonWithReturn0, "SignFuncitonWithReturn");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Int::RTTI(), &Function::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFuncitonWithReturn1, "SignFuncitonWithReturn");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Int::RTTI(), &Function::RTTI(), &Int::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFuncitonWithReturn2, "SignFuncitonWithReturn");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Int::RTTI(), &Function::RTTI(), &Int::RTTI(), &Int::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFuncitonWithReturn3, "SignFuncitonWithReturn");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result( &IFunctionInvoker::RTTI());
			sf.Arguments(&Int::RTTI(), &Function::RTTI(), &Int::RTTI(), &Int::RTTI(), &Int::RTTI(), &Int::RTTI());
			sf.Function(&hc::IScriptInstance_SignFuncitonWithReturn4, "SignFuncitonWithReturn");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result( &Instance::RTTI());
			sf.Function(&hc::IScriptInstance_GetScriptInstance, "GetScriptInstance");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}