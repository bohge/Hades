#pragma once
#include "network/server/IServer.h"
#include "ListenerJob.h"


#include <EASTL/vector.h>
namespace hc
{
	class IJobDispatcher;
}
namespace hles
{
	class LibeventServer : public hc::IServer
	{
	private:
		hc::SmartPtr<ListenerJob>		m_spListenerJob;
		hc::IJobDispatcher*				m_pJobThread;
	public:
		LibeventServer(hc::IServerSystem::ServerType st);
		~LibeventServer(void);
	private:
		virtual bool _OnStart(int connectthread, int port);
		virtual void _OnStop();
	public:
		virtual void SetHeartBeat(uint idle, uint inter, uint count);
		virtual void SetServerCallback(hc::ServerCallback::Callback func);
		virtual bool SendBuffer(uint64 id, hc::IMessage* req);
		virtual void Disconnect(uint64 id);
		virtual void SetSpecific(uint64 id, uint64 spec);
		virtual uint64 GetSpecific(uint64 id);
		virtual void Broadcast(uint64 id, hc::IMessage* msg);
		virtual hc::IServerSystem::ServerType GetServerType();
	};
}
