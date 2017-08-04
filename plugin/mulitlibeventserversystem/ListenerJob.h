#pragma once
#include "network/server/IServerSystem.h"
#include "core/Recycleablevector.hpp"
#include "network/server/IServer.h"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"




#include <EASTL/vector.h>

struct event_base;
struct evconnlistener;
namespace hc{ class IServer; }
namespace hles
{
	class ConnectionJob;
	class LibeventConnection;
	class ListenerJob : public hc::IJob
	{
	private:
		enum Constant
		{
			LJC_PREPARE_CLIENTPOOL_SIZE = 10000,
		};
		typedef eastl::vector< hc::SmartPtr<ConnectionJob> >	ConnectionJobArray;
		typedef hc::Recycleablevector< hc::SmartPtr< LibeventConnection > > Clientpool;
	private:
		hc::ServerCallback				m_OnReceiveCallback;
		hc::IServer*					m_rpHost;
		Clientpool*						m_pClientpool;
		ConnectionJobArray*				m_pConnecSendArray;
		struct event_base*				m_pBase;
		struct evconnlistener*			m_pListener;
		hc::IServerSystem::ServerType	m_ServerType;
		uint							m_Idle;
		uint							m_Interval;
		uint							m_Count;
	public:
		ListenerJob(hc::IServer* h, hc::IServerSystem::ServerType st);
		virtual ~ListenerJob();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		void Initialize();
		ConnectionJob* GetFewestConnectionJob();
		void OnConnect(uint64 id);
		void OnDisconnect(uint64 id);
		bool SendBuffer(uint64 id, hc::IMessage* msg);
		void Broadcast(uint64 id, hc::IMessage* msg);
		void Disconnect(uint64 id);
		void SetSpecific(uint64 id, uint64 spec);
		uint64 GetSpecific(uint64 id);
		eastl::string GetIPAddress();
		bool StartListen(int port);
		hc::SmartPtr<ConnectionJob> MakeConnectionJob();
	public:
		HADES_FORCEINLINE uint64 PushClient(hc::SmartPtr< LibeventConnection >& data);
		HADES_FORCEINLINE bool HaveClient(uint64 id);
		HADES_FORCEINLINE void SetServerCallback(hc::ServerCallback::Callback func);
		HADES_FORCEINLINE void OnReceive(uint64 id, hc::IMessage* msg);
		HADES_FORCEINLINE hc::IServerSystem::ServerType GetServerType();
		HADES_FORCEINLINE void SetHeartBeat(uint idle, uint inter, uint count);
		HADES_FORCEINLINE uint GetHeartBeatIdle();
		HADES_FORCEINLINE uint GetHeartBeatInterval();
		HADES_FORCEINLINE uint GetHeartBeatCount();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 ListenerJob::PushClient(hc::SmartPtr< LibeventConnection >& data)
	{
		return m_pClientpool->Push(data);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ListenerJob::HaveClient(uint64 id)
	{
		return m_pClientpool->Have(id);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ListenerJob::SetServerCallback(hc::ServerCallback::Callback func)
	{
		m_OnReceiveCallback.Add(func);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ListenerJob::OnReceive(uint64 id, hc::IMessage* msg)
	{
		m_OnReceiveCallback.Multicast(m_rpHost, id, hc::CallbackType::SCT_REQUEST, msg);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IServerSystem::ServerType ListenerJob::GetServerType()
	{
		return m_ServerType;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ListenerJob::SetHeartBeat(uint idle, uint inter, uint count)
	{
		m_Idle = idle;
		m_Interval = inter;
		m_Count = count;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ListenerJob::GetHeartBeatIdle()
	{
		return m_Idle;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ListenerJob::GetHeartBeatInterval()
	{
		return m_Interval;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ListenerJob::GetHeartBeatCount()
	{
		return m_Count;
	}
}
