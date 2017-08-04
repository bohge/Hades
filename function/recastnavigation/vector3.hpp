#pragma once
#include "serialize/ISerializable.h"


namespace hrnf
{
	class vector3 : public hc::ISerializable
	{
		RTTI_TYPE_DECLEAR(vector3);
		SERIALIZABL_TYPE_DECLEAR(vector3);
	public:
		union
		{
			struct	{ float	m_x, m_y, m_z; };
			float		m_Data[3];
		};
	public:
		HADES_FORCEINLINE vector3()
			:m_x(0.0f)
			, m_y(0.0f)
			, m_z(0.0f)
		{
		}
		HADES_FORCEINLINE vector3(float x, float y, float z)
			: m_x(x)
			, m_y(y)
			, m_z(z)
		{
		}
		HADES_FORCEINLINE vector3& operator+= (const vector3& v3)
		{
			m_x += v3.m_x;
			m_y += v3.m_y;
			m_z += v3.m_z;
			return *this;
		}
		HADES_FORCEINLINE vector3& operator-= (const vector3& v3)
		{
			m_x -= v3.m_x;
			m_y -= v3.m_y;
			m_z -= v3.m_z;
			return *this;
		}
		HADES_FORCEINLINE vector3& operator*= (const vector3& v3)
		{
			m_x *= v3.m_x;
			m_y *= v3.m_y;
			m_z *= v3.m_z;
			return *this;
		}
		HADES_FORCEINLINE vector3& operator/= (const vector3& v3)
		{
			m_x /= v3.m_x;
			m_y /= v3.m_y;
			m_z /= v3.m_z;
			return *this;
		}
		HADES_FORCEINLINE vector3 operator- (const vector3& v3) const
		{
			return vector3(m_x - v3.m_x, m_y - v3.m_y, m_z - v3.m_z);
		}
		HADES_FORCEINLINE vector3 operator+ (const vector3& v3) const
		{
			return vector3(m_x + v3.m_x, m_y + v3.m_y, m_z + v3.m_z);
		}
		HADES_FORCEINLINE vector3 operator/ (const vector3& v3) const
		{
			return vector3(m_x / v3.m_x, m_y / v3.m_y, m_z / v3.m_z);
		}
		HADES_FORCEINLINE vector3 operator* (const vector3& v3) const
		{
			return vector3(m_x * v3.m_x, m_y * v3.m_y, m_z * v3.m_z);
		}
		//Êý×Ö
		HADES_FORCEINLINE vector3& operator*= (const float num)
		{
			m_x *= num;
			m_y *= num;
			m_z *= num;
			return *this;
		}
		HADES_FORCEINLINE vector3& operator/= (const float num)
		{
			float inv = 1.0f / num;
			m_x *= inv;
			m_y *= inv;
			m_z *= inv;
			return *this;
		}
		HADES_FORCEINLINE vector3& operator+= (const float num)
		{
			m_x += num;
			m_y += num;
			m_z += num;
			return *this;
		}
		HADES_FORCEINLINE vector3& operator-= (const float num)
		{
			m_x -= num;
			m_y -= num;
			m_z -= num;
			return *this;
		}
		HADES_FORCEINLINE vector3 operator/ (const float num) const
		{
			float inv = 1.0f / num;
			return vector3(m_x * inv, m_y * inv, m_z * inv);
		}
		HADES_FORCEINLINE float LengthPow() const
		{
			return m_x*m_x + m_y*m_y + m_z*m_z;
		}
	public:
		virtual const byte* GetBuffer() const
		{
			return (byte*)m_Data;
		}
		virtual uint GetLength() const
		{
			return 3 * sizeof(float);
		}
		virtual bool FromBuffer(const byte* buff, uint len)
		{
			memcpy(m_Data, buff, GetLength());
			return true;
		}
	};
}