#include "core/ThreadSafeOprator.h"
#include "LocalServices.h"
#include "LocalAgent.h"



using namespace hc;
namespace hlss
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
		LocalAgent* la = static_cast<LocalAgent*>(agent);
		la->SetGID(GetGID());
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServices::_OnFreeAgent(hc::IAgent* agent)
	{

	}
}