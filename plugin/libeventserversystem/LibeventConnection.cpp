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
		, m_rpInputBuffer(NULL)
		, m_rpOutputBuffer(NULL)
		, m_Specific(nullid)
		, m_ID(nullid)
	{

		m_pRawbuffer = IMessageSystem::Instance()->RawbufferFactory();
		m_pIThreadRWLock = static_cast<IThreadRWLock*>
			(IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventConnection::~LibeventConnection(void)
	{
		IMessageSystem::Instance()->RecycleBin(m_pRawbuffer);
		IThreadSystem::Instance()->RecycleBin(m_pIThreadRWLock);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventConnection::Initialize(ServerJob* host, struct event_base* base, evutil_socket_t fd)
	{
		m_pIThreadRWLock->WriteLock();
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
		m_pBufferevent = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
		if (!m_pBufferevent) {
			fprintf(stderr, "Error constructing bufferevent!");
			event_base_loopbreak(base);
			return;
		}		
		bufferevent_setcb(m_pBufferevent, conn_readcb, NULL, conn_eventcb, this);
		bufferevent_enable(m_pBufferevent, EV_WRITE | EV_READ);
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventConnection::OnReceive(struct bufferevent* bev)
	{
		m_pIThreadRWLock->ReadLock();
		ASSERT(m_pBufferevent == bev);
		size_t rawlen = evbuffer_get_length(bufferevent_get_input(m_pBufferevent));
		size_t maxbuffersize = IServerSystem::Instance()->GetMaxBufferSize();
		if (rawlen > maxbuffersize || rawlen <= 0)
		{
			m_pIThreadRWLock->Unlock();
			HADESERROR("Net try get buffer size(%d) is out of maximun(%d)", rawlen, maxbuffersize);
			m_rpHost->Disconnect(m_ID);
		}
		else
		{
			uint len = rawlen;
			byte* buffer = m_pRawbuffer->NewBuffer(len);
			if (!buffer)
			{
				m_pIThreadRWLock->Unlock();
			}
			else
			{
				bufferevent_read(m_pBufferevent, buffer, len);
				m_pIThreadRWLock->Unlock();
				IMessage* msg;
				//g_recived += len;
				while (m_pRawbuffer->UnpackMessage(msg))
				{
					//HADESLOG("LibeventConnection::OnReceive %d", g_recived);
					//g_recived = 0;
					m_rpHost->OnReceive(m_ID, msg);
				}
			}
			if (!m_pRawbuffer->Windup())//这个函数可以优化掉，当只发生完全接受后再修改m_Index，但是在极端情况下（一直粘包的情况）会导致buffer无限增大
			{
				m_pIThreadRWLock->Unlock();
				m_rpHost->Disconnect(m_ID);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventConnection::SendBuffer(const byte* buf, uint size)
	{
		m_pIThreadRWLock->ReadLock();
		int res = bufferevent_write(m_pBufferevent, buf, size);
		m_pIThreadRWLock->Unlock();
		return 0 == res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventConnection::DoDisconnect()
	{
		m_pIThreadRWLock->WriteLock();
		if (m_pBufferevent)
		{
			bufferevent_free(m_pBufferevent);
			m_pBufferevent = NULL;
			m_Specific = nullid;
			m_ID = nullid;
			m_pRawbuffer->Reset();
		}
		m_pIThreadRWLock->Unlock();
	}
}