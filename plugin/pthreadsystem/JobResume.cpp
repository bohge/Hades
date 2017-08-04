#include "JobThread.h"
#include "JobResume.h"



namespace hpts
{

	//---------------------------------------------------------------------------------------------------------
	JobResume::JobResume(JobThread* host)
		:m_rpJobThread(host)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	JobResume::~JobResume()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void JobResume::Resumework(hc::SmartPtr<hc::IJob>& job)
	{
		m_rpJobThread->ResumeJob(job);
	}
}