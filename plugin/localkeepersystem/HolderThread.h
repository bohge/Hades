#pragma once
#include "thread/IJobDispatcher.h"
#include "LocalKeeperSystem.h"
#include "core/Singleton.hpp"
#include "core/SmartPtr.hpp"
#include "concurrentqueue.h"
#include "CopyJob.h"
#include "SaveJob.h"


namespace hc
{
	class Accessor;
	class IJobThreadArray;
}
namespace hlks
{	
	class HolderThread
	{
		SINGLETON_DECLARE(HolderThread);
	private:
		typedef moodycamel::ConcurrentQueue< hc::SmartPtr<CopyJob> > CopyJobQueue;
		typedef moodycamel::ConcurrentQueue< hc::SmartPtr<SaveJob> > SaveJobQueue;
	public:
		CopyJobQueue				m_CopyJobQueue;
		SaveJobQueue				m_SaveJobQueue;
		hc::IJobDispatcher*			m_pIJobDispatcher;
		RecycleCallback::Callback	m_pCopyJobCallback;
		RecycleCallback::Callback	m_pSaveJobCallback;
	private:
		HolderThread();
		~HolderThread();
	public:
		void OnFreeCopyJob(hc::SmartPtr<hc::IJob>& job);
		void OnFreeSaveJob(hc::SmartPtr<hc::IJob>& job);
	public:
		HADES_FORCEINLINE void PushJob(hc::SmartPtr<hc::IJob> job);
		HADES_FORCEINLINE hc::SmartPtr<CopyJob> GetCopyJob();
		HADES_FORCEINLINE hc::SmartPtr<SaveJob> GetSaveJob();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void HolderThread::PushJob(hc::SmartPtr<hc::IJob> job)
	{
		m_pIJobDispatcher->PushJob(job);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::SmartPtr<CopyJob> HolderThread::GetCopyJob()
	{
		hc::SmartPtr<CopyJob> res;
		if (!m_CopyJobQueue.try_dequeue(res))
		{
			res = hc::SmartPtr<CopyJob>(NEW CopyJob);
			res->SetJobRecycle(m_pCopyJobCallback);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::SmartPtr<SaveJob> HolderThread::GetSaveJob()
	{
		hc::SmartPtr<SaveJob> res;
		if (!m_SaveJobQueue.try_dequeue(res))
		{
			res = hc::SmartPtr<SaveJob>(NEW SaveJob);
			res->SetJobRecycle(m_pSaveJobCallback);
		}
		return res;
	}
}
