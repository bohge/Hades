#include "core/ThreadSafeOprator.h"
#include "RemoteServicesSystem.h"
#include "LocalServices.h"
#include "AdaptiveAgent.h"


using namespace hc;
namespace hrss
{
	//---------------------------------------------------------------------------------------------------------
	LocalServices::LocalServices()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LocalServices::~LocalServices()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServices::SetGID(uint64 id)
	{
		_SetGID(id);
		m_spScriptServices->SetGID(id);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalServices::RemoteInvoker(hc::Protocol* ptc)
	{
		return m_spScriptServices->RemoteInvoker(ptc);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalServices::RemoteCallBack(hc::Protocol* ptc)
	{
		return m_spScriptServices->RemoteCallBack(ptc);
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServices::_OnServicesFree()
	{
		m_spScriptServices->Cleanup();
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServices::_OnAllocAgent(hc::IAgent* agent)
	{
		RemoteServicesSystem* sys = static_cast<RemoteServicesSystem*>(IServicesSystem::Instance());
		AdaptiveAgent* adaagent = static_cast<AdaptiveAgent*>(agent);
		adaagent->SetCurrentGID(GetGID());
		adaagent->SetOriginalGID(GetGID());
		adaagent->SetCurrentHash(sys->GetHash());
		adaagent->SetOriginalHash(sys->GetHash());
		adaagent->SetServiceFile(GetServiceFile());
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServices::_OnFreeAgent(hc::IAgent* agent)
	{

	}
}