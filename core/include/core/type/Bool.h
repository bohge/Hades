#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Bool : public Type<bool>
	{
		RTTI_TYPE_DECLEAR(Bool)
	public:
		Bool(void){};
		Bool(bool v) :Type<bool>(v){};
		~Bool(void){};
	public:
		HADES_FORCEINLINE Bool operator == (const Bool& rhs)
		{
			return Bool(GetData() == rhs.GetData());
		}
		HADES_FORCEINLINE Bool operator != (const Bool& rhs)
		{
			return Bool(GetData() != rhs.GetData());
		}
	};
}