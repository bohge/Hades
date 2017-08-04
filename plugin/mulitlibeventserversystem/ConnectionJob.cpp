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
	static void sendqueue_func(evutil_socket_t fd, short what, void *arg)
	{
		ConnectionJob* host = (ConnectionJob*)arg;
		host->ExecuteSchedule();
	}
	//---------------------------------------------------------------------------------------------------------	
	ConnectionJob::ConnectionJob()
		:m_pBase(NULL)
		, m_pConnectionCount(0)
		, m_ThreadIndex(nullhandle)
		, m_pInfinityEvent(NEW event)
		, m_pSenderSchedule(NEW SenderQueue)
		, m_pDisconnectSchedule(NEW DisconnectQueue)
		, m_pConnectQueue(NEW ConnectQueue)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ConnectionJob::~ConnectionJob()
	{
		SAFE_DELETE(m_pInfinityEvent);
		SAFE_DELETE(m_pSenderSchedule);
		SAFE_DELETE(m_pConnectQueue);
		SAFE_DELETE(m_pDisconnectSchedule);
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
		{
			struct timeval tv;
			evutil_timerclear(&tv);
			tv.tv_sec = 0;
			tv.tv_usec = 10000;//每十毫秒check一次堆栈防止有没有发出去的数据
			event_assign(m_pInfinityEvent, m_pBase, -1, EV_PERSIST, sendqueue_func, this);
			event_add(m_pInfinityEvent, &tv);
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void ConnectionJob::ExecuteSchedule()
	{
		ConnectData tocon;
		while (m_pConnectQueue->try_dequeue(tocon))
		{
			tocon.first->Connect(m_rpHost, this, m_pBase, tocon.second);
		}
		SenderData tosend;
		while (m_pSenderSchedule->try_dequeue(tosend))
		{
			if (tosend.first.isValid())
			{
				tosend.first->SendBuffer(tosend.second->GetRawBuffer(), tosend.second->GetRawLength());
			}
			ISerializeSystem::Instance()->FreeSerializable(tosend.second);
		}
		hc::SmartPtr< LibeventConnection > todis;
		while (m_pDisconnectSchedule->try_dequeue(todis))
		{
			todis->Disconnect();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ConnectionJob::ScheduleSender(hc::SmartPtr< LibeventConnection >& lc, hc::IMessage* msg)
	{
		IMessage* tosend = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		*tosend = *msg;
		m_pSenderSchedule->enqueue(eastl::make_pair(lc, tosend));
	}
	//---------------------------------------------------------------------------------------------------------
	void ConnectionJob::ScheduleDisconnect(hc::SmartPtr< LibeventConnection >& lc)
	{
		m_pDisconnectSchedule->enqueue(lc);
	}
	//---------------------------------------------------------------------------------------------------------
	void ConnectionJob::ScheduleConnect(hc::SmartPtr< LibeventConnection >& lc, evutil_socket_t fd)
	{
		m_pConnectQueue->enqueue(eastl::make_pair(lc, fd));
	}
}