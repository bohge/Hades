#pragma once
#include "core/SmartPtr.hpp"
#include "IJobContainer.h"
#include "ThreadRWLock.h"
#include "thread/IJob.h"



#include <EASTL/map.h>


namespace hpts
{
	//异步线程队列
	class GreaterContainer : public IJobContainer
	{
	private:
		typedef eastl::multimap< int, hc::SmartPtr<hc::IJob>, eastl::greater<int> > JobGreaterMap;
	private:
		ThreadRWLock*		m_pRWMutex;
		JobGreaterMap		m_JobGreaterMap;//数据队列
	public:
		GreaterContainer(void) 
			:m_pRWMutex(NEW ThreadRWLock)
		{
		}
		~GreaterContainer(void)
		{
			SAFE_DELETE(m_pRWMutex);
		}
	public:
		//------------------------------------------------------------------------------------------------------
		virtual void Clear()
		{
			m_pRWMutex->WriteLock();
			m_JobGreaterMap.clear();
			m_pRWMutex->Unlock();
		}
		//------------------------------------------------------------------------------------------------------
		virtual bool TryPopJob(hc::SmartPtr<hc::IJob>& job)
		{
			bool res = false;
			m_pRWMutex->WriteLock();
			if (!m_JobGreaterMap.empty())
			{
				res = true;
				JobGreaterMap::iterator it = m_JobGreaterMap.begin();
				job = it->second;
				m_JobGreaterMap.erase(it);
			}
			m_pRWMutex->Unlock();
			return res;
		}
		//------------------------------------------------------------------------------------------------------
		virtual void PushJob(hc::SmartPtr<hc::IJob>& job)
		{
			m_pRWMutex->WriteLock();
			m_JobGreaterMap.insert( eastl::make_pair( job->GetPriority(), job ) );
			m_pRWMutex->Unlock();
		}
		//------------------------------------------------------------------------------------------------------
		virtual uint Size()
		{
			uint res;
			m_pRWMutex->ReadLock();
			res = m_JobGreaterMap.size();
			m_pRWMutex->Unlock();
			return res;
		}
	};
}
