#include "network/server/IServerSystem.h"
#include "network/message/IMessage.h"
#include "LibeventConnection.h"
#include "core/SmartPtr.hpp"
#include "Connectionpool.h"
#include "ServerJob.h"

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>



using namespace hc;
namespace hles
{
	//---------------------------------------------------------------------------------------------------------
	static void RecycleLibeventConnection(void* obj)
	{
		Connectionpool::Instance()->FreeLibeventConnection(static_cast<LibeventConnection*>(obj));
	}
	//---------------------------------------------------------------------------------------------------------
	static void	listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
	{
		hles::ServerJob* server = (hles::ServerJob*)user_data;
		struct event_base *base = server->GetEventbase();
		hles::LibeventConnection* pclient = Connectionpool::Instance()->GetLibeventConnection();
		hc::SmartPtr< hles::LibeventConnection > clinet = hc::SmartPtr< hles::LibeventConnection >(pclient, &RecycleLibeventConnection);
		pclient->Initialize(server, base, fd);
		uint64 key = server->PushClient(clinet);
		pclient->SetIndex(key);
		pclient->OnConnect();
	}
	//---------------------------------------------------------------------------------------------------------
	ServerJob::ServerJob(IServer* h)
		:m_rpHost(h)
		, m_pBase(NULL)
		, m_pListener(NULL)
		, m_pClientpool(NULL)
		, m_Idle(60 * 5)
		, m_Interval(30)
		, m_Count(5)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ServerJob::~ServerJob(void)
	{
		ASSERT(NULL == m_pClientpool);
	}
	//---------------------------------------------------------------------------------------------------------
	void ServerJob::_DoYieldJob()
	{		
		//收发线程是卡死的，需要改成loop的
		//暂时不处理了，等需要退出的时候再说
		event_base_loopexit(m_pBase, NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ServerJob::Initialize(int port)
	{
		m_pClientpool = NEW Clientpool;
		m_pClientpool->Initialize(SJC_PREPARE_CLIENTPOOL_SIZE);
		struct sockaddr_in sin;
		m_pBase = event_base_new();
		if (!m_pBase) {
			fprintf(stderr, "Could not initialize libevent!\n");
			return false;
		}
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);

		m_pListener = evconnlistener_new_bind(m_pBase, listener_cb, (void *)this,
			LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE_PORT, -1,
			(struct sockaddr*)&sin,
			sizeof(sin));

		if (!m_pListener) {
			fprintf(stderr, "Could not create a listener!\n");
			return false;
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void ServerJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		m_OnReceiveCallback.Multicast(m_rpHost, hc::nullid, CallbackType::SCT_SERVER_START, NULL);
		event_base_dispatch(m_pBase);
		evconnlistener_free(m_pListener);
		event_base_free(m_pBase);
		SAFE_DELETE(m_pClientpool);
	}
	//---------------------------------------------------------------------------------------------------------
	void ServerJob::OnConnect(uint64 id)
	{
		if (HaveClient(id))
		{
			m_OnReceiveCallback.Multicast(m_rpHost, id, CallbackType::SCT_CONNECT, NULL);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool ServerJob::SendBuffer(uint64 id, const byte* buf, uint size)
	{
		bool res(false);
		if (HaveClient(id))
		{
			hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(id);
			if (client.isValid())
			{
				res = client->SendBuffer(buf, size);
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void ServerJob::Broadcast(uint64 id, hc::IMessage* msg)
	{
		for (int i = 0; i < m_pClientpool->Size(); ++i )
		{
			hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(i);
			if (client.isValid())
			{
				if (client->GetIndex() != id)
				{
					client->SendBuffer(msg->GetRawBuffer(), msg->GetRawLength());
					//hc::IMessage* newmsg = IServerSystem::Instance()->AllocMessage();
					//*newmsg = *msg;
					//m_OnReceiveCallback.Unicast(client->GetIndex(), client->GetUUID(), CallbackType::SCT_BROADCAST, newmsg);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ServerJob::Disconnect(uint64 id)
	{
		if (HaveClient(id))
		{
			hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(id);
			if (client.isValid())
			{
				client->DoDisconnect();
			}
			m_OnReceiveCallback.Multicast(m_rpHost, id, CallbackType::SCT_DISCONNECT, NULL);
			m_pClientpool->Remove(id);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ServerJob::SetSpecific(uint64 id, uint64 spec)
	{
		if (HaveClient(id))
		{
			hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(id);
			if (client.isValid())
			{
				client->SetSpecific(spec);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 ServerJob::GetSpecific(uint64 id)
	{
		uint64 res = nullid;
		if (HaveClient(id))
		{
			hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(id);
			if (client.isValid())
			{
				res = client->GetSpecific();
			}
		}
		return res;
	}

}
