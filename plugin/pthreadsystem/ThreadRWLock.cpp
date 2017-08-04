#include "ThreadRWLock.h"


#include <pthread.h>


namespace hpts
{
	//------------------------------------------------------------------------------------------------------
	ThreadRWLock::ThreadRWLock(void)
	{
		m_pRWLock = NEW pthread_rwlock_t;
		pthread_rwlock_init( static_cast<pthread_rwlock_t*>(m_pRWLock), NULL );
	}
	//------------------------------------------------------------------------------------------------------
	ThreadRWLock::~ThreadRWLock(void)
	{
		pthread_rwlock_destroy( static_cast<pthread_rwlock_t*>(m_pRWLock) );
		delete static_cast<pthread_rwlock_t*>(m_pRWLock);
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadRWLock::ReadLock()//共享方式的读取锁
	{
		pthread_rwlock_rdlock( static_cast<pthread_rwlock_t*>(m_pRWLock) );
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadRWLock::WriteLock()//独占的写入锁
	{
		pthread_rwlock_wrlock( static_cast<pthread_rwlock_t*>(m_pRWLock) );
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadRWLock::Unlock()//解锁
	{
		pthread_rwlock_unlock( static_cast<pthread_rwlock_t*>(m_pRWLock) );
	}
}