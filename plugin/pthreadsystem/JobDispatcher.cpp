#include "StdQueueContainer.hpp"
#include "GreaterContainer.hpp"
#include "LessContaniner.hpp"
#include "QueueContainer.hpp"
#include "JobDispatcher.h"
#include "IJobContainer.h"
#include "ThreadMutex.h"
#include "thread/IJob.h"
#include "JobThread.h"
#include "log/ILogSystem.h"



using namespace hc;
using namespace moodycamel;

namespace hpts
{
	//---------------------------------------------------------------------------------------------------------
	JobDispatcher::JobDispatcher(void)
		: m_pIJobContainer(NULL)
		, m_Index(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	JobDispatcher::~JobDispatcher(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Start()
	{
		ASSERT(NULL == m_pIJobContainer);
		switch (GetSortType())
		{
		case ThreadProperty::ST_UNSORT: m_pIJobContainer = NEW QueueContainer; break;
		case ThreadProperty::ST_GREATER: m_pIJobContainer = NEW GreaterContainer; break;
		case ThreadProperty::ST_LESS: m_pIJobContainer = NEW LessContaniner; break;
		default: ASSERT(false); break;
		}
		m_ThreadVector.reserve(GetThreadCount());
		m_FixedJobQueueVector.reserve(GetThreadCount());
		for (int i = 0; i < GetThreadCount(); ++i)
		{
			JobThread* jt = NEW JobThread(i);
			m_ThreadVector.push_back(jt);
			m_FixedJobQueueVector.push_back(NEW FixedJobQueue);
			jt->SetGetJobHandle(MEMBER_FUNC_PTR(&JobDispatcher::GetJob));
			jt->SetThreadDataCallback(_GetFactoryCallBack(), _GetRecycleCallBack());
			jt->SetName(m_DispatchName);
		}
		for (int i = 0; i < GetThreadCount(); ++i)
		{
			JobThread* jt = m_ThreadVector[i];
			jt->Start();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Stop()
	{
		FOR_EACH(ThreadVector, it, m_ThreadVector)
		{
			(*it)->Stop();
			SAFE_DELETE( *it );
		}
		FOR_EACH(FixedJobQueueVector, queue, m_FixedJobQueueVector)
		{
			SAFE_DELETE(*queue);
		}
		m_ThreadVector.clear();
		SAFE_DELETE(m_pIJobContainer);
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::PushJob(hc::SmartPtr<hc::IJob> job)
	{
		switch (job->GetJobType())
		{
		case IJob::JT_SYNCHRONOUS: job->DoJob(job, NULL, NULL); break;//主线程没有现成相关数据，所以使用同步加载不能有特殊的线程数据,也不能被挂起
		case IJob::JT_ASYNCHRONOUS: _TryDispatchJob(job); break;
		default: ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::_TryDispatchJob(hc::SmartPtr<hc::IJob>& job)
	{
		if (job->isFixedThread())
		{
			byte index = job->GetThreadIndex();
			JobThread* jt = m_ThreadVector[index];
			FixedJobQueue* queue = m_FixedJobQueueVector[index];
			queue->enqueue(job);
			jt->Single();
		}
		else
		{
			uint beginindex = m_Index;
			uint size = m_ThreadVector.size();
			uint nextindex = m_Index;
			++nextindex;
			m_Index = nextindex < size ? nextindex : 0;
			m_pIJobContainer->PushJob(job);
			for (int i = 0; i < size; ++i)
			{
				uint localindex = (i + beginindex) % size;
				JobThread* jt = m_ThreadVector[localindex];
				if (jt->Single())
				{
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool JobDispatcher::GetJob(byte index, hc::SmartPtr<hc::IJob>& job)
	{
		bool res = true;
		FixedJobQueue* queue = m_FixedJobQueueVector[index];
		if (!queue->try_dequeue(job))
		{
			res = m_pIJobContainer->TryPopJob(job);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Pasue()
	{
		HADESLOG("Empty JobDispatcher Pasue !\n");
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::Continue()
	{
		HADESLOG("Empty JobDispatcher Continue !\n");
	}
	//---------------------------------------------------------------------------------------------------------
	void JobDispatcher::SetName(const eastl::string &name)
	{
		m_DispatchName = name;
		FOR_EACH(ThreadVector, it, m_ThreadVector)
		{
			(*it)->SetName(name);
		}
	}
}