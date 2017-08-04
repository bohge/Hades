#pragma once
#include "core/SmartPtr.hpp"
#include "core/Object.hpp"

namespace hc
{
	class IJobResume;
	class IThreadMutex;
	class HADES_CORE_API IJob : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IJob);
	private:
		enum Constant
		{
			JC_MAX_PRIORITY		= 250,
			JC_MAX_FIXED_THREAD = 250,
		};
	public:
		enum JobType
		{
			JT_ASYNCHRONOUS = 0,//异步
			JT_SYNCHRONOUS,//同步
		};
	private:
		bool			isDone;
		SmartPtr<IJob>	m_PendingHolder;
		IThreadMutex*	m_pDojob;
		IJobResume*		m_pIJobResume;
		JobType			m_eAsynJob;
		uint			m_nMixFlag;//优先级(8)，是否固定(4)，固定线程(8)
		bool			m_isPending;
	public:
		IJob(void);
		virtual ~IJob(void);
	private:
		virtual void _DoYieldJob() = 0;
		virtual void _DoJob(SmartPtr<IJob>& self, Object* userdata) = 0;
	protected:
		void _Pending();//只能在Dojob中访问
		void _Resume();//不能再Dojob中访问
	public:
		void YieldJob();
		void DoJob(SmartPtr<IJob>& self, Object* userdata, IJobResume* resume);
	public:
		HADES_FORCEINLINE void SetJobType(JobType jt);
		HADES_FORCEINLINE JobType GetJobType();
		HADES_FORCEINLINE void SetPriority(byte p);
		HADES_FORCEINLINE byte GetPriority() const;
		HADES_FORCEINLINE byte isFixedThread() const;
		HADES_FORCEINLINE void SetThreadIndex(byte index);
		HADES_FORCEINLINE byte GetThreadIndex() const;
		HADES_FORCEINLINE bool isPending() const;
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IJob::SetJobType(IJob::JobType jt)
	{
		m_eAsynJob = jt;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IJob::JobType IJob::GetJobType()
	{
		return m_eAsynJob;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IJob::isPending() const
	{
		return m_isPending;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IJob::SetPriority(byte p)
	{
		m_nMixFlag = m_nMixFlag | p;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE byte IJob::GetPriority() const
	{
		return (m_nMixFlag & 0xFF);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE byte IJob::isFixedThread() const
	{
		return (m_nMixFlag & 0x100) != 0;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IJob::SetThreadIndex(byte index)
	{
		m_nMixFlag = m_nMixFlag | 0x100;
		m_nMixFlag = m_nMixFlag | ((uint)index) << 12;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE byte IJob::GetThreadIndex() const
	{
		return (m_nMixFlag & 0xff000) >> 12;
	}
}
