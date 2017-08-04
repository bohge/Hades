#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "BehavictreePlugin.h"
#include "Behavior.h"

namespace hbtf
{
	//-------------------------------------------------------------------------------------------------------
	static int BehavictreePlugin_SetupBehavic(hc::IVirtualMachine* vm, hc::FunctionRegister* func, BehavictreePlugin* self, int* format, const char* filepath, const char* metapath)
	{		
		self->SetupBehavic((BehavictreePlugin::Fileformat)*format, filepath, metapath);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int BehavictreePlugin_SetupTreeTemplate(hc::IVirtualMachine* vm, hc::FunctionRegister* func, BehavictreePlugin* self, hc::IScriptInstance* response, const char* name)
	{
		self->SetupTreeTemplate(response, name);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int BehavictreePlugin_CreateBehavior(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, BehavictreePlugin* self, const char* name)
	{
		Behavior* obj = self->CreateBehavior(name);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_BehavictreePlugin(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&BehavictreePlugin::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::Int::RTTI(), &hc::String::RTTI(), &hc::String::RTTI());
			sf.Function(&BehavictreePlugin_SetupBehavic, "SetupBehavic");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Behavior::RTTI());
			sf.Arguments(&hc::String::RTTI());
			sf.Function(&BehavictreePlugin_CreateBehavior, "CreateBehavior");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::IScriptInstance::RTTI(), &hc::String::RTTI());
			sf.Function(&BehavictreePlugin_SetupTreeTemplate, "SetupTreeTemplate");
			sc.ClassFunction(sf);
		}

		sc.ClassConstant(BehavictreePlugin::Fileformat::EFF_XML, "EFF_XML");
		sc.ClassConstant(BehavictreePlugin::Fileformat::EFF_BSON, "EFF_BSON");

		sys.SetNativeLibrary(name, sc);
		sys.SetNativeGlobal(name, BehavictreePlugin::RTTI().GetTypeName(), BehavictreePlugin::Instance());
	}
}