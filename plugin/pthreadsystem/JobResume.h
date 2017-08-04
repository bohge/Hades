#pragma once
#include "thread/IJobResume.h"


namespace hpts
{
	class JobThread;
	class JobResume : public hc::IJobResume
	{
	private:
		JobThread*		m_rpJobThread;
	public:
		JobResume(JobThread* host);
		virtual ~JobResume();
	public:
		virtual void Resumework(hc::SmartPtr<hc::IJob>& job);
	};
}
