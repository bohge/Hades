#pragma once
#include "thread/IThreadEvent.h"




namespace hpts
{
	class ThreadEvent : public hc::IThreadEvent
	{
	private:
		void*	m_pCondition;
		void*	m_pMutex;
	public:
		ThreadEvent(void);
		virtual ~ThreadEvent(void);
	public:
		virtual void Lock();
		virtual void Unlock();
		virtual void Signal();
		virtual void Wait();
		virtual void Wait(uint us);
	};
}
