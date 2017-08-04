#pragma once
#include "core/SmartPtr.hpp"
#include "IJobContainer.h"
#include "thread/IJob.h"


#include "concurrentqueue.h"


#include <EASTL/list.h>
namespace hpts
{
	//异步线程队列
	class QueueContainer : public IJobContainer
	{
	private:
		//typedef eastl::list< SmartPtr<IJob> > JobList;
		typedef moodycamel::ConcurrentQueue< hc::SmartPtr<hc::IJob> > LockFreeQueue;
	private:
		//JobList		m_JobList;//数据队列
		LockFreeQueue*	m_pLockFreeQueue;
	public:
		QueueContainer(void)
			:m_pLockFreeQueue(NEW LockFreeQueue)
		{
		}
		~QueueContainer(void)
		{
			SAFE_DELETE(m_pLockFreeQueue);
		}
	public:
		//------------------------------------------------------------------------------------------------------
		virtual void Clear()
		{
			SAFE_DELETE(m_pLockFreeQueue);
			m_pLockFreeQueue = NEW LockFreeQueue;
			//m_JobList.clear();
		}
		//------------------------------------------------------------------------------------------------------

		virtual bool TryPopJob(hc::SmartPtr<hc::IJob>& job)
		{			
			return m_pLockFreeQueue->try_dequeue(job);
			//if (!m_JobList.empty())
			//{
			//	job = m_JobList.front();
			//	m_JobList.pop_front();
			//	return true;
			//}
			//return false;
		}
		//------------------------------------------------------------------------------------------------------
		virtual void PushJob(hc::SmartPtr<hc::IJob>& job)
		{
			//m_JobList.push_back(job);
			bool res = m_pLockFreeQueue->enqueue(job);
			ASSERT(true == res);
		}
		//------------------------------------------------------------------------------------------------------
		virtual uint Size()
		{
			return m_pLockFreeQueue->size_approx();
		}
	};
}
