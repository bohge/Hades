#include "thread/IThreadSystem.h"
#include "memory/IMemorySystem.h"
#include "thread/IThreadMutex.h"
#include "core/SmartPtr.hpp"
#include "TimeEvent.h"
#include "TimerJob.h"



#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>


using namespace hc;
using namespace eastl;
namespace hlet
{
	void cb_func(evutil_socket_t fd, short what, void *arg)
	{
		TimeEvent* et = static_cast<TimeEvent*>(arg);
		et->Callback.Unicast(et->Userdata);
	}
	void infinity_func(evutil_socket_t fd, short what, void *arg)
	{
		printf("infinity_func\r\n");
	}


	//---------------------------------------------------------------------------------------------------------
	TimerJob::TimerJob(void)
		:m_pBase(NULL)
		, m_pMutex(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	TimerJob::~TimerJob(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void TimerJob::_DoYieldJob()
	{
		event_base_loopexit(m_pBase, NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void TimerJob::Initialize()
	{
		evthread_use_depend_threads();
		event_set_mem_functions(
			&IMemorySystem::AllocMemory,
			&IMemorySystem::ReallocMemory,
			&IMemorySystem::ReleaseMemory
			);
#ifdef WIN32
		WORD wVersionRequested;
		WSADATA wsaData;

		wVersionRequested = MAKEWORD(2, 2);

		(void)WSAStartup(wVersionRequested, &wsaData);
#endif
		m_pBase = event_base_new();
		//没有EVLOOP_NO_EXIT_ON_EMPTY只能添加一个无限时间任务
		{
			struct timeval tv;
			evutil_timerclear(&tv);
			tv.tv_sec = (long)(((uint)-1) >> 3);
			event_assign(&m_InfinityEvent, m_pBase, -1, EV_PERSIST, infinity_func, NULL);
			event_add(&m_InfinityEvent, &tv);
		}
		m_pMutex =
			static_cast<IThreadMutex*>
			(IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_MUTEX));
		ASSERT(NULL != m_pBase);
	}
	//---------------------------------------------------------------------------------------------------------
	void TimerJob::Exit()
	{
		IThreadSystem::Instance()->RecycleBin(m_pMutex);
	}
	//---------------------------------------------------------------------------------------------------------
	void TimerJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		event_base_dispatch(m_pBase);
		event_base_free(m_pBase);
	}
	//---------------------------------------------------------------------------------------------------------
	CallbackHandle TimerJob::RegisterTimer(float sec, uint64 userdata, hc::TimerCallback::Callback func)
	{
		struct timeval tv;
		double si;
		float sf = modf(sec, &si);
		evutil_timerclear(&tv);
		tv.tv_sec = si;
		tv.tv_usec = sf * 1000000;
		TimeEvent* et = NEW TimeEvent;
		et->Userdata = userdata;
		CallbackHandle h =et->Callback.Add(func);
		event_assign(&et->Event, m_pBase, -1, EV_PERSIST, cb_func, et);
		event_add(&et->Event, &tv);
		m_pMutex->Lock();
		m_EventMap.insert(make_pair(h, et));
		m_pMutex->Unlock();
		return h;
	}
	//---------------------------------------------------------------------------------------------------------
	void TimerJob::UnregisterTimer(CallbackHandle h)
	{
		ASSERT(false);//需要删除事件，暂时没写
		m_pMutex->Lock();
		
		m_pMutex->Unlock();
	}
}
