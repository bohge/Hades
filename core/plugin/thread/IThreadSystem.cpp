#include "thread/IJobThreadArray.h"
#include "thread/IThreadSpecific.h"
#include "thread/IJobDispatcher.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadRWLock.h"
#include "thread/IJobWokerBase.h"
#include "thread/IThreadMutex.h"
#include "thread/IThreadEvent.h"
#include "thread/IJobResume.h"
#include "log/ILogSystem.h"
#include "thread/IJob.h"




using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IJob);
	RTTI_TYPE_IMPLEMENT(IJobResume);
	RTTI_TYPE_IMPLEMENT(IThreadEntity);
	RTTI_TYPE_IMPLEMENT(IThreadSystem);
	RTTI_TYPE_IMPLEMENT(IJobDispatcher);
	RTTI_TYPE_IMPLEMENT(IThreadEvent);
	RTTI_TYPE_IMPLEMENT(IThreadMutex);
	RTTI_TYPE_IMPLEMENT(IThreadRWLock);
	RTTI_TYPE_IMPLEMENT(IJobWokerBase);
	RTTI_TYPE_IMPLEMENT(IThreadSpecific);
	RTTI_TYPE_IMPLEMENT(IJobThreadArray);	
	SYSTEM_SINGLETON_IMPLEMENT(IThreadSystem);
	//---------------------------------------------------------------------------------------------------------
	IThreadSystem::IThreadSystem()
		:m_pIThreadMutex(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IThreadSystem::~IThreadSystem()
	{
		SYSTEM_SINGLETON_DESTROY;
	}	
	//---------------------------------------------------------------------------------------------------------
	void IThreadSystem::Initialize()
	{
		m_pIThreadMutex = static_cast<IThreadMutex*>
			(_DoThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	void IThreadSystem::Update()
	{
		//m_pIThreadMutex->Lock();
		//FOR_EACH(PendingMap, it, m_PendingMap)
		//{
		//	SmartPtr<IJob> job = it->second;
		//	job = it->second;
		//}
		//m_pIThreadMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void IThreadSystem::Exit()
	{
		_DoRecycleBin(m_pIThreadMutex);
	}
	//---------------------------------------------------------------------------------------------------------
	IThreadEntity* IThreadSystem::ThreadEntityFactory(ThreadProperty::EntityType et)
	{
		IThreadEntity* res = _DoThreadEntityFactory(et);
		//if (ThreadProperty::ET_JOB_DISPATCHER == et)
		//{
		//	IJobDispatcher* dis = static_cast<IJobDispatcher*>(res);
		//	m_pIThreadMutex->Lock();
		//	m_IJobDispatcherMap.insert(make_pair(dis->GetObjectID(), dis));
		//	m_pIThreadMutex->Unlock();
		//}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void IThreadSystem::RecycleBin(IThreadEntity* te)
	{
		//if (ThreadProperty::ET_JOB_DISPATCHER == te->GetEntityType())
		//{
		//	m_pIThreadMutex->Lock();
		//	m_IJobDispatcherMap.erase(te->GetObjectID());
		//	m_pIThreadMutex->Unlock();
		//}
		return _DoRecycleBin(te);
	}
	//---------------------------------------------------------------------------------------------------------
	DEBUGCODE(void IThreadSystem::InsertPendingMap(SmartPtr<IJob> job)
	{
		//m_pIThreadMutex->Lock();
		//m_PendingMap.insert(eastl::make_pair(job->GetObjectID(), job));
		//m_pIThreadMutex->Unlock();
	})
	//---------------------------------------------------------------------------------------------------------
	DEBUGCODE(void IThreadSystem::RemovePendingMap(SmartPtr<IJob> job)
	{
		//m_pIThreadMutex->Lock();
		//m_PendingMap.erase(job->GetObjectID());
		//m_pIThreadMutex->Unlock();
	})
}