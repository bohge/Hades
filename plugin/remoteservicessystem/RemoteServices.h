#pragma once
#include "network/socket/SocketProxy.h"
#include "services/IServices.h"



namespace hc{class SocketProxy;}
namespace hrss
{
	class RemoteServices : public hc::IServices
	{
	private:
		hc::SocketProxy*		m_pHostSocket;
		uint64					m_HostGID;//宿主服务GID
		uint					m_HostHash;//宿主地址哈西值
	public:
		RemoteServices();
		~RemoteServices();
	private:
		virtual void _OnServicesFree();
		virtual void _OnAllocAgent(hc::IAgent* agent);
		virtual void _OnFreeAgent(hc::IAgent* agent);
	public:
		virtual void SetGID(uint64 id);
		virtual	bool RemoteInvoker(hc::Protocol* ptc);
		virtual	bool RemoteCallBack(hc::Protocol* ptc);
	public:
		void SetHostSocket(hc::SocketProxy* skt);
	public:
		HADES_FORCEINLINE uint GetHostHash();
		HADES_FORCEINLINE void SetHostHash(uint hash);
		HADES_FORCEINLINE void SetHostGID(uint64 gid);
		HADES_FORCEINLINE uint64 GetHostGID();
	};




	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint RemoteServices::GetHostHash()
	{
		return m_HostHash;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RemoteServices::SetHostHash(uint hash)
	{
		m_HostHash = hash;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RemoteServices::SetHostGID(uint64 gid)
	{
		m_HostGID = gid;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 RemoteServices::GetHostGID()
	{
		return m_HostGID;
	}
}