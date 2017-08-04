#pragma once
#include "core/IFunction.h"


namespace hmfp
{
	class MathFunctionPlugin : public hc::IFunction
	{
		RTTI_DRIVER_TYPE_DECLEAR(MathFunctionPlugin, hc::IFunction);
	public:
		MathFunctionPlugin();
		~MathFunctionPlugin();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	};
}