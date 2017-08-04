#pragma once
#include "core/Delegate.hpp"
#include "core/SmartPtr.hpp"
#include "IJobWokerBase.h"
#include "thread/IJob.h"


namespace hc
{
	class HADES_CORE_API IJobDispatcher : public IJobWokerBase
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IJobDispatcher, IJobWokerBase)
	public:
		IJobDispatcher(void) :IJobWokerBase(ThreadProperty::ET_JOB_DISPATCHER){}
		virtual ~IJobDispatcher(void){}
	public:
		virtual void PushJob(SmartPtr<IJob> job) = 0;
	};
}