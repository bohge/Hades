//#include "JobThreadPool.h"
//#include "JobThread.h"
//
//using namespace eastl;
//namespace hpts
//{
//	SINGLETON_IMPLEMENT(JobThreadPool);
//	//---------------------------------------------------------------------------------------------------------
//	JobThreadPool::JobThreadPool(void)
//	{
//	}
//	//---------------------------------------------------------------------------------------------------------
//	JobThreadPool::~JobThreadPool(void)
//	{
//	}
//	//---------------------------------------------------------------------------------------------------------
//	JobThread* JobThreadPool::GetJobThread()
//	{
//		if ( m_ThreadList.empty() )
//		{
//			return NEW JobThread;
//		}
//		else
//		{
//			JobThread* t = m_ThreadList.front();
//			m_ThreadList.pop_front();
//			t->Reset();
//			return t;
//		}
//		return NULL;
//	}
//	//---------------------------------------------------------------------------------------------------------
//	void JobThreadPool::ReleaseJobThread( JobThread* t )
//	{
//		m_ThreadList.push_back( t );
//	}
//	//---------------------------------------------------------------------------------------------------------
//	void JobThreadPool::_OnCreate()
//	{
//
//	}
//	//---------------------------------------------------------------------------------------------------------
//	void JobThreadPool::_OnDestroy()
//	{
//		FOR_EACH( ThreadList, thr, m_ThreadList )
//		{
//			(*thr)->Stop();
//			SAFE_DELETE( *thr );
//		}
//		m_ThreadList.clear();
//	}
//
//}