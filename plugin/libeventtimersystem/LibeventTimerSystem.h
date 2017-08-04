#pragma once
#include "timer/ITimerSystem.h"
#include "core/SmartPtr.hpp"

#include <event2/event.h>


namespace hc{ class IJobDispatcher; }
namespace hlet
{
	class TimerJob;
	class LibeventTimerSystem : public hc::ITimerSystem
	{
	private:
		double						m_BeginTime;
		hc::SmartPtr<TimerJob>		m_spTimerJob;
		hc::IJobDispatcher*			m_pTimerThread;
	public:
		LibeventTimerSystem();
		virtual ~LibeventTimerSystem();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
		virtual CallbackHandle RegisterTimer(float sec, uint64 userdata, hc::TimerCallback::Callback func);
		virtual void UnregisterTimer(CallbackHandle h);
		virtual double GetTimevalue();
	};
}