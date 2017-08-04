#pragma once
#include "network/server/IServerSystem.h"
#include "core/Recycleablevector.hpp"
#include "network/server/IServer.h"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"




struct event_base;
struct evconnlistener;
namespace hc{ class IServer; }
namespace hles
{
	class LibeventConnection;
	class ServerJob : public hc::IJob
	{
	private:
		enum Constant
		{
			SJC_PREPARE_CLIENTPOOL_SIZE = 10000,
		};
	private:
		typedef hc::Recycleablevector< hc::SmartPtr< LibeventConnection > > Clientpool;
	private:
		hc::ServerCallback		m_OnReceiveCallback;
		hc::IServer*			m_rpHost;
		Clientpool*				m_pClientpool;
		struct event_base*		m_pBase;
		struct evconnlistener*	m_pListener;
		uint					m_Idle;
		uint					m_Interval;
		uint					m_Count;
	public:
		ServerJob(hc::IServer* h);
		~ServerJob(void);
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		bool Initialize(int port);
		void OnConnect(uint64 id);
		bool SendBuffer(uint64 id, const byte* buf, uint size);
		void Broadcast(uint64 id, hc::IMessage* msg);
		void Disconnect(uint64 id);
		void SetSpecific(uint64 id, uint64 spec);
		uint64 GetSpecific(uint64 id);
	public:
		HADES_FORCEINLINE event_base* GetEventbase();
		HADES_FORCEINLINE uint64 PushClient(hc::SmartPtr< LibeventConnection >& data);
		HADES_FORCEINLINE bool HaveClient(uint64 id);
		HADES_FORCEINLINE void SetServerCallback(hc::ServerCallback::Callback func);
		HADES_FORCEINLINE void OnReceive(uint64 id, hc::IMessage* msg);
		HADES_FORCEINLINE void SetHeartBeat(uint idle, uint inter, uint count);
		HADES_FORCEINLINE uint GetHeartBeatIdle();
		HADES_FORCEINLINE uint GetHeartBeatInterval();
		HADES_FORCEINLINE uint GetHeartBeatCount();
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE event_base* ServerJob::GetEventbase()
	{
		return m_pBase;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 ServerJob::PushClient(hc::SmartPtr< LibeventConnection >& data)
	{
		return m_pClientpool->Push(data);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ServerJob::HaveClient(uint64 id)
	{
		return m_pClientpool->Have(id);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerJob::SetServerCallback(hc::ServerCallback::Callback func)
	{
		m_OnReceiveCallback.Add(func);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerJob::OnReceive(uint64 id, hc::IMessage* msg)
	{
		m_OnReceiveCallback.Multicast(m_rpHost, id, hc::CallbackType::SCT_REQUEST, msg);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerJob::SetHeartBeat(uint idle, uint inter, uint count)
	{
		m_Idle = idle;
		m_Interval = inter;
		m_Count = count;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ServerJob::GetHeartBeatIdle()
	{
		return m_Idle;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ServerJob::GetHeartBeatInterval()
	{
		return m_Interval;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ServerJob::GetHeartBeatCount()
	{
		return m_Count;
	}
}