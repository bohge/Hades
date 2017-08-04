#include "ThreadMutex.h"


#include <pthread.h>

namespace hpts
{
	//------------------------------------------------------------------------------------------------------
	ThreadMutex::ThreadMutex(void)
	{
		m_pMutex = NEW pthread_mutex_t;
		pthread_mutex_init( static_cast<pthread_mutex_t*>(m_pMutex), NULL );
	}
	//------------------------------------------------------------------------------------------------------
	ThreadMutex::~ThreadMutex(void)
	{
		pthread_mutex_destroy( static_cast<pthread_mutex_t*>(m_pMutex) );
		delete static_cast<pthread_mutex_t*>(m_pMutex);
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadMutex::Lock()//¼ÓËø
	{
		pthread_mutex_lock( static_cast<pthread_mutex_t*>(m_pMutex) );
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadMutex::Unlock()//½âËø
	{
		pthread_mutex_unlock( static_cast<pthread_mutex_t*>(m_pMutex) );
	}
}
