#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API UInt64 : public Type<uint64>
	{
		RTTI_TYPE_DECLEAR(UInt64);
	public:
		UInt64(void){};
		UInt64(uint64 v) :Type<uint64>(v){};
		~UInt64(void){};
	public:
		HADES_FORCEINLINE UInt64 operator + (const UInt64& rhs)
		{
			return UInt64(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE UInt64 operator - (const UInt64& rhs)
		{
			return UInt64(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE UInt64 operator * (const UInt64& rhs)
		{
			return UInt64(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE UInt64 operator / (const UInt64& rhs)
		{
			return UInt64(GetData() + rhs.GetData());
		}
	};
}