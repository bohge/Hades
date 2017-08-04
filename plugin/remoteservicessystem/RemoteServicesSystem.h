#pragma once
#include "network/socket/ISocketSystem.h"
#include "network/server/IServerSystem.h"
#include "network/server/ServerProxy.h"
#include "services/IServicesSystem.h"
#include "AgentData.h"

#include <vector>

namespace hc
{
	class SocketProxy;
	class IThreadEvent;
	class IThreadRWLock;
}
namespace hrss
{
	class RemoteServicesSystem : public hc::IServicesSystem
	{
	private:
		struct ClusterStack
		{
			uint64						Gid;
			bool						isReciveEcho;
			bool						isConnected;
			bool						isInvoked;
			hc::IThreadRWLock*			ClusterRWLock;
			hc::SocketProxy*			RemoteSocketProxy;
			eastl::vector<uint64>		SendServicesIndex;
			eastl::vector<uint64>		LocalServicesIndex;
		};
	private:
		typedef eastl::map<uint, ClusterStack*>	ClusterMap;
	private:
		ClusterMap						m_ClusterMap;
		eastl::string					m_Address;
		hc::IThreadEvent*				m_pServerEvent;
		hc::IThreadRWLock*				m_pIThreadRWLock;
		hc::IAgent*						m_pMainAgent;
		hc::ServerProxy*				m_pServerProxy;
		hc::IServerSystem::ServerType	m_ServerType;
		uint							m_IPHash;
		uint							m_FuncRecevieAgent;
		uint							m_FuncRemotenodeReady;
		uint							m_FuncDisconnect;
		bool							m_ScriptInitialized;
	public:
		RemoteServicesSystem();
		virtual ~RemoteServicesSystem();
	private:
		virtual hc::SmartPtr<hc::IServices> _OnServiceFactory(const eastl::string& file, hc::ScriptServices* ss);
		virtual hc::ISession* _DoAllocSession();
		virtual void _DoFreeSession(hc::ISession* ss);
		virtual void _OnServicesStart();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		virtual hc::IResumer* AllocResumer(hc::IVirtualMachine* vm);
	public:
		bool OnReciveMessage(hc::IServer* ser, uint64 id, hc::CallbackType ct, hc::IMessage* message);
		void OnConnected(hc::ISocket* sk);
	private:
		void _TryRemotenodeReadyInvoke(ClusterStack* cs);
		bool _OnDisconnect(uint64 id);
		void _OnRecevieCall(uint64 id, hc::IMessage* msg);
		void _OnRecevieAgent(hc::IMessage* msg);
		void _OnRecevieIncrment(uint64 id, hc::IMessage* msg);
		void _OnRecevieFree(hc::IMessage* msg);
		void _OnRemoteCall(hc::IMessage* msg);
		void _OnRemoteCallBack(hc::IMessage* msg);
		hc::IMessage* _GetCallMessage();
		uint64 _GetCreateRemoteServices(const AgentData& data);
		ClusterStack* _ConnectNode(hc::ISocketSystem::SocketType st, const eastl::string& ip);
	public:
		void ScriptInitialized();
		void SetServerProxy(hc::ServerProxy* ser);
		void SetHandler(hc::IAgent* agent);
		void SendAgent(uint index, hc::IAgent* agent, hc::IMessage* msg);
		void SendRemoteCallBack(hc::Protocol* ptc);
		hc::SmartPtr<hc::IServices> GetRemoteService(const AgentData& data);
		ClusterStack* ConnectNode(hc::ISocketSystem::SocketType st, const eastl::string& ip);
		void EraseRemoteServicesFlag(uint hosthash, uint64 hostgid);
	public:
		HADES_FORCEINLINE uint GetHash();
	};




	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint RemoteServicesSystem::GetHash()
	{
		return m_IPHash;
	}
}