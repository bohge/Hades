#pragma once
#include "core/SmartPtr.hpp"
#include "IJobContainer.h"
#include "ThreadRWLock.h"
#include "thread/IJob.h"



#include <EASTL/list.h>
//#include <list>
namespace hpts
{
	//异步线程队列
	class StdQueueContainer : public IJobContainer
	{
	private:
		typedef eastl::list< hc::SmartPtr<hc::IJob> > JobList;
	private:
		ThreadRWLock*	m_pRWMutex;
		JobList			m_JobList;//数据队列
	public:
		StdQueueContainer(void)
			:m_pRWMutex(NEW ThreadRWLock)
		{
		}
		~StdQueueContainer(void)
		{
			SAFE_DELETE(m_pRWMutex);
		}
	public:
		//------------------------------------------------------------------------------------------------------
		virtual void Clear()
		{
			m_pRWMutex->WriteLock();
			m_JobList.clear();
			m_pRWMutex->Unlock();
		}
		//------------------------------------------------------------------------------------------------------

		virtual bool TryPopJob(hc::SmartPtr<hc::IJob>& job)
		{
			bool res = false;
			m_pRWMutex->WriteLock();
			if (!m_JobList.empty())
			{
				job = m_JobList.front();
				m_JobList.pop_front();
				res = true;
			}
			m_pRWMutex->Unlock();
			return res;
		}
		//------------------------------------------------------------------------------------------------------
		virtual void PushJob(hc::SmartPtr<hc::IJob>& job)
		{
			m_pRWMutex->WriteLock();
			m_JobList.push_back(job);
			m_pRWMutex->Unlock();
		}
		//------------------------------------------------------------------------------------------------------
		virtual uint Size()
		{
			uint res;
			m_pRWMutex->ReadLock();
			res = m_JobList.size();
			m_pRWMutex->Unlock();
			return res;
		}
	};
}
