#pragma once
#include "thread/IJobResume.h"


namespace hpts
{
	class WorkThread;
	class WorkResume : public hc::IJobResume
	{
	private:
		WorkThread*		m_rpJobThread;
	public:
		WorkResume(WorkThread* host);
		virtual ~WorkResume();
	public:
		virtual void Resumework(hc::SmartPtr<hc::IJob>& job);
	};
}
