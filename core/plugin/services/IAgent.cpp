#include "script/warp/services/IServicesSystemWarp.hpp"
#include "serialize/FactorConfigure.h"
#include "services/IServicesSystem.h"
#include "services/IAgent.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(IAgent, FACTOR_AGENT);
	//---------------------------------------------------------------------------------------------------------
	hc::Rtti IAgent::_MakeRtti()
	{
		return hc::Rtti(sizeof(IAgent), &ISerializable::RTTI(), "IAgent");
	}
	//---------------------------------------------------------------------------------------------------------
	hc::Rtti IAgent::_MakeRttiWithFacotr(uint f)
	{
		return hc::Rtti(sizeof(IAgent), &ISerializable::RTTI(), "IAgent", f);
	}
	//---------------------------------------------------------------------------------------------------------
	const hc::Rtti& IAgent::RTTI()
	{
		return IAgent::s_Rtti;
	}
	//---------------------------------------------------------------------------------------------------------
	uint IAgent::SID()
	{
		return IAgent::s_Rtti.GetTypeID();
	}
	//---------------------------------------------------------------------------------------------------------
	IAgent::IAgent(void)
		: m_pAgentRtti(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	IAgent::~IAgent(void)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	uint IAgent::GetTypeID() const
	{
		ASSERT(false);
		return s_Rtti.GetTypeID();
	}
	//---------------------------------------------------------------------------------------------------------
	bool IAgent::isType(const hc::Rtti& rtti) const
	{
		ASSERT(false);
		return s_Rtti.GetTypeID() == rtti.GetTypeID();
	}
	//---------------------------------------------------------------------------------------------------------
	const char* IAgent::GetTypeName() const
	{
		const char* res = NULL;
		if (NULL != m_pAgentRtti)
		{
			res = m_pAgentRtti->GetTypeName();
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	uint IAgent::GetTypeSize() const
	{
		ASSERT(false);
		return s_Rtti.GetTypeSize();
	}
	//---------------------------------------------------------------------------------------------------------
	bool IAgent::isTypeOrDriverType(const hc::Rtti& rtti) const
	{
		return s_Rtti.isTypeOrDriverType(rtti);
	}
	//---------------------------------------------------------------------------------------------------------
	const hc::Rtti& IAgent::GetRtti() const
	{
		ASSERT(false);
		return s_Rtti;
	}
	//---------------------------------------------------------------------------------------------------------
	const hc::Rtti* IAgent::GetBase() const
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	ClassRegister* IAgent::GetClassInfomation()
	{
		ClassRegister* res = NULL;
		SmartPtr<IServices> service = IServicesSystem::Instance()->GetSerivices(GetGID());
		if (service.isValid())
		{
			res = service->GetClassInfomation();
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	ScriptValue::ClearFunction IAgent::GetClearFunction()
	{
		return &ServiceCleaner;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IAgent::RemoteInvoker(Protocol* ptc)
	{
		return IServicesSystem::Instance()->RemoteInvoke(GetGID(), ptc);
	}
	//---------------------------------------------------------------------------------------------------------
	uint IAgent::GetFunctionIndex(const eastl::string& funcname)
	{
		uint res = nullhandle;
		SmartPtr<IServices> service = IServicesSystem::Instance()->GetSerivices(GetGID());
		if (service.isValid())
		{
			res = service->GetFunctionIndex(funcname);
		}
		return res;
	}
}