#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptSystem.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"
#include "core/Log.h"

#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"
#include "script/IFunctionInvoker.h"

#include "ThulacFunction.h"

namespace htf
{
	//-------------------------------------------------------------------------------------------------------
	static int TulacFunction_Initialize(hc::IVirtualMachine* vm, hc::FunctionRegister* func, ThulacFunction* self, hc::StringBuffer* model, hc::StringBuffer* user, bool* just_seg, bool* t2s, bool* ufilter, hc::StringBuffer* sep)
	{
		self->InitializeThulac((const char*)model->pBuffer,
			(const char*)user->pBuffer,
			*just_seg,
			*t2s,
			*ufilter,
			((const char*)sep->pBuffer)[0]);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int TulacFunction_Split(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, ThulacFunction* self, hc::StringBuffer* input)
	{
		*res = self->Split((const char*)input->pBuffer);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_TulacFunction(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&ThulacFunction::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::String::RTTI(), &hc::String::RTTI(), &hc::Bool::RTTI(), &hc::Bool::RTTI(), &hc::Bool::RTTI(), &hc::String::RTTI());
			sf.Function(&htf::TulacFunction_Initialize, "Initialize");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::String::RTTI());
			sf.Result(&hc::String::RTTI());
			sf.Function(&htf::TulacFunction_Split, "Split");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(name, sc);
		sys.SetNativeGlobal(name, ThulacFunction::RTTI().GetTypeName(), ThulacFunction::Instance());
	}
}