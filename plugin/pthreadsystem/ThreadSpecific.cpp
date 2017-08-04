#include "ThreadSpecific.h"
#include "log/ILogSystem.h"

namespace hpts
{
	//---------------------------------------------------------------------------------------------------------
	ThreadSpecific::ThreadSpecific(void)
		:m_pKey(NULL)
	{
		Initialize();
	}
	//---------------------------------------------------------------------------------------------------------
	ThreadSpecific::~ThreadSpecific(void)
	{
		if ( NULL != m_pKey )
		{
			pthread_setspecific(*m_pKey, NULL);
			pthread_key_delete(*m_pKey);
			SAFE_DELETE( m_pKey );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void* ThreadSpecific::_GetSpecific()
	{
		return pthread_getspecific( *m_pKey );
	}
	//---------------------------------------------------------------------------------------------------------
	void ThreadSpecific::Initialize( )
	{
		ASSERT( NULL == m_pKey );
		m_pKey = NEW pthread_key_t;
		int ret = pthread_key_create(m_pKey, NULL);
		if (ret != 0)
		{
			HADESLOG(" can not init TLS Slot ");
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ThreadSpecific::SetSpecific( const void* data )
	{
		ASSERT( pthread_getspecific(*m_pKey) == NULL );
		pthread_setspecific(*m_pKey, data);	
	}

}