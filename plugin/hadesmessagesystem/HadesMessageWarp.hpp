#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "HadesMessage.h"

namespace hms
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_HadesMessage(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&HadesMessage::RTTI(), &hc::IMessage::RTTI());
		sys.SetNativeLibrary(name, sc);
	}
}