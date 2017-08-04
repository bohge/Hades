#include "ThreadSpecific.h"
#include "JobThreadArray.h"
#include "JobDispatcher.h"
#include "JobThreadPool.h"
#include "ThreadSystem.h"
#include "ThreadRWLock.h"
#include "ThreadMutex.h"
#include "ThreadEvent.h"



using namespace hc;

namespace hpts
{
	//---------------------------------------------------------------------------------------------------------
	ThreadSystem::ThreadSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ThreadSystem::~ThreadSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void ThreadSystem::Initialize()
	{
		//JobThreadPool::Create();
		IThreadSystem::Initialize();
	}
	//---------------------------------------------------------------------------------------------------------
	void ThreadSystem::Exit()
	{
		IThreadSystem::Exit();
		//JobThreadPool::Destroy();
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IThreadEntity* ThreadSystem::_DoThreadEntityFactory(hc::ThreadProperty::EntityType et) const
	{
		switch (et)
		{
		case ThreadProperty::ET_EVENT: return NEW ThreadEvent;
		case ThreadProperty::ET_MUTEX: return NEW ThreadMutex;
		case ThreadProperty::ET_RWLOCK: return NEW ThreadRWLock;
		case ThreadProperty::ET_JOB_DISPATCHER: return NEW JobDispatcher;
		case ThreadProperty::ET_THREAD_SPECIFIC: return NEW ThreadSpecific;
		case ThreadProperty::ET_JOB_THREAD_ARRAY: return NEW JobThreadArray;
		default: ASSERT(false); return NULL;
		}
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void ThreadSystem::_DoRecycleBin(hc::IThreadEntity* te) const
	{
		SAFE_DELETE(te);
	}
}
