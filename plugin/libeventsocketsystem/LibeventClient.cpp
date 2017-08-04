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
		sys->ExecuteQueues();
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
		, m_pRawbuffer(IMessageSystem::Instance()->RawbufferFactory())
		, m_rpInputBuffer(NULL)
		, m_rpOutputBuffer(NULL)
		, m_isSuccess(false)
		, m_isDisconnect(false)
		, m_rpBase(NULL)		
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventClient::~LibeventClient()
	{
		IMessageSystem::Instance()->RecycleBin(m_pRawbuffer);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventClient::OnStart()
	{
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::_DoConnect(const eastl::string& ipport)
	{
		m_isDisconnect = false;
		LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
		sys->PushConnectQueue(m_Gid, ipport);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::Reconnect()
	{
		if (m_pBufferevent)
		{
			if (bufferevent_socket_connect(m_pBufferevent, (struct sockaddr *)&m_sSin, sizeof(m_sSin)) != 0)
			{
				bufferevent_free(m_pBufferevent);
				HADESERROR("3.%s\r\n", "error connect");
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::ConnectReslut(bool res)
	{
		if (!m_isSuccess)
		{
			m_isSuccess = res;
			if (m_isSuccess && !m_isDisconnect)
			{
				evutil_socket_t fd = bufferevent_getfd(m_pBufferevent);
				//HADESLOG("socket file disc %d", fd);
				int enable = 1;
				//evutil_make_socket_nonblocking(fd);
				if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable)) < 0)
				{
					HADESERROR("set nodelay error\r\n");
				}
				_OnConnected();
			}
			else
			{
				if (!m_isDisconnect)
				{
					LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
					sys->PushReconnectQueue(m_Gid);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::_DoDisconnect()
	{
		if (!m_isDisconnect)
		{
			m_isDisconnect = true;
			LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
			sys->PushDisconnectQueue(m_Gid);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::_DoSend(hc::IMessage* msg)
	{
		LibeventClientSystem* sys = static_cast<LibeventClientSystem*>(ISocketSystem::Instance());
		sys->PushSendQueue(m_Gid, msg);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::RealConnect(const eastl::string& ipport)
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
		m_pBufferevent = (bufferevent_socket_new(m_rpBase, -1, BEV_OPT_CLOSE_ON_FREE));
#ifdef ENVIRONMENT32
		error("(void*)m_Gid not work");
#endif
		bufferevent_setcb(m_pBufferevent, client_readcb, NULL, client_eventcb, (void*)m_Gid);
		bufferevent_enable(m_pBufferevent, EV_READ | EV_WRITE);

		m_rpInputBuffer = bufferevent_get_input(m_pBufferevent);
		m_rpOutputBuffer = bufferevent_get_output(m_pBufferevent);
		//int res = evbuffer_enable_locking(m_rpInputBuffer, NULL);
		//if (0 != res)
		//{
		//	HADESERROR("1.%s\r\n", "error connect");
		//}
		//res = evbuffer_enable_locking(m_rpOutputBuffer, NULL);
		//if (0 != res)
		//{
		//	HADESERROR("2.%s\r\n", "error connect");
		//}

		Reconnect();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::SendBuffer(const byte* buf, uint size)
	{
		//HADESLOG("_DoSend len %d", size
		if (m_pBufferevent)
		{
			if (-1 == bufferevent_write(m_pBufferevent, buf, size))
			{
				HADESLOG("_DoSend error");
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::OnReceive(struct bufferevent* bev)
	{
		//HADESLOG("LibeventClient::OnReceive");
		ASSERT(m_pBufferevent == bev);
		int len = evbuffer_get_length(m_rpInputBuffer);
		byte* buffer = m_pRawbuffer->NewBuffer(len);
		if (buffer)
		{
			bufferevent_read(bev, buffer, len);
			IMessage* msg;
			while (m_pRawbuffer->UnpackMessage(msg))
			{
				_OnReciveMessage(msg);
			}
		}
		m_pRawbuffer->Windup();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventClient::RealDisconnect()
	{
		m_isSuccess = false;
		if (m_pBufferevent)
		{
			evutil_socket_t fd = bufferevent_getfd(m_pBufferevent);
			bufferevent_setcb(m_pBufferevent, NULL, NULL, NULL, this);
			int res = evutil_closesocket(fd);
			bufferevent_free(m_pBufferevent);
			m_pBufferevent = NULL;
			m_SelfReferences = SmartPtr<ISocket>(NULL);
		}
		//HADESLOG("_DoDisconnect fd %d res %d", fd, res);
	}
}