#pragma once
#include "script/IScriptSystem.h"

namespace hlss
{
	class LuaScriptSystem : public hc::IScriptSystem
	{
	public:
		LuaScriptSystem();
		virtual ~LuaScriptSystem();
	private:
		virtual hc::IVirtualMachine* _DoVirtualMachineFactory(uint index);
		virtual void _DoRecycleBin(hc::IVirtualMachine* vm);
	private:
		virtual hc::IArguments* _DoAllocArguments();
		virtual void _DoFreeArguments(hc::IArguments* args);
	public:
		virtual hc::IScriptSystem::ArgumentFunction GetScriptArgumentFunction(const hc::Rtti* rtti) const;
		virtual hc::IScriptSystem::ResultFunction GetNativeResultValueFunction(const hc::Rtti* rtti) const;
		virtual hc::IScriptSystem::ResultFunction GetNativeResultPushFunction(const hc::Rtti* rtti) const;
		virtual hc::IScriptSystem::TypeCompare GetTypeCompare();
	public:
		virtual void Initialize();
		virtual void Exit();
	};
}
