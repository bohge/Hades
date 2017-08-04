#include "thread/IJob.h"
#include "ThreadMutex.h"
#include "ThreadEvent.h"
#include "JobThread.h"
#include "JobResume.h"


using namespace hc;

namespace hpts
{
	//------------------------------------------------------------------------------------------------------
	JobThread::JobThread(byte index)
		:m_isLiveing(true)
		, m_rpUserdata(NULL)
		, m_pJobResume(NEW JobResume(this))
		, m_isWorking(false)
		, m_ThreadIndex(index)
	{
		m_pCondition = NEW ThreadEvent();
		m_pStopEvent = NEW ThreadEvent();
	}
	//------------------------------------------------------------------------------------------------------
	JobThread::~JobThread(void)
	{
		SAFE_DELETE(m_pCondition);
		SAFE_DELETE(m_pStopEvent);
		SAFE_DELETE(m_pJobResume);
	}
	//------------------------------------------------------------------------------------------------------
	void JobThread::_OnStart()
	{
	}
	//------------------------------------------------------------------------------------------------------
	bool JobThread::_StopWork()
	{
		m_pStopEvent->Lock();
		m_isLiveing = false;
		m_pCondition->Lock();
		if (m_isWorking
			&&m_Job.isValid())
		{
			m_Job->YieldJob();
		}
		m_pCondition->Signal();
		m_pStopEvent->Wait();
		return true;
	}
	//------------------------------------------------------------------------------------------------------
	void JobThread::_OnReset()
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------------------------------
	void* JobThread::DoWork()
	{
		if (!m_UserdataFactory.Empty())
		{
			m_rpUserdata = m_UserdataFactory.Unicast();
		}
		//m_pMutex->Unlock();
		while (m_isLiveing)
		{
			m_pCondition->Lock();
			if (!m_GetJobHandle.Unicast(m_ThreadIndex, m_Job))
			{
				m_isWorking = false;
				m_pCondition->Wait();
			}
			else
			{
				m_isWorking = true;
				m_pCondition->Unlock();
				m_Job->DoJob(m_Job, m_rpUserdata, m_pJobResume);
				m_Job = hc::SmartPtr<hc::IJob>(NULL);
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
	bool JobThread::Single()
	{
		bool res = false;
		m_pCondition->Lock();
		if (!m_isWorking)
		{
			res = true;
			m_pCondition->Signal();
		}
		else
		{
			m_pCondition->Unlock();
		}
		return res;
	}
	//------------------------------------------------------------------------------------------------------
	void JobThread::ResumeJob(hc::SmartPtr<hc::IJob>& job)
	{
		ASSERT(false);
	}
}