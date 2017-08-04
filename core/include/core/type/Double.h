#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Double : public Type<double>
	{
		RTTI_TYPE_DECLEAR(Double)
	public:
		Double(void){};
		Double(double v) :Type<double>(v){};
		~Double(void){};
	public:
		HADES_FORCEINLINE Double operator + (const Double& rhs)
		{
			return Double(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Double operator - (const Double& rhs)
		{
			return Double(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Double operator * (const Double& rhs)
		{
			return Double(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Double operator / (const Double& rhs)
		{
			return Double(GetData() + rhs.GetData());
		}
	};
}