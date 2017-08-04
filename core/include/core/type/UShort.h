#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API UShort : public Type<ushort>
	{
		RTTI_TYPE_DECLEAR(UShort)
	public:
		UShort(void){};
		UShort(ushort v) :Type<ushort>(v){};
		~UShort(void){};
	public:
		HADES_FORCEINLINE UShort operator + (const UShort& rhs)
		{
			return UShort(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE UShort operator - (const UShort& rhs)
		{
			return UShort(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE UShort operator * (const UShort& rhs)
		{
			return UShort(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE UShort operator / (const UShort& rhs)
		{
			return UShort(GetData() + rhs.GetData());
		}
	};
}