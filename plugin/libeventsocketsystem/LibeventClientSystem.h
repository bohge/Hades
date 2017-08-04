#pragma once
#include "network/socket/ISocketSystem.h"
#include "core/Recycleablevector.hpp"
#include "core/ObjectPool.hpp"




namespace hc
{
	class IJobDispatcher;
}
struct event;
struct event_base;
namespace hlec
{
	class ClientJob;
	class LibeventClient;
	class LibeventClientSystem : public hc::ISocketSystem
	{
	private:
		enum Constant
		{
			SC_POOL_SIZE = 1000,
		};
	private:
		typedef moodycamel::ConcurrentQueue< eastl::pair< uint64, hc::IMessage* > > SenderQueue;
		typedef moodycamel::ConcurrentQueue< eastl::pair< uint64, eastl::string > > ConnectQueue;
		typedef moodycamel::ConcurrentQueue< uint64 > DisconnectQueue;
		typedef moodycamel::ConcurrentQueue< uint64 > ReconnectQueue;
		typedef hc::Recycleablevector< hc::SmartPtr<LibeventClient> > Socketpool;

		typedef hc::ObjectPool< LibeventClient > LibeventClientlQueue;
	private:
		Socketpool					m_Socketpool;
		SenderQueue					m_SenderQueue;
		ConnectQueue				m_ConnectQueue;
		DisconnectQueue				m_DisconnectQueue;
		ReconnectQueue				m_ReconnectQueue;
		LibeventClientlQueue		m_LibeventClientlQueue;
		event*						m_pInfinityEvent;
		event*						m_pReconnectEvent;
		hc::SmartPtr<ClientJob>		m_spClientJob;
		hc::IJobDispatcher*			m_pClientThread;
		event_base*					m_pBase;
	public:
		LibeventClientSystem(void);
		virtual ~LibeventClientSystem(void);
	private:
		virtual hc::SmartPtr<hc::ISocket> _DoSocketFactory(SocketType type);
		virtual void _DoRecycleBin(hc::SmartPtr<hc::ISocket>& ct);
	public:
		virtual void Initialize();
		virtual void Exit();
		virtual void Update();
	private:
		void _ExecuteSendQueue();
		void _ExecuteConnectQueue();
		void _ExecuteDisconnectQueue();
	public:
		void PushDisconnectQueue(uint64 id);
		void PushSendQueue(uint64 id, hc::IMessage* msg);
		void PushConnectQueue(uint64 id, const eastl::string& ipport);
		void PushReconnectQueue(uint64 id);
		void ExecuteQueues();
		void ExecuteReconnect();
	public:
		HADES_FORCEINLINE bool HaveSocket(uint64 key);
		HADES_FORCEINLINE const hc::SmartPtr<LibeventClient>& GetSocket(uint64 key);
		HADES_FORCEINLINE void FreeLibeventClient(LibeventClient* lc);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool LibeventClientSystem::HaveSocket(uint64 key)
	{
		return m_Socketpool.Have(key);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const hc::SmartPtr<LibeventClient>& LibeventClientSystem::GetSocket(uint64 key)
	{
		return m_Socketpool.Get(key);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventClientSystem::FreeLibeventClient(LibeventClient* lc)
	{
		m_LibeventClientlQueue.FreeObject(lc);
	}
}

