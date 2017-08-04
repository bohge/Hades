#pragma once
#include "network/server/IServer.h"



namespace hc
{
	class IJobDispatcher;
}
namespace hles
{
	class ServerJob;
	class LibeventServer : public hc::IServer
	{
	private:
		hc::SmartPtr<ServerJob>		m_spServerJob;
		hc::IJobDispatcher*			m_pServerThread;
	public:
		LibeventServer(void);
		~LibeventServer(void);
	private:
		virtual bool _OnStart(int connectthread, int port);
		virtual void _OnStop();
	public:
		virtual void SetHeartBeat(uint idle, uint inter, uint count);
		virtual void SetServerCallback(hc::ServerCallback::Callback func);
		virtual bool SendBuffer(uint64 id, hc::IMessage* req);
		virtual bool SendBuffer(uint64 id, const byte* buf, uint size);
		virtual void Disconnect(uint64 id);
		virtual void SetSpecific(uint64 id, uint64 spec);
		virtual uint64 GetSpecific(uint64 id);
		virtual void Broadcast(uint64 id, hc::IMessage* msg);
		virtual hc::IServerSystem::ServerType GetServerType();
	private:
		eastl::string _GetIPAddress();
	};
}
