#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Float : public Type<float>
	{
		RTTI_TYPE_DECLEAR(Float)
	public:
		Float(void){};
		Float(float v) :Type<float>(v){};
		~Float(void){};
	public:
		HADES_FORCEINLINE Float operator + (const Float& rhs)
		{
			return Float(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Float operator - (const Float& rhs)
		{
			return Float(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Float operator * (const Float& rhs)
		{
			return Float(GetData() + rhs.GetData());
		}
		HADES_FORCEINLINE Float operator / (const Float& rhs)
		{
			return Float(GetData() + rhs.GetData());
		}
	};
}