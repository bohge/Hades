#pragma once
#include "network/server/IServerSystem.h"
#include "core/Recycleablevector.hpp"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "core/Object.hpp"


namespace hc
{
	class IAgent;
	class IMessage;
	class Protocol;
	class IThreadRWLock;
	struct RequestHandle;
	enum CallbackType
	{
		SCT_SERVER_START = 0,
		SCT_CONNECT,
		SCT_REQUEST,
		SCT_DISCONNECT,
		//SCT_BROADCAST,
	};
	typedef InterceptDelegate4< bool, IServer*, uint64, CallbackType, IMessage* > ServerCallback;
	class HADES_CORE_API IServer : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IServer);
	private:
		enum Constant
		{
			SNAC_CLIENT_CACHE_SIZE = 5000,
		};
	private:
		typedef Recycleablevector< SmartPtr<RequestHandle> > RequestHandleArray;
		typedef eastl::vector< SmartPtr<RequestHandle> > DefualtHandleArray;
	private:
		eastl::string			m_IPAddress;
		DefualtHandleArray		m_DefualtHandleArray;
		RequestHandleArray*		m_pRequestHandleArray;
		uint					m_AgentIndex;
		int						m_Port;
		bool					m_isRunning;
	public:
		IServer();
		virtual ~IServer();
	private:
		virtual bool _OnStart(int connectthread, int port) = 0;
		virtual void _OnStop() = 0;
	public:
		virtual void SetHeartBeat(uint idle, uint inter, uint count) = 0;
		virtual void SetServerCallback(ServerCallback::Callback func) = 0;
		virtual bool SendBuffer(uint64 id, IMessage* req) = 0;//使用小写，大写的会与
		virtual void Disconnect(uint64 id) = 0;
		virtual void Broadcast(uint64 id, IMessage* msg) = 0;
		virtual void SetSpecific(uint64 id, uint64 spec) = 0;
		virtual uint64 GetSpecific(uint64 id) = 0;
		virtual IServerSystem::ServerType GetServerType() = 0;
	public:
		bool OnReciveMessage(IServer* ser, uint64 id, CallbackType ct, IMessage* message);
	private:
		SmartPtr<RequestHandle> _GetDefaultAgent();
		void _RemoteInvoke(Protocol* ptc, uint64 id, uint index);
		Protocol* _GenerateProtocol(uint64 id, IMessage* message);
	protected:
		void _SetServerAddress(const eastl::string& ip);
	public:
		bool Start(int connectthread, int port);
		void Stop();
		void PushDefualtHandler(IAgent* agent);
		void SetClientHandle(uint64 gid, IAgent* agent);
	public:
		HADES_FORCEINLINE bool isRunning();
		HADES_FORCEINLINE int GetPort();
		HADES_FORCEINLINE const eastl::string& GetServerAddress();
	};
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE int IServer::GetPort()
	{
		return m_Port;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& IServer::GetServerAddress()
	{
		return m_IPAddress;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IServer::isRunning()
	{
		return m_isRunning;
	}
}