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
#include <signal.h>

using namespace hc;
using namespace eastl;
namespace hlec
{
	//---------------------------------------------------------------------------------------------------------
	void infinity_func(evutil_socket_t fd, short what, void *arg)
	{
		printf("infinity_func\r\n");
	}
	static void RecycleFunction(void* obj)
	{
		LibeventClient* lc = static_cast<LibeventClient*>(obj);
		LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
		sys->FreeLibeventClient(lc);
	}
	//---------------------------------------------------------------------------------------------------------
	void event_log_cb(int severity, const char *msg)
	{
		if (msg)
		{
			switch (severity) {
			case _EVENT_LOG_DEBUG: fprintf(stderr, "DEBUG: %s\t\n", msg); break;
			case _EVENT_LOG_MSG:   fprintf(stderr, "MSG: %s\t\n", msg);   break;
			case _EVENT_LOG_WARN:  fprintf(stderr, "%s\t\n", msg);  break;
			case _EVENT_LOG_ERR:   fprintf(stderr, "%s\t\n", msg); break;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventClientSystem::LibeventClientSystem()
		:m_pClientThread(NULL)
		, m_pInfinityEvent(NEW event)
		, m_spClientJob(SmartPtr<ClientJob>(NEW ClientJob))
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventClientSystem::~LibeventClientSystem()
	{
		SAFE_DELETE(m_pInfinityEvent);
		m_spClientJob = SmartPtr<ClientJob>(NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClientSystem::Initialize()
	{
		ISocketSystem::Initialize();
		//event_enable_debug_mode();
		//event_set_log_callback(event_log_cb);
		//event_enable_debug_logging(EVENT_DBG_ALL);
		evthread_use_depend_threads();
#ifdef WIN32
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);
#else
		signal(SIGPIPE, SIG_IGN);
#endif
		m_pBase = event_base_new();

		{
			struct timeval tv;
			evutil_timerclear(&tv);
			tv.tv_sec = (long)(((uint)-1) >> 3);
			event_assign(m_pInfinityEvent, m_pBase, -1, EV_PERSIST, infinity_func, NULL);
			event_add(m_pInfinityEvent, &tv);
		}

		//Í¨¹ýClientJobÆô¶¯dispatch
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
}