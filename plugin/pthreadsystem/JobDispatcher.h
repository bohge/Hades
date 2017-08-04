#pragma once
#include "thread/IJobDispatcher.h"


#include <EASTL/vector.h>


namespace hpts
{
	class JobThread;
	class ThreadMutex;
	class IJobContainer;
	class JobDispatcher : public hc::IJobDispatcher
	{
	private:
		typedef eastl::vector< JobThread* > ThreadVector;
		typedef moodycamel::ConcurrentQueue< JobThread* > ThreadLockFreeQueue;
		typedef moodycamel::ConcurrentQueue< hc::SmartPtr<hc::IJob> > FixedJobQueue;
		typedef eastl::vector< FixedJobQueue* > FixedJobQueueVector;
	private:
		IJobContainer*				m_pIJobContainer;
		eastl::string				m_DispatchName;
		ThreadVector				m_ThreadVector;//全部的线程
		FixedJobQueueVector			m_FixedJobQueueVector;
		uint						m_Index;
	public:
		JobDispatcher(void);
		~JobDispatcher(void);
	private:
		void _TryDispatchJob(hc::SmartPtr<hc::IJob>& job);
	public:
		virtual void SetName(const eastl::string &name);
		virtual void Pasue();
		virtual void Continue();
		virtual void Start();
		virtual void Stop();
		virtual void PushJob(hc::SmartPtr<hc::IJob> job);
	public:
		bool GetJob(byte index, hc::SmartPtr<hc::IJob>& job);
	};
}

