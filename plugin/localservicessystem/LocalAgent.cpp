#include "services/IServicesSystem.h"
#include "services/IServices.h"
#include "LocalAgent.h"

using namespace hc;
namespace hlss
{
	//---------------------------------------------------------------------------------------------------------
	LocalAgent::LocalAgent()
		:m_ServiceGID(nullid)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LocalAgent::~LocalAgent()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	hc::Object* LocalAgent::Duplicate() const
	{
		return NEW LocalAgent;
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* LocalAgent::GetBuffer() const
	{
		return (byte*)&m_ServiceGID;
	}
	//---------------------------------------------------------------------------------------------------------
	uint LocalAgent::GetLength() const
	{
		return sizeof(m_ServiceGID);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalAgent::FromBuffer(const byte* buff, uint len)
	{
		m_ServiceGID = *(uint64*)buff;
		SmartPtr<IServices> service = IServicesSystem::Instance()->GetSerivices(m_ServiceGID);
		if (service.isValid())
		{
			service->IncrementAgent();
			SetAgentRtti(service->GetAgentRtti());
		}
		else
		{
			SetAgentRtti(NULL);
		}
		return service.isValid();
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalAgent::isEqual(const IAgent& lhs)
	{
		const LocalAgent& llhs = static_cast<const LocalAgent&>(lhs);
		return m_ServiceGID == llhs.m_ServiceGID;
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LocalAgent::GetGID() const
	{
		return m_ServiceGID;
	}
}