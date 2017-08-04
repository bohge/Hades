#pragma once
#include "timer/ITimerSystem.h"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"



#include <event2/event.h>
#include <event2/event_struct.h>
namespace hc
{
	class IThreadMutex;
}
namespace hlet
{
	struct TimeEvent;
	class TimerJob : public hc::IJob
	{
	private:
		typedef eastl::map<CallbackHandle, TimeEvent*> EventMap;
	private:
		struct event_base*		m_pBase;
		hc::IThreadMutex*		m_pMutex;
		struct event			m_InfinityEvent;
		EventMap				m_EventMap;
	public:
		TimerJob(void);
		~TimerJob(void);
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		void Initialize();
		void Exit();
		CallbackHandle RegisterTimer(float sec, uint64 userdata, hc::TimerCallback::Callback func);
		void UnregisterTimer(CallbackHandle h);
	};
}