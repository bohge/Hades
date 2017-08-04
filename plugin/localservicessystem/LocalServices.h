#pragma once
#include "services/IServices.h"


namespace hlss
{
	class LocalServices : public hc::IServices
	{
	private:
		hc::SmartPtr<hc::ScriptServices>	m_spScriptServices;
	public:
		LocalServices();
		~LocalServices();
	private:
		virtual void _OnServicesFree();
		virtual void _OnAllocAgent(hc::IAgent* agent);
		virtual void _OnFreeAgent(hc::IAgent* agent);
	public:
		virtual void SetGID(uint64 id);
		virtual	bool RemoteInvoker(hc::Protocol* ptc);
		virtual	bool RemoteCallBack(hc::Protocol* ptc);
	public:
		HADES_FORCEINLINE void SetScriptServices(hc::SmartPtr<hc::ScriptServices>& sss);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LocalServices::SetScriptServices(hc::SmartPtr<hc::ScriptServices>& sss)
	{
		m_spScriptServices = sss;
	}
}