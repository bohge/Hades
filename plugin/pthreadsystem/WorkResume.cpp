#include "WorkThread.h"
#include "WorkResume.h"



namespace hpts
{

	//---------------------------------------------------------------------------------------------------------
	WorkResume::WorkResume(WorkThread* host)
		:m_rpJobThread(host)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	WorkResume::~WorkResume()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void WorkResume::Resumework(hc::SmartPtr<hc::IJob>& job)
	{
		m_rpJobThread->ResumeJob(job);
	}
}