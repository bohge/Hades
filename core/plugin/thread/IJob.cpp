#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "thread/IJobResume.h"
#include "thread/IJob.h"
#include "log/ILogSystem.h"

namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	IJob::IJob(void)
		:m_nMixFlag(0)
		, m_eAsynJob(JT_ASYNCHRONOUS)
		, m_pDojob(NULL)
		, m_pIJobResume(NULL)
		, m_isPending(false)
		, isDone(true)
	{
			m_pDojob =
				static_cast<IThreadMutex*>
				(IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	IJob::~IJob(void)
	{
		ASSERT(isDone);
		IThreadSystem::Instance()->RecycleBin(m_pDojob);
		m_pDojob = NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void IJob::DoJob(SmartPtr<IJob>& self, Object* userdata, IJobResume* resume)
	{
		m_pDojob->Lock();
		DEBUGCODE(isDone = false);
		ASSERT(false == m_isPending);
		_DoJob(self, userdata);
		if (m_isPending)
		{
			DEBUGCODE(IThreadSystem::Instance()->InsertPendingMap(self));
			m_PendingHolder = self;
			m_pIJobResume = resume;
		}
		DEBUGCODE(isDone = true);
		m_pDojob->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void IJob::YieldJob()
	{
		_DoYieldJob();
		m_pDojob->Lock();//如果卡在这里说明是在Dojob中调用了YieldJob
		m_pDojob->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void IJob::_Pending()
	{
		ASSERT(false == m_isPending);
		m_isPending = true;
	}
	//---------------------------------------------------------------------------------------------------------
	void IJob::_Resume()
	{
	
		m_pDojob->Lock();
		ASSERT(m_PendingHolder.isValid());
		m_isPending = false;
		m_pIJobResume->Resumework(m_PendingHolder);
		DEBUGCODE(IThreadSystem::Instance()->RemovePendingMap(m_PendingHolder));
		m_PendingHolder = SmartPtr<IJob>(NULL);
		m_pIJobResume = NULL;
		m_pDojob->Unlock();
	}

}
