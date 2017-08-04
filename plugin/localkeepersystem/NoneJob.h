#pragma once
#include "LocalKeeperSystem.h"
#include "keeper/Accessor.h"
#include "thread/IJob.h"



namespace hc
{
	class Accessor;
}
namespace hlks
{
	class NoneJob : public hc::IJob
	{
	private:
		AccessCallback::Callback	m_pUserCallback;
	public:
		NoneJob();
		virtual ~NoneJob();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		HADES_FORCEINLINE void SetCallback(AccessCallback::Callback func);
	};




	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void NoneJob::SetCallback(AccessCallback::Callback func)
	{
		m_pUserCallback = func;
	}

}
