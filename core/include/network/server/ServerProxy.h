#pragma once
#include "serialize/ISerializable.h"
#include "network/server/IServer.h"


namespace hc
{
	class HADES_CORE_API ServerProxy : public ISerializable
	{
		SERIALIZABL_TYPE_DECLEAR(ServerProxy);
		RTTI_DRIVER_TYPE_DECLEAR(ServerProxy, ISerializable);
	private:
		IServer*	m_rpServer;
		uint		m_Index;
	public:
		ServerProxy();
		virtual ~ServerProxy();
	public:
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint rawlen);
		virtual ScriptValue::ClearFunction GetClearFunction();
	public:
		void SetIndex(uint i);
	public:
		HADES_FORCEINLINE uint GetIndex();
		HADES_FORCEINLINE void Stop();
		HADES_FORCEINLINE int GetPort();
		HADES_FORCEINLINE bool isRunning();
		HADES_FORCEINLINE bool Start(int connectthread, int port);
		HADES_FORCEINLINE void SetHeartBeat(uint idle, uint inter, uint count);
		HADES_FORCEINLINE void PushDefualtHandler(IAgent* agent);
		HADES_FORCEINLINE void SetClientHandle(uint64 gid, IAgent* agent);
		HADES_FORCEINLINE const eastl::string& GetServerAddress();
		HADES_FORCEINLINE void SetServerCallback(ServerCallback::Callback func);
		HADES_FORCEINLINE bool SendBuffer(uint64 id, IMessage* req);
		HADES_FORCEINLINE void Disconnect(uint64 id);
		HADES_FORCEINLINE void Broadcast(uint64 id, IMessage* msg);
		HADES_FORCEINLINE void SetSpecific(uint64 id, uint64 spec);
		HADES_FORCEINLINE uint64 GetSpecific(uint64 id);
		HADES_FORCEINLINE IServerSystem::ServerType GetServerType();
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::Stop()
	{
		m_rpServer->Stop();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ServerProxy::GetIndex()
	{
		return m_Index;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE int ServerProxy::GetPort()
	{
		return m_rpServer->GetPort();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ServerProxy::Start(int connectthread, int port)
	{
		return m_rpServer->Start(connectthread, port);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::PushDefualtHandler(IAgent* agent)
	{
		m_rpServer->PushDefualtHandler(agent);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::SetHeartBeat(uint idle, uint inter, uint count)
	{
		m_rpServer->SetHeartBeat(idle, inter, count);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::SetClientHandle(uint64 gid, IAgent* agent)
	{
		return m_rpServer->SetClientHandle(gid, agent);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& ServerProxy::GetServerAddress()
	{
		return m_rpServer->GetServerAddress();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::SetServerCallback(ServerCallback::Callback func)
	{
		m_rpServer->SetServerCallback(func);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ServerProxy::SendBuffer(uint64 id, IMessage* req)
	{
		return m_rpServer->SendBuffer(id, req);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::Disconnect(uint64 id)
	{
		m_rpServer->Disconnect(id);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::Broadcast(uint64 id, IMessage* msg)
	{
		return m_rpServer->Broadcast(id, msg);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ServerProxy::SetSpecific(uint64 id, uint64 spec)
	{
		return m_rpServer->SetSpecific(id, spec);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 ServerProxy::GetSpecific(uint64 id)
	{
		return m_rpServer->GetSpecific(id);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IServerSystem::ServerType ServerProxy::GetServerType()
	{
		return m_rpServer->GetServerType();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ServerProxy::isRunning()
	{
		return m_rpServer->isRunning();
	}
}