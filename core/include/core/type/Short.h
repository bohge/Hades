#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Short : public Type<short>
	{
		RTTI_TYPE_DECLEAR(Short);
	public:
		Short(void){};
		Short(short v) :Type<short>(v){};
		~Short(void){};
	public:
		HADES_FORCEINLINE Short operator + (const Short& rhs)
		{
			return Short(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Short operator - (const Short& rhs)
		{
			return Short(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Short operator * (const Short& rhs)
		{
			return Short(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Short operator / (const Short& rhs)
		{
			return Short(GetData() + rhs.GetData());
		}
	};
}