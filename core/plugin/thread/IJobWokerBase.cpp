#include "thread/IJobWokerBase.h"

namespace hc
{
	//------------------------------------------------------------------------------------------------------
	IJobWokerBase::IJobWokerBase(ThreadProperty::EntityType et)
		:IThreadEntity(et)
		, m_FactoryCallBack(NULL)
		, m_RecycleCallBack(NULL)
		, m_FixedIndex(0)
		, m_ThreadCount(1)
		, m_SortType(ThreadProperty::ST_UNSORT)
	{

	}
	//------------------------------------------------------------------------------------------------------
	IJobWokerBase::~IJobWokerBase(void)
	{
		SAFE_DELETE(m_FactoryCallBack);
		SAFE_DELETE(m_RecycleCallBack);
	}
	//------------------------------------------------------------------------------------------------------
	IJobWokerBase::FactoryEvent::Callback IJobWokerBase::_GetFactoryCallBack()
	{
		FactoryEvent::Callback res = NULL;
		if (NULL != m_FactoryCallBack)
		{
			res = m_FactoryCallBack->Duplicate();
		}
		return res;
	}
	//------------------------------------------------------------------------------------------------------
	IJobWokerBase::RecycleEvent::Callback IJobWokerBase::_GetRecycleCallBack()
	{
		RecycleEvent::Callback res = NULL;
		if (NULL != m_RecycleCallBack)
		{
			res = m_RecycleCallBack->Duplicate();
		}
		return res;
	}
	//------------------------------------------------------------------------------------------------------
	byte IJobWokerBase::GetFixedIndex()
	{
		byte temp = m_FixedIndex;
		++m_FixedIndex;
		return temp % m_ThreadCount;
	}
}
