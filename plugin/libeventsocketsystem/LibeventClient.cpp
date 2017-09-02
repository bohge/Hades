#include "network/message/IMessageSystem.h"
#include "network/message/IRawbuffer.h"
#include "network/message/IMessage.h"
#include "thread/IJobDispatcher.h"
#include "thread/IThreadSystem.h"
#include "LibeventClientSystem.h"
#include "thread/IThreadEvent.h"
#include "log/ILogSystem.h"
#include "LibeventClient.h"

#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/event_struct.h>
#ifndef _PLATFORM_WINDOWS
#include <netinet/tcp.h>
#endif


#define SLEEP_SECOND 5
using namespace hc;
namespace hlec
{
	//---------------------------------------------------------------------------------------------------------
	void client_readcb(bufferevent *bev, void *user_data)
	{
		uint64 gid = (uint64)user_data;
		LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
		if (sys->HaveSocket(gid))
		{
			SmartPtr<LibeventClient> sk = sys->GetSocket(gid);
			if (sk.isValid())
			{
				sk->OnReceive(bev);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void client_eventcb(bufferevent *bev, short events, void *user_data)
	{
		bool res = false;
		if (events & BEV_EVENT_CONNECTED)
		{
			res = true;
		}
		if (events & BEV_EVENT_EOF)
		{
			int code = EVUTIL_SOCKET_ERROR();
			HADESERROR("Connection closed, events %d code %d", events, code);
		}
		else if (events & BEV_EVENT_ERROR)
		{
			int code = EVUTIL_SOCKET_ERROR();
			HADESERROR("Connection error, events %d code %d", events, code);
		}
		uint64 gid = (uint64)user_data;
		LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
		if (sys->HaveSocket(gid))
		{
			SmartPtr<LibeventClient> sk = sys->GetSocket(gid);
			if (sk.isValid())
			{
				sk->ConnectReslut(res);
			}
		}
	}

	//---------------------------------------------------------------------------------------------------------
	LibeventClient::LibeventClient()
		:m_Port(0)
		, m_rpBase(NULL)
	{
		m_pRawbuffer = IMessageSystem::Instance()->RawbufferFactory();
		m_pIThreadRWLock = static_cast<IThreadRWLock*>
			(IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventClient::~LibeventClient()
	{
		IMessageSystem::Instance()->RecycleBin(m_pRawbuffer);
		IThreadSystem::Instance()->RecycleBin(m_pIThreadRWLock);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventClient::OnStart()
	{
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::_DoConnect(const eastl::string& ipport)
	{
		int find_pos = ipport.find(":");
		int len = ipport.length();
		_SetIP(ipport.substr(0, find_pos));
		eastl::string port = ipport.substr(find_pos + 1, len);
		_SetPort(std::atoi(port.data()));

		memset(&m_sSin, 0, sizeof(m_sSin));
		m_sSin.sin_family = AF_INET;
		m_sSin.sin_port = htons(m_Port);
		//sin.sin_addr.s_addr = inet_addr(m_IP.c_str());
#ifdef _PLATFORM_WINDOWS
		inet_pton(AF_INET, m_IP.c_str(), (void*)&m_sSin.sin_addr.S_un.S_addr);
#else
		inet_pton(AF_INET, m_IP.c_str(), (void*)&m_sSin.sin_addr.s_addr);
#endif
		//memcpy(sin.sa_data, m_IP.c_str(), sizeof(sin.sa_data));

		//m_rpBase = event_base_new();
		m_pBufferevent = (bufferevent_socket_new(m_rpBase, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE));
#ifdef ENVIRONMENT32
		error("(void*)m_Gid not work");
#endif
		bufferevent_setcb(m_pBufferevent, client_readcb, NULL, client_eventcb, (void*)m_Gid);
		bufferevent_enable(m_pBufferevent, EV_READ | EV_WRITE);
		m_pIThreadRWLock->WriteLock();
		if (bufferevent_socket_connect(m_pBufferevent, (struct sockaddr *)&m_sSin, sizeof(m_sSin)) != 0)
		{
			bufferevent_free(m_pBufferevent);
			HADESERROR("3.%s\r\n", "error connect");
		}
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::ConnectReslut(bool res)
	{
		if (res)
		{
			int enable = 1;
			evutil_socket_t fd = bufferevent_getfd(m_pBufferevent);
			//HADESLOG("socket file disc %d", fd);
			//evutil_make_socket_nonblocking(fd);
			if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable)) < 0)
			{
				HADESERROR("set nodelay error on fd %d, error code %d\r\n", fd, EVUTIL_SOCKET_ERROR());
			}
			_OnConnected();
		}
		else
		{
			_OnDisconnected();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::_DoDisconnect()
	{
		m_pIThreadRWLock->WriteLock();
		if (m_pBufferevent)
		{
			bufferevent_free(m_pBufferevent);
			m_pBufferevent = NULL;
			m_SelfReferences = SmartPtr<ISocket>(NULL);
		}
		//HADESLOG("_DoDisconnect fd %d res %d", fd, res);
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::_DoSend(hc::IMessage* msg)
	{
		//HADESLOG("_DoSend len %d", size
		m_pIThreadRWLock->ReadLock();
		if (m_pBufferevent)
		{
			if (-1 == bufferevent_write(m_pBufferevent, msg->GetRawBuffer(), msg->GetRawLength()))
			{
				HADESLOG("_DoSend error");
			}
		}
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::OnReceive(struct bufferevent* bev)
	{
		//HADESLOG("LibeventClient::OnReceive");
		m_pIThreadRWLock->ReadLock();
		ASSERT(m_pBufferevent == bev);
		int len = evbuffer_get_length(bufferevent_get_input(m_pBufferevent));
		byte* buffer = m_pRawbuffer->NewBuffer(len);
		if (!buffer)
		{
			m_pIThreadRWLock->Unlock();
		}
		else
		{
			bufferevent_read(bev, buffer, len);
			m_pIThreadRWLock->Unlock();
			IMessage* msg;
			while (m_pRawbuffer->UnpackMessage(msg))
			{
				_OnReciveMessage(msg);
			}
		}
		m_pRawbuffer->Windup();
	}
}