#include "RemoteServicesSystem.h"
#include "RemoteServices.h"
#include "AdaptiveAgent.h"



using namespace hc;
namespace hrss
{
	//---------------------------------------------------------------------------------------------------------
	AdaptiveAgent::AdaptiveAgent()
	{
		m_AgentData.CurrentGID = nullid;
		m_AgentData.OriginalGID = nullid;
		m_AgentData.CurrentHash = nullhandle;
		m_AgentData.OriginalHash = nullhandle;
	}
	//---------------------------------------------------------------------------------------------------------
	AdaptiveAgent::~AdaptiveAgent()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	hc::Object* AdaptiveAgent::Duplicate() const
	{
		return NEW AdaptiveAgent;
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* AdaptiveAgent::GetBuffer() const
	{
		return (byte*)&m_AgentData;
	}
	//---------------------------------------------------------------------------------------------------------
	uint AdaptiveAgent::GetLength() const
	{
		return sizeof(m_AgentData);
	}
	//---------------------------------------------------------------------------------------------------------
	bool AdaptiveAgent::FromBuffer(const byte* buff, uint len)
	{
		memcpy(&m_AgentData, buff, len);
		RemoteServicesSystem* sys = static_cast<RemoteServicesSystem*>(IServicesSystem::Instance());
		SmartPtr<IServices> service;
		if (sys->GetHash() == m_AgentData.CurrentHash)
		{
			service = sys->GetSerivices(m_AgentData.CurrentGID);
		}
		else if (sys->GetHash() == m_AgentData.OriginalHash)
		{
			service = sys->GetSerivices(m_AgentData.OriginalGID);
		}
		else
		{
			service = sys->GetRemoteService(m_AgentData);			
		}
		if (service.isValid())
		{
			service->IncrementAgent();
			SetCurrentGID(service->GetGID());
			SetAgentRtti(service->GetAgentRtti());
		}
		else
		{
			SetCurrentGID(nullid);
			SetAgentRtti(NULL);
		}
		SetCurrentHash(sys->GetHash());
		return service.isValid();
	}
	//---------------------------------------------------------------------------------------------------------
	bool AdaptiveAgent::isEqual(const IAgent& lhs)
	{
		const AdaptiveAgent& llhs = static_cast<const AdaptiveAgent&>(lhs);
		return m_AgentData.CurrentGID == llhs.m_AgentData.CurrentGID
			&& m_AgentData.CurrentHash == m_AgentData.CurrentHash;
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 AdaptiveAgent::GetGID() const
	{
		return m_AgentData.CurrentGID;
	}
}