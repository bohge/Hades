#include "serialize/ISerializeSystem.h"
#include "network/message/IMessage.h"
#include "RemoteServicesSystem.h"
#include "services/Protocol.h"
#include "RemoteServices.h"
#include "AdaptiveAgent.h"
#include "Configure.h"
#include "Utility.h"




using namespace hc;
namespace hrss
{
	//---------------------------------------------------------------------------------------------------------
	RemoteServices::RemoteServices()
		:m_HostHash(nullhandle)
		, m_pHostSocket(NULL)
		, m_HostGID(nullid)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	RemoteServices::~RemoteServices()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServices::SetGID(uint64 id)
	{
		_SetGID(id);
	}
	//---------------------------------------------------------------------------------------------------------
	bool RemoteServices::RemoteInvoker(hc::Protocol* ptc)
	{
		IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		SerializeRemoteCall(this, ptc, msg);
		//HADESLOG("RemoteInvoker func %d", ptc->GetFunction());
		m_pHostSocket->Send(msg);
		IServicesSystem::Instance()->FreeProtocol(ptc);
		ISerializeSystem::Instance()->FreeSerializable(msg);
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	bool RemoteServices::RemoteCallBack(hc::Protocol* ptc)
	{
		HADESERROR("Unexpected remote call back \r\n");
		IServicesSystem::Instance()->FreeProtocol(ptc);
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServices::_OnServicesFree()
	{
		RemoteServicesSystem* sys = static_cast<RemoteServicesSystem*>(IServicesSystem::Instance());
		sys->EraseRemoteServicesFlag(m_HostHash, m_HostGID);
		IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		SerializeFree(m_HostGID, msg);
		m_pHostSocket->Send(msg);
		ISerializeSystem::Instance()->FreeSerializable(msg);
		ISocketSystem::Instance()->RecycleBin(m_pHostSocket);
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServices::_OnAllocAgent(hc::IAgent* agent)
	{
		RemoteServicesSystem* sys = static_cast<RemoteServicesSystem*>(IServicesSystem::Instance());
		AdaptiveAgent* adaagent = static_cast<AdaptiveAgent*>(agent);
		adaagent->SetCurrentGID(GetGID());
		adaagent->SetOriginalGID(m_HostGID);
		adaagent->SetCurrentHash(sys->GetHash());
		adaagent->SetOriginalHash(m_HostHash);
		adaagent->SetServiceFile(GetServiceFile());
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServices::_OnFreeAgent(hc::IAgent* agent)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServices::SetHostSocket(SocketProxy* skt)
	{
		uint64 gid = skt->GetIndex();
		m_pHostSocket = ISocketSystem::Instance()->AllocSocketProxy(gid);
	}
}