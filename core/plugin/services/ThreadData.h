#pragma once
#include "core/Object.hpp"




namespace hc
{
	class IArguments;
	class HADES_CORE_API ThreadData : public Object
	{
		RTTI_TYPE_DECLEAR(ThreadData);
	public:
		IArguments*	InputArgument;
		IArguments*	ResultArgument;
	public:
		ThreadData(void);
		virtual ~ThreadData(void);
	};
}
