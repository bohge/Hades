#pragma once
#include "thread/IJobThreadArray.h"
#include "concurrentqueue.h"

#include <EASTL/list.h>
namespace hpts
{
	class WorkThread;
	class ThreadMutex;
	class IJobContainer;
	class JobThreadArray : public hc::IJobThreadArray
	{
	private:
		typedef eastl::vector< WorkThread* > ThreadArray;
	private:
		eastl::string				m_DispatchName;
		ThreadArray					m_ThreadArray;//全部的线程
	public:
		JobThreadArray(void);
		~JobThreadArray(void);
	public:
		virtual void SetName(const eastl::string &name);
		virtual void Pasue();
		virtual void Continue();
		virtual void Start();
		virtual void Stop();
		virtual void PushJob(byte index, hc::SmartPtr<hc::IJob> job);
	};
}

