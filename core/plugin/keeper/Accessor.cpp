#include "keeper/Accessor.h"
#include "core/Bufferpool.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(Accessor);
	//---------------------------------------------------------------------------------------------------------
	Accessor::Accessor()
		:m_ID(nullid)
		, m_rpBuffer(NULL)
		, m_Usingsize(0)
		, m_Realsize(0)
		, m_eAccessorMode(AM_NON_ACCESSOR)
		, m_Timestamp(nulltimestamp)
	{
		
	}
	//---------------------------------------------------------------------------------------------------------
	void Accessor::Reset()
	{
		m_ID = nullid;
		if (NULL != m_rpBuffer)
		{
			Bufferpool::Instance()->Free(m_Realsize, m_rpBuffer);
			m_rpBuffer = NULL;
		}
		m_Usingsize = 0;
		m_Realsize = 0;
		m_eAccessorMode = AM_NON_ACCESSOR;
	}
	//---------------------------------------------------------------------------------------------------------
	Accessor::Accessor(const Accessor& rhs)
	{
		ASSERT(false);
	}
	//---------------------------------------------------------------------------------------------------------
	Accessor::~Accessor()
	{
		if (NULL != m_rpBuffer)
		{
			Bufferpool::Instance()->Free(m_Realsize, m_rpBuffer);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Accessor::_TryReallocBuffer(uint size)
	{
		if (m_Realsize < size)
		{
			if (NULL != m_rpBuffer)
			{
				ASSERT(0 != m_Realsize);
				Bufferpool::Instance()->Free(m_Realsize, m_rpBuffer);
			}
			m_rpBuffer = Bufferpool::Instance()->Get(size, m_Realsize);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Accessor::SetupAccessor(const Accessor& rhs, bool iscopy)
	{
		m_ID = rhs.m_ID;
		m_Usingsize = rhs.m_Usingsize;
		if (iscopy)
		{
			_TryReallocBuffer(rhs.m_Usingsize);
			if (NULL != rhs.m_rpBuffer
				&& rhs.m_Realsize >= rhs.m_Usingsize)
			{
				memcpy(m_rpBuffer, rhs.m_rpBuffer, rhs.m_Usingsize);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Accessor::CopyFrom(const byte* form, uint usingsize)
	{
		_TryReallocBuffer(usingsize);
		m_Usingsize = usingsize;
		//printf("m_Realsize %d, m_Usingsize %d\r\n", m_Realsize, m_Usingsize);
		memcpy(m_rpBuffer, form, usingsize);
	}

}