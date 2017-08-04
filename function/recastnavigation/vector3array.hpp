#pragma once
#include "vector3.hpp"

#include <EASTL/vector.h>

namespace hrnf
{
	class vector3array
	{
	private:
		enum Constant
		{
			VAC_DATA_COUNT = 3,
		};
	private:
		typedef eastl::vector< float >	DataArray;
	private:
		uint			m_Size;
		DataArray		m_Data;
	public:
		vector3array(void) :m_Size(0){}
		virtual ~vector3array(void){}
	public:
		HADES_FORCEINLINE const eastl::vector< float >& Data() const
		{
			return m_Data;
		}
		HADES_FORCEINLINE uint Size()
		{
			return m_Size;
		}
		HADES_FORCEINLINE void Clear()
		{
			m_Size = 0;
			m_Data.clear();
		}
		HADES_FORCEINLINE void PushBack(const vector3& data)
		{
			++m_Size;
			m_Data.push_back(data.m_x);
			m_Data.push_back(data.m_y);
			m_Data.push_back(data.m_z);
		}
		HADES_FORCEINLINE vector3 At(uint index)
		{
			float x = m_Data[index*VAC_DATA_COUNT];
			float y = m_Data[index*VAC_DATA_COUNT + 1];
			float z = m_Data[index*VAC_DATA_COUNT + 2];
			return vector3(x, y, z);
		}
		HADES_FORCEINLINE const float& XAt(uint index)
		{
			return m_Data[index*VAC_DATA_COUNT];
		}
		HADES_FORCEINLINE const float& YAt(uint index)
		{
			return m_Data[index*VAC_DATA_COUNT + 1];
		}
		HADES_FORCEINLINE const float& ZAt(uint index)
		{
			return m_Data[index*VAC_DATA_COUNT + 2];
		}
	};
}