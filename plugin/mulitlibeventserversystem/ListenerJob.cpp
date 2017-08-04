#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "network/message/IMessage.h"
#include "serialize/ISerializable.h"
#include "LibeventConnection.h"
#include "core/SmartPtr.hpp"
#include "log/ILogSystem.h"
#include "Connectionpool.h"
#include "ConnectionJob.h"
#include "ListenerJob.h"



#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#ifndef _PLATFORM_WINDOWS
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string.h>
#define SOCKET_ERROR -1
#endif


using namespace hc;
using namespace eastl;

namespace hles
{
	static void RecycleLibeventConnection(void* obj)
	{
		Connectionpool::Instance()->FreeLibeventConnection(static_cast<LibeventConnection*>(obj));
	}
	//---------------------------------------------------------------------------------------------------------
	static void	listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
	{
		hles::ListenerJob* server = (hles::ListenerJob*)user_data;

		hles::LibeventConnection* pclient = Connectionpool::Instance()->GetLibeventConnection();
		hc::SmartPtr< hles::LibeventConnection > clinet = hc::SmartPtr< hles::LibeventConnection >(pclient, &RecycleLibeventConnection);
		ConnectionJob* connjob = server->GetFewestConnectionJob();
		connjob->Increase();
		uint64 key = server->PushClient(clinet);
		pclient->SetIndex(key);
		uint index = connjob->GetThreadIndex();
		pclient->SetThreadIndex(index);
		connjob->ScheduleConnect(clinet, fd);
		//HADESLOG("connection fd %d uuid %llu", fd, key);
	}
	//---------------------------------------------------------------------------------------------------------
	ListenerJob::ListenerJob(hc::IServer* h, hc::IServerSystem::ServerType st)
		:m_rpHost(h)
		, m_ServerType(st)
		, m_pBase(NULL)
		, m_pListener(NULL)
		, m_pClientpool(NULL)
		, m_pConnecSendArray(NULL)
		, m_Idle(60 * 5)
		, m_Interval(30)
		, m_Count(5)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ListenerJob::~ListenerJob()
	{
		ASSERT(NULL == m_pClientpool);
	}
	//---------------------------------------------------------------------------------------------------------
	void ListenerJob::_DoYieldJob()
	{		
		event_base_loopexit(m_pBase, NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void ListenerJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		m_OnReceiveCallback.Multicast(m_rpHost, hc::nullid, CallbackType::SCT_SERVER_START, NULL);
		event_base_dispatch(m_pBase);
		evconnlistener_free(m_pListener);
		event_base_free(m_pBase);
		SAFE_DELETE(m_pClientpool);
		SAFE_DELETE(m_pConnecSendArray);
	}
	//---------------------------------------------------------------------------------------------------------
	ConnectionJob* ListenerJob::GetFewestConnectionJob()
	{		
		ConnectionJob* res = m_pConnecSendArray->front().GetUnsafePointer();
		uint conncout = res->GetCount();
		for (int i = 1; i < m_pConnecSendArray->size(); ++i)
		{
			ConnectionJob* local = m_pConnecSendArray->at(i).GetUnsafePointer();
			if (local->GetCount() < conncout)
			{
				res = local;
				conncout = local->GetCount();
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void hles::ListenerJob::Initialize()
	{
		m_pClientpool = NEW Clientpool;
		m_pClientpool->Initialize(LJC_PREPARE_CLIENTPOOL_SIZE);
		m_pConnecSendArray = NEW ConnectionJobArray;
		m_pBase = event_base_new();
		if (!m_pBase) {
			HADESERROR("Could not initialize libevent!\n");
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool ListenerJob::StartListen(int port)
	{
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);

		m_pListener = evconnlistener_new_bind(m_pBase, listener_cb, (void *)this,
			LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
			(struct sockaddr*)&sin,
			sizeof(sin));

		if (!m_pListener) {
			HADESERROR("Could not create a listener on port %d!\n", port);
			return false;
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string ListenerJob::GetIPAddress()
	{
		eastl::string res;
#ifdef _PLATFORM_WINDOWS
		char hostname[256];
		if (gethostname(hostname, sizeof(hostname)) != SOCKET_ERROR)
		{
			struct hostent* host = gethostbyname(hostname);
			if (host != NULL)
			{
				strcpy(hostname, inet_ntoa(*(in_addr*)*host->h_addr_list));
				res = hostname;
			}
		}
#else
		struct ifaddrs * ifAddrStruct = NULL, * ifa = NULL;
		void * tmpAddrPtr = NULL;
		getifaddrs(&ifAddrStruct);
		for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
			if (ifa->ifa_addr->sa_family == AF_INET) { // Check it is IPv4
				char mask[INET_ADDRSTRLEN];
				void* mask_ptr = &((struct sockaddr_in*) ifa->ifa_netmask)->sin_addr;
				inet_ntop(AF_INET, mask_ptr, mask, INET_ADDRSTRLEN);
				if (strcmp(mask, "255.0.0.0") != 0) {
					// Is a valid IPv4 Address
					tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
					char addressBuffer[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
					res = addressBuffer;
				}
				else if (ifa->ifa_addr->sa_family == AF_INET6) { // Check it is
					// a valid IPv6 Address.

					// Do something
				}
			}
		}
		if (ifAddrStruct != NULL)
			freeifaddrs(ifAddrStruct);
#endif
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void ListenerJob::OnConnect(uint64 id)
	{
		if (HaveClient(id))
		{
			m_OnReceiveCallback.Multicast(m_rpHost, id, CallbackType::SCT_CONNECT, NULL);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ListenerJob::OnDisconnect(uint64 id)
	{
		m_OnReceiveCallback.Multicast(m_rpHost, id, CallbackType::SCT_DISCONNECT, NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ListenerJob::SendBuffer(uint64 id, hc::IMessage* msg)
	{
		bool res(false);
		if (NULL != msg && HaveClient(id))
		{
			hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(id);
			if (client.isValid())
			{
				//client->SendBuffer(msg->FullData(), msg->GetRawLength());
				uint index = client->GetThreadIndex();
				if (index >= 0 && index < m_pConnecSendArray->size())
				{
					ConnectionJob* target = m_pConnecSendArray->at(index).GetUnsafePointer();
					target->ScheduleSender(client, msg);
				}
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void ListenerJob::Broadcast(uint64 id, hc::IMessage* msg)
	{
		if (NULL != msg)
		{
			for (int i = 0; i < m_pClientpool->Size(); ++i)
			{
				hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(i);
				if (client.isValid())
				{
					if (client->GetIndex() != id)
					{
						//client->SendBuffer(msg->FullData(), msg->GetRawLength());
						uint index = client->GetThreadIndex();
						if (index > 0 && index < m_pConnecSendArray->size())
						{
							ConnectionJob* target = m_pConnecSendArray->at(index).GetUnsafePointer();
							target->ScheduleSender(client, msg);
						}
					}
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ListenerJob::Disconnect(uint64 id)
	{
		if (HaveClient(id))
		{
			hc::SmartPtr< LibeventConnection > client = m_pClientpool->Get(id);
			if (client.isValid())
			{
				uint index = client->GetThreadIndex();
				if (index >= 0 && index < m_pConnecSendArray->size())
				{
					ConnectionJob* target = m_pConnecSendArray->at(index).GetUnsafePointer();
					target->ScheduleDisconnect(client);
				}
			}
			m_pClientpool->Remove(id);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ListenerJob::SetSpecific(uint64 id, uint64 spec)
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
	uint64 ListenerJob::GetSpecific(uint64 id)
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
	//---------------------------------------------------------------------------------------------------------
	hc::SmartPtr<ConnectionJob> ListenerJob::MakeConnectionJob()
	{
		hc::SmartPtr<ConnectionJob> conn = hc::SmartPtr<ConnectionJob>(NEW ConnectionJob);
		if (!conn->Initialize(this))
		{
			conn = hc::SmartPtr<ConnectionJob>(NULL);
		}
		else
		{
			uint res = m_pConnecSendArray->size();
			m_pConnecSendArray->push_back(conn);
			conn->SetThreadIndex(res);
		}
		return conn;
	}
}