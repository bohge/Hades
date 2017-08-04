#include "network/message/IMessage.h"
#include "thread/IJobDispatcher.h"
#include "thread/IThreadSystem.h"
#include "LibeventConnection.h"
#include "LibeventServer.h"
#include "ServerJob.h"
#include "log/ILogSystem.h"

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
namespace hles
{
	//---------------------------------------------------------------------------------------------------------
	LibeventServer::LibeventServer(void)
		:m_pServerThread(NULL),
		m_spServerJob(SmartPtr<ServerJob>(NEW ServerJob(this)))
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventServer::~LibeventServer(void)
	{
		m_spServerJob = SmartPtr<ServerJob>(NULL);
		ASSERT(NULL == m_pServerThread);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventServer::_OnStart(int connectthread, int port)
	{
		bool res = m_spServerJob->Initialize(port);
		if (res)
		{
			m_pServerThread =
				static_cast<IJobDispatcher*>(
				IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
			m_pServerThread->SetSortType(ThreadProperty::ST_UNSORT);
			m_pServerThread->SetThreadCount(1);
			m_pServerThread->SetName("Server");
			m_pServerThread->Start();
			m_pServerThread->PushJob(m_spServerJob);
		}
		_SetServerAddress(_GetIPAddress());
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::_OnStop()
	{
		m_pServerThread->Stop();
		IThreadSystem::Instance()->RecycleBin(m_pServerThread);
		m_pServerThread = NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::SetServerCallback(hc::ServerCallback::Callback func)
	{
		m_spServerJob->SetServerCallback(func);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::Broadcast(uint64 id, hc::IMessage* msg)
	{
		m_spServerJob->Broadcast(id, msg);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventServer::SendBuffer(uint64 id, IMessage* req)
	{
		return m_spServerJob->SendBuffer(id, req->GetRawBuffer(), req->GetRawLength());
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventServer::SendBuffer(uint64 id, const byte* buf, uint size)
	{
		return m_spServerJob->SendBuffer(id, buf, size);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::Disconnect(uint64 id)
	{
		m_spServerJob->Disconnect(id);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::SetSpecific(uint64 id, uint64 spec)
	{
		m_spServerJob->SetSpecific(id, spec);
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LibeventServer::GetSpecific(uint64 id)
	{
		return m_spServerJob->GetSpecific(id);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IServerSystem::ServerType LibeventServer::GetServerType()
	{
		return IServerSystem::ST_COMMON_TL;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::SetHeartBeat(uint idle, uint inter, uint count)
	{
		m_spServerJob->SetHeartBeat(idle, inter, count);
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string LibeventServer::_GetIPAddress()
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
		struct ifaddrs * ifAddrStruct = NULL, *ifa = NULL;
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
}