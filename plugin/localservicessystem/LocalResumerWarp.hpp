#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "LocalResumer.h"

namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_LocalResumer(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&LocalResumer::RTTI(), &hc::IResumer::RTTI());
		sys.SetNativeLibrary(name, sc);
	}
}