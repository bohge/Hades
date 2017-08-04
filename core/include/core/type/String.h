#pragma once
#include "Type.h"
#include "EASTL/string.h"

namespace hc
{
	class HADES_CORE_API String : public Type<eastl::string>
	{
		RTTI_TYPE_DECLEAR( String )
	public:
		String(void){};
		String( const eastl::string v ):Type<eastl::string>(v){};
		~String(void){};
	public:
		HADES_FORCEINLINE String operator + ( const String& rhs)
		{
			return String( GetData() + rhs.GetData() );
		}
	};
}