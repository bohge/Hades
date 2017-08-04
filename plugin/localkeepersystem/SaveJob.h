#pragma once
#include "keeper/IKeeperSystem.h"
#include "ObjectHolder.h"
#include "thread/IJob.h"



namespace hc
{
	class Accessor;
}
namespace hlks
{
	class SaveJob : public hc::IJob
	{
	private:
		hc::Accessor*				m_rpSource;
		hc::Accessor*				m_rpDestination;
		SaveCallback::Callback		m_pUserCallback;
		RecycleCallback::Callback	m_pSaveJobCallback;
	public:
		SaveJob();
		virtual ~SaveJob();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		HADES_FORCEINLINE void SetCallback(SaveCallback::Callback func);//这里传递进来的callback不会自己析构
		HADES_FORCEINLINE void SetAccessors(hc::Accessor* des, hc::Accessor* src);
		HADES_FORCEINLINE void SetJobRecycle(RecycleCallback::Callback func);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SaveJob::SetJobRecycle(RecycleCallback::Callback func)
	{
		m_pSaveJobCallback = func;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SaveJob::SetAccessors(hc::Accessor* des, hc::Accessor* src)
	{
		m_rpSource = src;
		m_rpDestination = des;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SaveJob::SetCallback(SaveCallback::Callback func)
	{
		m_pUserCallback = func;
	}

}
