#include "ThreadEvent.h"


#include <time.h>
#ifdef WIN32
#   include <windows.h>
#else
#   include <sys/time.h>
#endif



#include <pthread.h>

using namespace hc;
namespace hpts
{
	//------------------------------------------------------------------------------------------------------
	ThreadEvent::ThreadEvent(void)
	{
		m_pCondition = NEW pthread_cond_t;
		m_pMutex = NEW pthread_mutex_t;
		pthread_cond_init( static_cast<pthread_cond_t*>(m_pCondition), NULL );
		pthread_mutex_init( static_cast<pthread_mutex_t*>(m_pMutex), NULL );

	}
	//------------------------------------------------------------------------------------------------------
	ThreadEvent::~ThreadEvent(void)
	{
		pthread_cond_destroy( static_cast<pthread_cond_t*>(m_pCondition) );
		pthread_mutex_destroy( static_cast<pthread_mutex_t*>(m_pMutex) );
		delete static_cast<pthread_cond_t*>(m_pCondition);
		delete static_cast<pthread_mutex_t*>(m_pMutex);
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadEvent::Lock()
	{
		pthread_mutex_lock(static_cast<pthread_mutex_t*>(m_pMutex));
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadEvent::Unlock()
	{
		pthread_mutex_unlock(static_cast<pthread_mutex_t*>(m_pMutex));
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadEvent::Wait()
	{
		//wait函数在等待时会把mutex打开，当等待完成后给mutex上锁
		//pthread_mutex_lock(static_cast<pthread_mutex_t*>(m_pMutex));
		pthread_cond_wait( static_cast<pthread_cond_t*>(m_pCondition), static_cast<pthread_mutex_t*>(m_pMutex) );
		pthread_mutex_unlock(static_cast<pthread_mutex_t*>(m_pMutex));
	}
	//------------------------------------------------------------------------------------------------------
	void ThreadEvent::Signal()
	{
		//pthread_mutex_lock(static_cast<pthread_mutex_t*>(m_pMutex));
		pthread_cond_signal(static_cast<pthread_cond_t*>(m_pCondition));
		pthread_mutex_unlock(static_cast<pthread_mutex_t*>(m_pMutex));
	}
	//------------------------------------------------------------------------------------------------------
#ifdef WIN32
	struct timezone;
	int gettimeofday(struct timeval * tp, struct timezone * tzp)
	{
		// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
		static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

		SYSTEMTIME  system_time;
		FILETIME    file_time;
		uint64_t    time;

		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		time = ((uint64_t)file_time.dwLowDateTime);
		time += ((uint64_t)file_time.dwHighDateTime) << 32;

		tp->tv_sec = (long)((time - EPOCH) / 10000000L);
		tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
		return 0;
	}
#endif
	void ThreadEvent::Wait(uint us)
	{
		struct timeval now;
		struct timespec outtime;
		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec;
		outtime.tv_nsec = (now.tv_usec + us) * 1000;//只尝试锁us毫秒
		pthread_cond_timedwait(static_cast<pthread_cond_t*>(m_pCondition), static_cast<pthread_mutex_t*>(m_pMutex), &outtime);
	}
}