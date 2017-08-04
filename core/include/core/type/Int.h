#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Int : public Type<int>
	{
		RTTI_TYPE_DECLEAR(Int)
	public:
		Int(void){};
		Int(int v) :Type<int>(v){};
		~Int(void){};
	public:
		HADES_FORCEINLINE Int operator + (const Int& rhs)
		{
			return Int(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Int operator - (const Int& rhs)
		{
			return Int(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Int operator * (const Int& rhs)
		{
			return Int(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Int operator / (const Int& rhs)
		{
			return Int(GetData() + rhs.GetData());
		}
	};
}