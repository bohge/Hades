#include "StdQueueContainer.hpp"
#include "JobThreadArray.h"
#include "JobThreadPool.h"
#include "ThreadMutex.h"
#include "thread/IJob.h"
#include "WorkThread.h"
#include "log/ILogSystem.h"



using namespace hc;
using namespace moodycamel;

namespace hpts
{
	//---------------------------------------------------------------------------------------------------------
	JobThreadArray::JobThreadArray(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	JobThreadArray::~JobThreadArray(void)
	{
		ASSERT(0 == m_ThreadArray.size());//必须停止
	}
	//---------------------------------------------------------------------------------------------------------
	void JobThreadArray::Start()
	{
		m_ThreadArray.reserve(GetThreadCount());
		for (int i = 0; i < GetThreadCount(); ++i)
		{
			WorkThread* wt = NEW WorkThread;
			wt->SetThreadDataCallback(_GetFactoryCallBack(), _GetRecycleCallBack());
			wt->SetSortType(GetSortType());
			wt->SetName(m_DispatchName);
			wt->Start();
			m_ThreadArray.push_back(wt);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void JobThreadArray::Stop()
	{
		FOR_EACH(ThreadArray, it, m_ThreadArray)
		{
			Object* ud = (*it)->GetUserdata();
			(*it)->Stop();
			SAFE_DELETE( *it );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void JobThreadArray::PushJob(byte index, hc::SmartPtr<hc::IJob> job)
	{
		switch (job->GetJobType())
		{
		case IJob::JT_SYNCHRONOUS: job->DoJob(job, NULL, NULL); break;//主线程没有现成相关数据，所以使用同步加载不能有特殊的线程数据,也不能被挂起
		case IJob::JT_ASYNCHRONOUS:
		{
			ASSERT(index < GetThreadCount());
			m_ThreadArray[index]->SetJob(job);
		}break;
		default: ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void JobThreadArray::Pasue()
	{
		HADESLOG("Empty JobDispatcher Pasue !\n");
	}
	//---------------------------------------------------------------------------------------------------------
	void JobThreadArray::Continue()
	{
		HADESLOG("Empty JobDispatcher Continue !\n");
	}
	//---------------------------------------------------------------------------------------------------------
	void JobThreadArray::SetName(const eastl::string &name)
	{
		m_DispatchName = name;
		FOR_EACH(ThreadArray, it, m_ThreadArray)
		{
			(*it)->SetName(name);
		}
	}

}