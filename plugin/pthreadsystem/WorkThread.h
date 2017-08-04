#pragma once
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"
#include "Threading.h"





namespace hpts
{
	class WorkResume;
	class ThreadEvent;
	class IJobContainer;
	//异步线程队列
	class WorkThread : public Threading//这个队列不支持移除工作
	{
	private:
		typedef hc::Unidelegate<hc::Object*> FactoryEvent;
		typedef hc::Unidelegate1<void, hc::Object*> RecycleEvent;
		typedef eastl::list< hc::SmartPtr<hc::IJob> > JobList;
	private:
		FactoryEvent				m_UserdataFactory;
		RecycleEvent				m_UserdataRecycle;
		JobList						m_JobList;
		hc::SmartPtr<hc::IJob>		m_Job;
		hc::Object*					m_rpUserdata;
		ThreadEvent*				m_pCondition;
		ThreadEvent*				m_pStopEvent;
		WorkResume*					m_pWorkResume;
		volatile bool				m_isLiveing;
	public:
		WorkThread(void);
		~WorkThread(void);
	private:
		virtual void _OnReset();
		virtual void _OnStart();
		virtual bool _StopWork();
	public:
		virtual void* DoWork();
	public:
		void SetSortType(hc::ThreadProperty::SortType st);
		void SetJob(hc::SmartPtr<hc::IJob>& job);
		void ResumeJob(hc::SmartPtr<hc::IJob>& job);	
	public:
		HADES_FORCEINLINE void SetThreadDataCallback(FactoryEvent::Callback factory, RecycleEvent::Callback recycle);
		HADES_FORCEINLINE hc::Object* GetUserdata();
	};



	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void WorkThread::SetThreadDataCallback(FactoryEvent::Callback factory, RecycleEvent::Callback recycle)
	{
		m_UserdataFactory.Add(factory);
		m_UserdataRecycle.Add(recycle);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::Object* WorkThread::GetUserdata()
	{
		return m_rpUserdata;
	}
}
