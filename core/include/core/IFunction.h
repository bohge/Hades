#pragma once
#include "core/IPlugin.h"

#include <EASTL/map.h>
#include <EASTL/string.h>

namespace hc
{
	class SystemManager;
	class HADES_CORE_API IFunction : public IPlugin
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IFunction);
	public:
		IFunction();
		virtual ~IFunction();
	};
}