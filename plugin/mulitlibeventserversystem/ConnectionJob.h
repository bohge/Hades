#pragma once
#include "network/server/IServerSystem.h"
#include "network/server/IServer.h"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"
#include "ListenerJob.h"


#include <event2/event.h>
#include <event2/bufferevent.h>

struct event;
struct event_base;
struct evconnlistener;
namespace hles
{
	class ConnectionJob : public hc::IJob
	{
	private:
		typedef eastl::pair< hc::SmartPtr< LibeventConnection >, evutil_socket_t> ConnectData;
		typedef eastl::pair< hc::SmartPtr< LibeventConnection >, hc::IMessage* >	SenderData;
		typedef moodycamel::ConcurrentQueue< SenderData > SenderQueue;
		typedef moodycamel::ConcurrentQueue< ConnectData > ConnectQueue;
		typedef moodycamel::ConcurrentQueue< hc::SmartPtr< LibeventConnection > > DisconnectQueue;
	private:
		struct event_base*		m_pBase;
		struct event*			m_pInfinityEvent;
		ListenerJob*			m_rpHost;
		SenderQueue*			m_pSenderSchedule;
		ConnectQueue*			m_pConnectQueue;
		DisconnectQueue*		m_pDisconnectSchedule;
		uint					m_ThreadIndex;
		volatile uint			m_pConnectionCount;
	public:
		ConnectionJob();
		virtual ~ConnectionJob();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		bool Initialize(ListenerJob* host);
		void ExecuteSchedule();
		void ScheduleSender(hc::SmartPtr< LibeventConnection >& lc, hc::IMessage* msg);
		void ScheduleConnect(hc::SmartPtr< LibeventConnection >& lc, evutil_socket_t fd);
		void ScheduleDisconnect(hc::SmartPtr< LibeventConnection >& lc);
	public:
		HADES_FORCEINLINE uint GetCount();
		HADES_FORCEINLINE void Increase();
		HADES_FORCEINLINE void Decrease();
		HADES_FORCEINLINE void SetThreadIndex(uint index);
		HADES_FORCEINLINE uint GetThreadIndex();
	};

	



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ConnectionJob::GetCount()
	{
		return m_pConnectionCount;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ConnectionJob::Increase()
	{
		++m_pConnectionCount;//不安全，但是无所谓
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ConnectionJob::Decrease()
	{
		--m_pConnectionCount;//不安全，但是无所谓
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ConnectionJob::SetThreadIndex(uint index)
	{
		m_ThreadIndex = index;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ConnectionJob::GetThreadIndex()
	{
		return m_ThreadIndex;
	}
}