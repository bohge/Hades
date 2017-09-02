#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "network/message/IMessage.h"
#include "memory/IMemorySystem.h"
#include "LibeventConnection.h"
#include "core/SmartPtr.hpp"
#include "ConnectionJob.h"




#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/event_struct.h>



using namespace hc;


namespace hles
{
	//---------------------------------------------------------------------------------------------------------
	void infinity_func(evutil_socket_t fd, short what, void *arg)
	{
		printf("infinity_func\r\n");
	}
	//---------------------------------------------------------------------------------------------------------	
	ConnectionJob::ConnectionJob()
		:m_pBase(NULL)
		, m_pConnectionCount(0)
		, m_ThreadIndex(nullhandle)
		, m_pInfinityEvent(NEW event)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ConnectionJob::~ConnectionJob()
	{
		SAFE_DELETE(m_pInfinityEvent);
	}
	//---------------------------------------------------------------------------------------------------------
	void ConnectionJob::_DoYieldJob()
	{
		event_base_loopexit(m_pBase, NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void ConnectionJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		event_base_dispatch(m_pBase);
		//event_base_loop(m_pBase, EVLOOP_NO_EXIT_ON_EMPTY);
		event_base_free(m_pBase);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ConnectionJob::Initialize(ListenerJob* host)
	{
		m_rpHost = host;
		struct sockaddr_in sin;
		m_pBase = event_base_new();
		if (!m_pBase)
		{
			fprintf(stderr, "Could not initialize libevent!\n");
			return false;
		}
		//没有EVLOOP_NO_EXIT_ON_EMPTY只能添加一个无限时间任务
		{
			struct timeval tv;
			evutil_timerclear(&tv);
			tv.tv_sec = (long)(((uint)-1) >> 3);
			event_assign(m_pInfinityEvent, m_pBase, -1, EV_PERSIST, infinity_func, NULL);
			event_add(m_pInfinityEvent, &tv);
		}
		return true;
	}
}