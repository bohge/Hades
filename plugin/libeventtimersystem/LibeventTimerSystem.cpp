#include "thread/IJobDispatcher.h"
#include "thread/IThreadSystem.h"
#include "LibeventTimerSystem.h"
#include "TimerJob.h"



using namespace hc;
namespace hlet
{
	HADES_FORCEINLINE double _ToSecond(const struct timeval& input)
	{
		double s = input.tv_sec;
		double us = input.tv_usec;
		return s + us * 0.000001;
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventTimerSystem::LibeventTimerSystem()
		:m_pTimerThread(NULL)
		, m_spTimerJob(SmartPtr<TimerJob>(NEW TimerJob))
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventTimerSystem::~LibeventTimerSystem()
	{
		m_spTimerJob = SmartPtr<TimerJob>(NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventTimerSystem::Initialize()
	{
		struct timeval newtime;
		evutil_gettimeofday(&newtime, NULL);
		m_BeginTime = _ToSecond(newtime);

		ITimerSystem::Initialize();
		m_spTimerJob->Initialize();
		m_pTimerThread =
			static_cast<IJobDispatcher*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
		m_pTimerThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pTimerThread->SetThreadCount(1);
		m_pTimerThread->SetName("Timer");
		m_pTimerThread->Start();
		m_pTimerThread->PushJob(m_spTimerJob);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventTimerSystem::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventTimerSystem::Exit()
	{
		m_pTimerThread->Stop();
		m_spTimerJob->Exit();
		IThreadSystem::Instance()->RecycleBin(m_pTimerThread);
		ITimerSystem::Exit();
	}
	//---------------------------------------------------------------------------------------------------------
	CallbackHandle LibeventTimerSystem::RegisterTimer(float sec, uint64 userdata, hc::TimerCallback::Callback func)
	{
		return m_spTimerJob->RegisterTimer(sec, userdata, func);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventTimerSystem::UnregisterTimer(CallbackHandle h)
	{
		m_spTimerJob->UnregisterTimer(h);
	}
	//---------------------------------------------------------------------------------------------------------
	double LibeventTimerSystem::GetTimevalue()
	{
		struct timeval newtime;
		evutil_gettimeofday(&newtime, NULL);
		double now = _ToSecond(newtime);
		return now - m_BeginTime;
	}
}