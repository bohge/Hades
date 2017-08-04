#include "serialize/ISerializeSystem.h"
#include "network/message/IMessage.h"
#include "thread/IJobDispatcher.h"
#include "LibeventClientSystem.h"
#include "thread/IThreadSystem.h"
#include "LibeventClient.h"
#include "log/ILogSystem.h"


#include <event2/event_struct.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

using namespace hc;
using namespace eastl;
namespace hlec
{
	//---------------------------------------------------------------------------------------------------------
	static void RecycleFunction(void* obj)
	{
		LibeventClient* lc = static_cast<LibeventClient*>(obj);
		LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
		sys->FreeLibeventClient(lc);
	}
	//---------------------------------------------------------------------------------------------------------
	static void sendqueue_func(evutil_socket_t fd, short what, void *arg)
	{
		LibeventClientSystem* host = (LibeventClientSystem*)arg;
		host->ExecuteQueues();
	}
	//---------------------------------------------------------------------------------------------------------
	static void reconnectqueue_func(evutil_socket_t fd, short what, void *arg)
	{
		LibeventClientSystem* host = (LibeventClientSystem*)arg;
		host->ExecuteReconnect();
	}
	//---------------------------------------------------------------------------------------------------------
	void event_log_cb(int severity, const char *msg)
	{
		if (msg)
		{
			switch (severity) {
			case _EVENT_LOG_DEBUG: HADESLOG("DEBUG: %s", msg); break;
			case _EVENT_LOG_MSG:   HADESLOG("MSG: %s", msg);   break;
			case _EVENT_LOG_WARN:  HADESWARNING("%s", msg);  break;
			case _EVENT_LOG_ERR:   HADESERROR("%s", msg); break;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventClientSystem::LibeventClientSystem()
		:m_pClientThread(NULL)
		, m_pInfinityEvent(NEW event)
		, m_pReconnectEvent(NEW event)
		, m_spClientJob(SmartPtr<ClientJob>(NEW ClientJob))
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventClientSystem::~LibeventClientSystem()
	{
		SAFE_DELETE(m_pInfinityEvent);
		SAFE_DELETE(m_pReconnectEvent);
		m_spClientJob = SmartPtr<ClientJob>(NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::Initialize()
	{
		ISocketSystem::Initialize();
		//event_enable_debug_mode();
		//event_set_log_callback(event_log_cb);
		//evthread_use_depend_threads();
#ifdef WIN32
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);
#endif	

		m_pBase = event_base_new();

		{
			struct timeval tv;
			evutil_timerclear(&tv);
			tv.tv_sec = 0;
			tv.tv_usec = 1000;//每毫秒check一次堆栈防止有没有发出去的数据
			event_assign(m_pInfinityEvent, m_pBase, -1, EV_PERSIST, sendqueue_func, this);
			event_add(m_pInfinityEvent, &tv);
		}

		{
			struct timeval tv;
			evutil_timerclear(&tv);
			tv.tv_sec = 60;
			tv.tv_usec = 0;
			event_assign(m_pReconnectEvent, m_pBase, -1, EV_PERSIST, reconnectqueue_func, this);
			event_add(m_pReconnectEvent, &tv);
		}

		//通过ClientJob启动dispatch
		if (!m_pBase) {
			fprintf(stderr, "Could not initialize libevent!\n");
		}
		m_spClientJob->SetBase(*m_pBase);
		m_pClientThread =
			static_cast<IJobDispatcher*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
		m_pClientThread->SetName("Socket");
		m_pClientThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pClientThread->SetThreadCount(1);
		m_pClientThread->Start();
		m_pClientThread->PushJob(m_spClientJob);
		m_Socketpool.Initialize(SC_POOL_SIZE);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::Exit()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::Update()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	hc::SmartPtr<hc::ISocket> LibeventClientSystem::_DoSocketFactory(SocketType st)
	{
		uint64 gid = nullid;
		SmartPtr<ISocket> res;
		switch (st)
		{
		case hc::ISocketSystem::ST_COMMON_TL: 
			res = SmartPtr<ISocket>(m_LibeventClientlQueue.AllocObjcet(), &RecycleFunction);
			gid = m_Socketpool.Push(res);
			static_cast<LibeventClient*>(res.GetUnsafePointer())->SetBase(m_pBase);
			static_cast<LibeventClient*>(res.GetUnsafePointer())->SetGID(gid);
			static_cast<LibeventClient*>(res.GetUnsafePointer())->SetReferences(res);
			break;
		//case hc::ISocketSystem::ST_OPENSSL_TLS: res = NEW SSLLibeventClient(m_Openssl); break;
		default:ASSERT(false); break;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::_DoRecycleBin(hc::SmartPtr<hc::ISocket>& is)
	{
		LibeventClient* lc = static_cast<LibeventClient*>(is.GetUnsafePointer());
		uint64 gid = lc->GetGID();
		if (m_Socketpool.Have(gid))
		{
			m_Socketpool.Remove(gid);
		}
		is = SmartPtr <ISocket>(NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::PushSendQueue(uint64 id, hc::IMessage* msg)
	{
		IMessage* tosend = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		*tosend = *msg;
		m_SenderQueue.enqueue(make_pair(id, tosend));
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::_ExecuteSendQueue()
	{
		eastl::pair< uint64, hc::IMessage* > tosend;
		while (m_SenderQueue.try_dequeue(tosend))
		{
			if (HaveSocket(tosend.first))
			{
				hc::SmartPtr< LibeventClient > client = GetSocket(tosend.first);
				if (client.isValid())
				{
					client->SendBuffer(tosend.second->GetRawBuffer(), tosend.second->GetRawLength());
				}
			}
			ISerializeSystem::Instance()->FreeSerializable(tosend.second);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::PushConnectQueue(uint64 id, const eastl::string& ipport)
	{
		m_ConnectQueue.enqueue(make_pair(id, ipport));
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::_ExecuteConnectQueue()
	{
		eastl::pair< uint64, eastl::string > toconnect;
		while (m_ConnectQueue.try_dequeue(toconnect))
		{
			if (HaveSocket(toconnect.first))
			{
				hc::SmartPtr< LibeventClient > client = GetSocket(toconnect.first);
				if (client.isValid())
				{
					client->RealConnect(toconnect.second);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::_ExecuteDisconnectQueue()
	{
		uint64 id;
		while (m_DisconnectQueue.try_dequeue(id))
		{
			if (HaveSocket(id))
			{
				hc::SmartPtr< LibeventClient > client = GetSocket(id);
				if (client.isValid())
				{
					client->RealDisconnect();
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::PushDisconnectQueue(uint64 id)
	{
		m_DisconnectQueue.enqueue(id);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::ExecuteQueues()
	{
		_ExecuteSendQueue();
		_ExecuteDisconnectQueue();//服用，所以先disconnect
		_ExecuteConnectQueue();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::PushReconnectQueue(uint64 id)
	{
		m_ReconnectQueue.enqueue(id);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::ExecuteReconnect()
	{
		uint64 id;
		while (m_ReconnectQueue.try_dequeue(id))
		{
			if (HaveSocket(id))
			{
				hc::SmartPtr< LibeventClient > client = GetSocket(id);
				if (client.isValid())
				{
					client->Reconnect();
				}
			}
		}
	}
}