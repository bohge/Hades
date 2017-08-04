#include "GreaterContainer.hpp"
#include "LessContaniner.hpp"
#include "QueueContainer.hpp"
#include "thread/IJob.h"
#include "ThreadMutex.h"
#include "ThreadEvent.h"
#include "WorkThread.h"
#include "WorkResume.h"

using namespace hc;

namespace hpts
{
	//------------------------------------------------------------------------------------------------------
	WorkThread::WorkThread(void)
		:m_isLiveing(true)
		, m_rpUserdata(NULL)
		, m_pWorkResume(NEW WorkResume(this))
	{
		m_pCondition = NEW ThreadEvent();
		m_pStopEvent = NEW ThreadEvent();
	}
	//------------------------------------------------------------------------------------------------------
	WorkThread::~WorkThread(void)
	{
		SAFE_DELETE(m_pCondition);
		SAFE_DELETE(m_pStopEvent);
		SAFE_DELETE(m_pWorkResume);
	}
	//------------------------------------------------------------------------------------------------------
	void WorkThread::_OnStart()
	{
	}
	//------------------------------------------------------------------------------------------------------
	bool WorkThread::_StopWork()
	{		
		m_pCondition->Lock();
		m_isLiveing = false;
		m_pCondition->Signal();
		m_pStopEvent->Lock();
		m_pStopEvent->Wait();
		return true;
	}
	//------------------------------------------------------------------------------------------------------
	void WorkThread::_OnReset()
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------------------------------
	void* WorkThread::DoWork()
	{
		if (!m_UserdataFactory.Empty())
		{
			m_rpUserdata = m_UserdataFactory.Unicast();
		}
		//m_pMutex->Unlock();
		while (m_isLiveing)
		{
			m_pCondition->Lock();
			if (m_JobList.empty())
			{
				m_pCondition->Wait();
				continue;
			}
			else
			{
				m_Job = m_JobList.front();
				m_JobList.pop_front();
				m_pCondition->Unlock();
				m_Job->DoJob(m_Job, m_rpUserdata, m_pWorkResume);
			}
		}
		if (!m_UserdataRecycle.Empty()
			&& NULL != m_rpUserdata)
		{
			m_UserdataRecycle.Unicast(m_rpUserdata);
		}
		m_pStopEvent->Lock();
		m_pStopEvent->Signal();
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void WorkThread::SetSortType(ThreadProperty::SortType st)
	{
		switch (st)
		{
		case ThreadProperty::ST_UNSORT: break;
		case ThreadProperty::ST_GREATER://暂时没有
		case ThreadProperty::ST_LESS:
		default: ASSERT(false); break;
		}
	}
	//------------------------------------------------------------------------------------------------------
	void WorkThread::SetJob(hc::SmartPtr<hc::IJob>& job)
	{
		//另外的队列
		ASSERT(isRunning());
		m_pCondition->Lock();
		m_JobList.push_back(job);
		m_pCondition->Signal();
	}
	//------------------------------------------------------------------------------------------------------
	void WorkThread::ResumeJob(hc::SmartPtr<hc::IJob>& job)
	{
		m_pCondition->Lock();
		m_JobList.push_front(job);
		m_pCondition->Signal();
	}
}