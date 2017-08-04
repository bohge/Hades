#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API UInt : public Type<uint>
	{
		RTTI_TYPE_DECLEAR(UInt);
	public:
		UInt(void){};
		UInt(uint v) :Type<uint>(v){};
		~UInt(void){};
	public:
		HADES_FORCEINLINE UInt operator + ( const UInt& rhs)
		{
			return UInt( GetData() + rhs.GetData() );
		}
		HADES_FORCEINLINE UInt operator - ( const UInt& rhs)
		{
			return UInt( GetData() + rhs.GetData() );
		}
		HADES_FORCEINLINE UInt operator * ( const UInt& rhs)
		{
			return UInt( GetData() + rhs.GetData() );
		}
		HADES_FORCEINLINE UInt operator / ( const UInt& rhs)
		{
			return UInt( GetData() + rhs.GetData() );
		}
	};
}