#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Int64 : public Type<int64>
	{
		RTTI_TYPE_DECLEAR(Int64)
	public:
		Int64(void){};
		Int64(int64 v) :Type<int64>(v){};
		~Int64(void){};
	public:
		HADES_FORCEINLINE Int64 operator + (const Int64& rhs)
		{
			return Int64(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Int64 operator - (const Int64& rhs)
		{
			return Int64(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Int64 operator * (const Int64& rhs)
		{
			return Int64(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Int64 operator / (const Int64& rhs)
		{
			return Int64(GetData() + rhs.GetData());
		}
	};
}