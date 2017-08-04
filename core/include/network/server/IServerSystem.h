#pragma once
#include "network/SpecificDefined.h"
#include "network/RequestHandle.h"
#include "core/Singleton.hpp"
#include "core/Delegate.hpp"
#include "core/Configure.h"
#include "core/IPlugin.h"



namespace hc
{
	class IMessage;
	class IServer;
	class ServerProxy;
	class IThreadRWLock;
	class HADES_CORE_API IServerSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IServerSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IServerSystem, IPlugin);
	public:
		enum ServerType
		{
			ST_COMMON_TL = 0,
			ST_OPENSSL_TLS,
		};
	private:
		typedef eastl::pair<IServer*, volatile int> ServerPair;
		typedef eastl::vector< ServerPair* > IServerArray;
	private:
		IServerArray			m_IServerArray;
		IThreadRWLock*			m_pIServerRWLock;
		uint					m_MaxBufferSize;
	public:
		IServerSystem();
		virtual ~IServerSystem();
	private:
		virtual IServer* _DoServerFactory(ServerType st) const = 0;
		virtual void _DoRecycleBin(IServer* is) const = 0;
	public:
		virtual void Initialize();
		virtual void Exit();
	public:
		ServerProxy* ServerFactory(ServerType st);
		ServerProxy* AllocServerProxy(uint64 gid);
		void RecycleBin(ServerProxy* is);
		IServer* GetServerReference(uint index);
	public:
		HADES_FORCEINLINE uint GetMaxBufferSize();
		HADES_FORCEINLINE void SetMaxBUfferSize(uint size);
	};

	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint IServerSystem::GetMaxBufferSize()
	{
		return m_MaxBufferSize;
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServerSystem::SetMaxBUfferSize(uint size)
	{
		m_MaxBufferSize = size > MIN_BUFFER_SIZE ? size : MIN_BUFFER_SIZE;
	}
}