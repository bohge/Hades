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
	class CopyJob : public hc::IJob
	{
	private:
		int							m_Timestamp;
		hc::Accessor*				m_rpSource;
		hc::Accessor::AccessMode	m_eAccessMode;
		AccessCallback::Callback	m_pUserCallback;
		RecycleCallback::Callback	m_pCopyJobCallback;
		bool						m_isCopy;
	public:
		CopyJob();
		virtual ~CopyJob();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		HADES_FORCEINLINE void SetCallback(AccessCallback::Callback func);
		HADES_FORCEINLINE void SetAccessor(hc::Accessor* src);
		HADES_FORCEINLINE void SetMode(hc::Accessor::AccessMode am);
		HADES_FORCEINLINE void SetJobRecycle(RecycleCallback::Callback func);
		HADES_FORCEINLINE void SetCopyFlag(bool f);
		HADES_FORCEINLINE void SetTimestamp(int v);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void CopyJob::SetTimestamp(int v)
	{
		m_Timestamp = v;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void CopyJob::SetCopyFlag(bool f)
	{
		m_isCopy = f;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void CopyJob::SetJobRecycle(RecycleCallback::Callback func)
	{
		m_pCopyJobCallback = func;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void CopyJob::SetMode(hc::Accessor::AccessMode am)
	{
		m_eAccessMode = am;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void CopyJob::SetAccessor(hc::Accessor* src)
	{
		m_rpSource = src;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void CopyJob::SetCallback(AccessCallback::Callback func)
	{
		m_pUserCallback = func;
	}

}
