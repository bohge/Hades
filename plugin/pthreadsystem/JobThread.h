#pragma once
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"
#include "Threading.h"


#include <EASTL/list.h>


namespace hpts
{
	class JobResume;
	class ThreadEvent;
	class IJobContainer;
	//异步线程队列
	class JobThread : public Threading//这个队列不支持移除工作
	{
	private:
		typedef hc::Unidelegate<hc::Object*> FactoryEvent;
		typedef hc::Unidelegate1<void, hc::Object*> RecycleEvent;
		typedef hc::Unidelegate2<bool, byte, hc::SmartPtr<hc::IJob>& > GetJobHandle;
	private:
		GetJobHandle				m_GetJobHandle;
		FactoryEvent				m_UserdataFactory;
		RecycleEvent				m_UserdataRecycle;
		hc::SmartPtr<hc::IJob>		m_Job;
		hc::Object*					m_rpUserdata;
		ThreadEvent*				m_pCondition;
		ThreadEvent*				m_pStopEvent;
		JobResume*					m_pJobResume;
		volatile bool				m_isLiveing;
		volatile bool				m_isWorking;
		const byte					m_ThreadIndex;
	public:
		JobThread(byte index);
		~JobThread(void);
	private:
		virtual void _OnReset();
		virtual void _OnStart();
		virtual bool _StopWork();
	public:
		virtual void* DoWork();
	public:
		bool Single();
		void ResumeJob(hc::SmartPtr<hc::IJob>& job);
	public:
		HADES_FORCEINLINE hc::Object* GetUserdata();
		HADES_FORCEINLINE void SetGetJobHandle(GetJobHandle::Callback handle);
		HADES_FORCEINLINE void SetThreadDataCallback(FactoryEvent::Callback factory, RecycleEvent::Callback recycle);
	};



	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void JobThread::SetGetJobHandle(GetJobHandle::Callback handle)
	{
		m_GetJobHandle.Add(handle);
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void JobThread::SetThreadDataCallback(FactoryEvent::Callback factory, RecycleEvent::Callback recycle)
	{
		m_UserdataFactory.Add(factory);
		m_UserdataRecycle.Add(recycle);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::Object* JobThread::GetUserdata()
	{
		return m_rpUserdata;
	}
}
