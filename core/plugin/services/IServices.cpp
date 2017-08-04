#include "serialize/ISerializeSystem.h"
#include "services/IServicesSystem.h"
#include "script/ClassRegister.h"
#include "services/IServices.h"
#include "services/IAgent.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(IServices);
	//---------------------------------------------------------------------------------------------------------
	IServices::IServices(void)
		:m_ServiceGID(nullid)
		, m_AgentCount(0)
		, m_rpAgentRtti(NULL)
		, m_rpClassRegister(NULL)
		, m_isPasueAutoFree(false)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IServices::~IServices(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IAgent* IServices::AllocAgent()
	{
		ThreadSafeOprator::Increment(&m_AgentCount);
		IAgent* ag = static_cast<IAgent*>(ISerializeSystem::Instance()
			->GetSerializable(IAgent::RTTI().GetFactor()));
		ag->SetAgentRtti(m_rpAgentRtti);
		_OnAllocAgent(ag);
		return ag;
	}
	//---------------------------------------------------------------------------------------------------------
	void IServices::FreeAgent(IAgent* agent)
	{
		_OnFreeAgent(agent);
		DecrementAgent();
		ISerializeSystem::Instance()->FreeSerializable(agent);
	}
	//---------------------------------------------------------------------------------------------------------
	void IServices::IncrementAgent()
	{
		m_isPasueAutoFree = false;
		ThreadSafeOprator::Increment(&m_AgentCount);
	}
	//---------------------------------------------------------------------------------------------------------
	void IServices::DecrementAgent()
	{
		if (0 == ThreadSafeOprator::Decrement(&m_AgentCount)
			&& !m_isPasueAutoFree)
		{
			_OnServicesFree();
			IServicesSystem::Instance()->RemoveService(m_ServiceGID);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	uint IServices::GetFunctionIndex(const eastl::string& funcname)
	{
		uint res = nullhandle;
		uint funcCount = m_rpClassRegister->GetFunctionListCount();
		for (int i = 0; i < funcCount; ++i)
		{
			const eastl::string& name = m_rpClassRegister->GetFunctionListName(i);
			if (name == funcname)
			{
				res = i;
				break;
			}
		}
		return res;
	}
}