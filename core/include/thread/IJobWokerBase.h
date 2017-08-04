#pragma once
#include "core/Delegate.hpp"
#include "core/SmartPtr.hpp"
#include "IThreadEntity.h"
#include "thread/IJob.h"


namespace hc
{
	class HADES_CORE_API IJobWokerBase : public IThreadEntity
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IJobWokerBase, IThreadEntity)
	private:
		typedef Unidelegate<Object*> FactoryEvent;
		typedef Unidelegate1<void, Object*> RecycleEvent;
	private:
		FactoryEvent::Callback		m_FactoryCallBack;
		RecycleEvent::Callback		m_RecycleCallBack;
		ThreadProperty::SortType	m_SortType;
		byte						m_FixedIndex;
		byte						m_ThreadCount;
	public:
		IJobWokerBase(ThreadProperty::EntityType et);
		virtual ~IJobWokerBase(void);
	public:
		virtual void SetName(const eastl::string &name) = 0;
		virtual void Pasue() = 0;
		virtual void Continue() = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;
	protected:
		FactoryEvent::Callback _GetFactoryCallBack();
		RecycleEvent::Callback _GetRecycleCallBack();
	public:
		byte GetFixedIndex();
	public:
		HADES_FORCEINLINE void SetThreadCount( byte c );
		HADES_FORCEINLINE byte GetThreadCount();
		HADES_FORCEINLINE void SetSortType(ThreadProperty::SortType st);
		HADES_FORCEINLINE ThreadProperty::SortType GetSortType();
		HADES_FORCEINLINE void SetThreadDataCallback(FactoryEvent::Callback factory, RecycleEvent::Callback recycle);
	};



	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IJobWokerBase::SetThreadCount(byte c)
	{
		m_ThreadCount = c;
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE byte IJobWokerBase::GetThreadCount()
	{
		return m_ThreadCount;
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IJobWokerBase::SetSortType(ThreadProperty::SortType st)
	{
		m_SortType = st;
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ThreadProperty::SortType IJobWokerBase::GetSortType()
	{
		return m_SortType;
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IJobWokerBase::SetThreadDataCallback(FactoryEvent::Callback factory, RecycleEvent::Callback recycle)
	{
		m_FactoryCallBack = factory;
		m_RecycleCallBack = recycle;
	}

}