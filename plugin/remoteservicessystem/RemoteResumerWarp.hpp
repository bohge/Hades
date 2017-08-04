#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "RemoteResumer.h"

namespace hrss
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_RemoteResumer(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&RemoteResumer::RTTI(), &hc::IResumer::RTTI());
		sys.SetNativeLibrary(name, sc);
	}
}