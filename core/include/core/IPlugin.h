#pragma once
#include "core/Object.hpp"

#include <EASTL/map.h>
#include <EASTL/string.h>

namespace hc
{
	class SystemManager;
	class HADES_CORE_API IPlugin : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IPlugin);
	public:
		IPlugin();
		virtual ~IPlugin();
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;
	};
}