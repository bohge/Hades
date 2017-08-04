#pragma once
#include "core/Object.hpp"



namespace hc
{
	class HADES_CORE_API Accessor : public Object
	{
		RTTI_TYPE_DECLEAR(Accessor);
	public:
		enum AccessMode
		{
			AM_NON_ACCESSOR = 0,
			AM_UNRIGOROUS,
			AM_READ_ONLY,
			AM_READ_WRITE,
			AM_CREATE_ACCESS,
			AM_DESTORY_ACCESS,
		};
	private:
		uint64			m_ID;
		byte*			m_rpBuffer;
		AccessMode		m_eAccessorMode;
		int				m_Timestamp;
		bool			m_TimestampValid;
		uint			m_Realsize;
		uint			m_Usingsize;
	public:
		Accessor();
		virtual ~Accessor();
	private:
		Accessor(const Accessor& rhs);
	private:
		void _TryReallocBuffer(uint size);
	public:
		void SetupAccessor(const Accessor& rhs, bool iscopy);
		void CopyFrom(const byte* form, uint usingsize);
		void Reset();
	public:
		static HADES_FORCEINLINE bool isHigh(AccessMode lhs, AccessMode rhs);
	public:		
		HADES_FORCEINLINE uint GetRealSize();//实际内存大小
		HADES_FORCEINLINE uint GetUsingSize();//使用中的内存大小
		HADES_FORCEINLINE void SetUsingSize(uint size);
		HADES_FORCEINLINE uint64 GetID();
		HADES_FORCEINLINE void SetID(uint64 id);
		HADES_FORCEINLINE void SetUserIndex(byte index);
		HADES_FORCEINLINE void SetAccessorMode(AccessMode am);
		HADES_FORCEINLINE AccessMode GetAccessorMode();
		HADES_FORCEINLINE const byte* GetBuffer();
		HADES_FORCEINLINE bool isTimestampValid();
		HADES_FORCEINLINE void SetTimestampValid(bool v);
		HADES_FORCEINLINE void SetTimestamp(int v);
		HADES_FORCEINLINE int GetTimestamp();
		HADES_FORCEINLINE bool isLowthan(AccessMode am);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool Accessor::isHigh(AccessMode lhs, AccessMode rhs)
	{
		return lhs > rhs;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool Accessor::isLowthan(AccessMode am)
	{
		return isHigh(am, m_eAccessorMode);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Accessor::SetTimestamp(int v)
	{
		m_Timestamp = v;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE int Accessor::GetTimestamp()
	{
		return m_Timestamp;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool Accessor::isTimestampValid()
	{
		return m_TimestampValid;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Accessor::SetTimestampValid(bool v)
	{
		m_TimestampValid = v;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const byte* Accessor::GetBuffer()
	{
		return m_rpBuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Accessor::GetRealSize()
	{
		return m_Realsize;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Accessor::GetUsingSize()
	{
		return m_Usingsize;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Accessor::SetUsingSize(uint size)
	{
		m_Usingsize = size;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 Accessor::GetID()
	{
		return m_ID;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Accessor::SetID(uint64 id)
	{
		m_ID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Accessor::SetAccessorMode(Accessor::AccessMode am)
	{
		m_eAccessorMode = am;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE Accessor::AccessMode Accessor::GetAccessorMode()
	{
		return m_eAccessorMode;
	}
}