#include "network/message/IMessageSystem.h"
#include "network/message/IRawbuffer.h"
#include "network/message/IMessage.h"
#include "LibeventConnection.h"




#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#ifndef _PLATFORM_WINDOWS
#include <netinet/tcp.h>
#endif


//---------------------------------------------------------------------------------------------------------
static void	conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
	if (events & BEV_EVENT_EOF
		|| events & BEV_EVENT_ERROR
		|| events & BEV_EVENT_TIMEOUT)
	{
		hles::LibeventConnection* client = (hles::LibeventConnection*)user_data;
		client->Disconnect();
	}
}
//---------------------------------------------------------------------------------------------------------
static void	conn_readcb(struct bufferevent *bev, void *user_data)
{
	hles::LibeventConnection* client = (hles::LibeventConnection*)user_data;
	client->OnReceive(bev);
}


using namespace hc;
namespace hles
{
	RTTI_TYPE_IMPLEMENT(LibeventConnection);
	//---------------------------------------------------------------------------------------------------------
	LibeventConnection::LibeventConnection(void)
		:m_pBufferevent(NULL)
		, m_rpHost(NULL)
		, m_pRawbuffer(IMessageSystem::Instance()->RawbufferFactory())
		, m_rpInputBuffer(NULL)
		, m_rpOutputBuffer(NULL)
		, m_Specific(nullid)
		, m_ID(nullid)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventConnection::~LibeventConnection(void)
	{
		bufferevent_free(m_pBufferevent);
		IMessageSystem::Instance()->RecycleBin(m_pRawbuffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventConnection::Initialize(ServerJob* host, struct event_base* base, evutil_socket_t fd)
	{
		m_rpHost = host;
		int val = 1;
		if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&val, sizeof(val)) < 0)
		{
			HADESERROR("set nodelay error\r\n");
		}
		if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&val, sizeof(val)) < 0)
		{
			HADESERROR("set keepalive error\r\n");
		}
#ifndef _PLATFORM_WINDOWS
		val = m_rpHost->GetHeartBeatIdle();
		if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, (const char*)&val, sizeof(val)) < 0)
		{
			HADESERROR("setsockopt TCP_KEEPIDLE error \r\n");
		}
		val = m_rpHost->GetHeartBeatInterval();
		if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, (const char*)&val, sizeof(val)) < 0)
		{
			HADESERROR("setsockopt TCP_KEEPINTVL error \r\n");
		}
		val = m_rpHost->GetHeartBeatInterval();
		if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, (const char*)&val, sizeof(val)) < 0)
		{
			HADESERROR("setsockopt TCP_KEEPCNT error \r\n");
		}
#endif
		m_pBufferevent = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
		if (!m_pBufferevent) {
			fprintf(stderr, "Error constructing bufferevent!");
			event_base_loopbreak(base);
			return;
		}		
		bufferevent_setcb(m_pBufferevent, conn_readcb, NULL, conn_eventcb, this);
		bufferevent_enable(m_pBufferevent, EV_WRITE | EV_READ);
		m_rpInputBuffer = bufferevent_get_input(m_pBufferevent);		
		m_rpOutputBuffer = bufferevent_get_output(m_pBufferevent);
		int res = evbuffer_enable_locking(m_rpInputBuffer, NULL);
		ASSERT(0 == res);
		res = evbuffer_enable_locking(m_rpOutputBuffer, NULL);
		ASSERT(0 == res);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventConnection::OnReceive(struct bufferevent* bev)
	{
		ASSERT(m_pBufferevent == bev);
		int len = evbuffer_get_length(m_rpInputBuffer);
		byte* buffer = m_pRawbuffer->NewBuffer(len);
		if (buffer)
		{
			bufferevent_read(m_pBufferevent, buffer, len);
			IMessage* msg;
			while (m_pRawbuffer->UnpackMessage(msg))
			{
				m_rpHost->OnReceive(m_ID, msg);
			}

		}
		if (!m_pRawbuffer->Windup())
		{
			m_rpHost->Disconnect(m_ID);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventConnection::SendBuffer(const byte* buf, uint size)
	{
		int res = bufferevent_write(m_pBufferevent, buf, size);
		return 0 == res;
	}
}