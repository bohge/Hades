#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "RequestHandle.h"
#include "core/Types.h"


namespace hsna
{
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_CreateAccessor(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self, hc::IScriptInstance* instance)
	{
		self->CreateAccessor(instance);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_UnrigorousAccessor(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self, uint64* id, hc::IScriptInstance* instance)
	{
		self->UnrigorousAccessor(*id, instance);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_ReadonlyAccessor(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self, uint64* id, hc::IScriptInstance* instance)
	{
		self->ReadonlyAccessor(*id, instance);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_ReadwriteAccessor(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self, uint64* id, hc::IScriptInstance* instance)
	{
		self->ReadwriteAccessor(*id, instance);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_CreateAccessor_Noinstance(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self)
	{
		hc::IScriptInstance* instance = self->CreateAccessor();
		*res = instance->GetScriptInstance();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_UnrigorousAccessor_Noinstance(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self, uint64* id)
	{
		hc::IScriptInstance* instance = self->UnrigorousAccessor(*id);
		*res = instance->GetScriptInstance();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_ReadonlyAccessor_Noinstance(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self, uint64* id)
	{
		hc::IScriptInstance* instance = self->ReadonlyAccessor(*id);
		*res = instance->GetScriptInstance();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_ReadwriteAccessor_Noinstance(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self, uint64* id)
	{
		hc::IScriptInstance* instance = self->ReadwriteAccessor(*id);
		*res = instance->GetScriptInstance();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_CreateAccessorImmediately(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self)
	{
		bool isyeild = false;
		int magic = 0;
		hc::IScriptInstance* instance = self->CreateAccessorImmediately(isyeild, magic);
		if (isyeild)
		{
			*res = hc::nullhandle;
			return magic;
		}
		else
		{
			*res = instance->GetScriptInstance();
			return 1;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_UnrigorousAccessorImmediately(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self, uint64* id)
	{
		bool isyeild = false;
		int magic = 0;
		hc::IScriptInstance* instance = self->UnrigorousAccessorImmediately(*id, isyeild, magic);
		if (isyeild)
		{
			*res = hc::nullhandle;
			return magic;
		}
		else
		{
			*res = instance->GetScriptInstance();
			return 1;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_ReadonlyAccessorImmediately(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self, uint64* id)
	{
		bool isyeild = false;
		int magic = 0;
		hc::IScriptInstance* instance = self->ReadonlyAccessorImmediately(*id, isyeild, magic);
		if (isyeild)
		{
			*res = hc::nullhandle;
			return magic;
		}
		else
		{
			*res = instance->GetScriptInstance();
			return 1;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_ReadwriteAccessorImmediately(hc::IVirtualMachine* vm, hc::FunctionRegister* func, int* res, RequestHandle* self, uint64* id)
	{
		bool isyeild = false;
		int magic = 0;
		hc::IScriptInstance* instance = self->ReadwriteAccessorImmediately(*id, isyeild, magic);
		if (isyeild)
		{
			*res = hc::nullhandle;
			return magic;
		}
		else
		{
			*res = instance->GetScriptInstance();
			return 1;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_DestroyAccessor(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self, uint64* id)
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_DestroyAccessorImmediately(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self, uint64* id)
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_GetAccessors(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self)
	{
		int magic = 0;
		if (self->BeginRequest(magic))
		{
			return magic;
		}
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int RequestHandle_FreeAccessors(hc::IVirtualMachine* vm, hc::FunctionRegister* func, RequestHandle* self, uint64* id)
	{
		self->ReleaseAccessor(*id);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_RequestHandle(hc::IScriptSystem& sys, hc::IVirtualMachine& vm)
	{
		hc::ClassRegister sc;
		sc.ClassType(&RequestHandle::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::IScriptInstance::RTTI());
			sf.Function(&RequestHandle_CreateAccessor, "CreateAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI(), &hc::IScriptInstance::RTTI());
			sf.Function(&RequestHandle_UnrigorousAccessor, "UnrigorousAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI(), &hc::IScriptInstance::RTTI());
			sf.Function(&RequestHandle_ReadonlyAccessor, "ReadonlyAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI(), &hc::IScriptInstance::RTTI());
			sf.Function(&RequestHandle_ReadwriteAccessor, "ReadwriteAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Function(&RequestHandle_CreateAccessor_Noinstance, "CreateAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_UnrigorousAccessor_Noinstance, "UnrigorousAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_ReadonlyAccessor_Noinstance, "ReadonlyAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_ReadwriteAccessor_Noinstance, "ReadwriteAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Function(&RequestHandle_CreateAccessorImmediately, "CreateAccessorImmediately");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_UnrigorousAccessorImmediately, "UnrigorousAccessorImmediately");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_ReadonlyAccessorImmediately, "ReadonlyAccessorImmediately");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Instance::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_ReadwriteAccessorImmediately, "ReadwriteAccessorImmediately");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_DestroyAccessor, "DestroyAccessor");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_DestroyAccessorImmediately, "DestroyAccessorImmediately");
			sc.ClassFunction(sf);
		}


		{
			hc::FunctionRegister sf;
			sf.Function(&RequestHandle_GetAccessors, "GetAccessors");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&RequestHandle_FreeAccessors, "FreeAccessors");
			sc.ClassFunction(sf);
		}



		vm.RegisterClass(sc);
	}
}