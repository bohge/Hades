#pragma once

#include "core/Object.hpp"

#include <EASTL/string.h>

namespace hc
{
	template<typename T>
	class HADES_CORE_API Type : public Object
	{
	private:
		T			m_Data;
	public:
		Type(void){};
		Type( const T& d ):m_Data(d){}
		virtual ~Type(void){};
	public:
		HADES_FORCEINLINE operator T&()
		{
			return m_Data;
		}
		HADES_FORCEINLINE operator const T&() const
		{
			return m_Data;
		}
		HADES_FORCEINLINE const T& GetData() const
		{
			return m_Data;
		}
		HADES_FORCEINLINE T& GetData()
		{
			return m_Data;
		}
	};
}