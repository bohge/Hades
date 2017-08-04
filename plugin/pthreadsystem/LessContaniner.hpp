#pragma once
#include "core/SmartPtr.hpp"
#include "IJobContainer.h"
#include "ThreadRWLock.h"
#include "thread/IJob.h"



#include <EASTL/map.h>


namespace hpts
{
	class ThreadRWLock;
	//异步线程队列
	class LessContaniner : public IJobContainer
	{
	private:
		typedef eastl::multimap< int, hc::SmartPtr<hc::IJob> > JobLessMap;
	private:
		ThreadRWLock*	m_pRWMutex;
		JobLessMap		m_JobLessMap;//数据队列
	public:
		LessContaniner(void)
			:m_pRWMutex(NEW ThreadRWLock)
		{
		}
		~LessContaniner(void)
		{
			SAFE_DELETE(m_pRWMutex);
		}
	public:
		//------------------------------------------------------------------------------------------------------
		virtual void Clear()
		{
			m_pRWMutex->WriteLock();
			m_JobLessMap.clear();
			m_pRWMutex->Unlock();
		}
		//------------------------------------------------------------------------------------------------------
		virtual bool TryPopJob(hc::SmartPtr<hc::IJob>& job)
		{
			bool res = false;
			m_pRWMutex->WriteLock();
			if (!m_JobLessMap.empty())
			{
				res = true;
				JobLessMap::iterator it = m_JobLessMap.begin();
				job = it->second;
				m_JobLessMap.erase(it);
			}
			m_pRWMutex->Unlock();
			return res;
		}
		//------------------------------------------------------------------------------------------------------
		virtual void PushJob(hc::SmartPtr<hc::IJob>& job)
		{
			m_pRWMutex->WriteLock();
			m_JobLessMap.insert( eastl::make_pair( job->GetPriority(), job ) );
			m_pRWMutex->Unlock();
		}
		//------------------------------------------------------------------------------------------------------
		virtual uint Size()
		{
			uint res;
			//m_pRWMutex->ReadLock();
			res = m_JobLessMap.size();
			//m_pRWMutex->Unlock();
			return res;
		}
	};
}
