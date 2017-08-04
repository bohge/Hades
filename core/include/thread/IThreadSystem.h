#pragma once
#include "core/ThreadSafeOprator.h"
#include "core/Singleton.hpp"
#include "core/SmartPtr.hpp"
#include "ThreadProperty.h"
#include "core/IPlugin.h"
#include "thread/IJob.h"



#include <EASTL/map.h>

namespace hc
{
	class IThreadMutex;
	class IThreadEntity;
	class IJobDispatcher;
	class HADES_CORE_API IThreadSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IThreadSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IThreadSystem, IPlugin);
	private:
		typedef eastl::map< intptr_t, IJobDispatcher* > IJobDispatcherMap;
		typedef eastl::map<intptr_t, SmartPtr<IJob> > PendingMap;
	private:
		DEBUGCODE(PendingMap	m_PendingMap);
		//IJobDispatcherMap		m_IJobDispatcherMap;
		IThreadMutex*			m_pIThreadMutex;
	public:
		IThreadSystem();
		virtual ~IThreadSystem();
	private:
		virtual IThreadEntity* _DoThreadEntityFactory(ThreadProperty::EntityType et) const = 0;
		virtual void _DoRecycleBin(IThreadEntity* te) const = 0;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		IThreadEntity* ThreadEntityFactory(ThreadProperty::EntityType et);
		void RecycleBin(IThreadEntity* te);
	public:
		DEBUGCODE(void InsertPendingMap(SmartPtr<IJob> job));
		DEBUGCODE(void RemovePendingMap(SmartPtr<IJob> job));
	};
}