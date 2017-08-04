#pragma once
#include "core/Delegate.hpp"
#include "core/SmartPtr.hpp"
#include "IJobWokerBase.h"
#include "thread/IJob.h"


namespace hc
{
	class HADES_CORE_API IJobThreadArray : public IJobWokerBase
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IJobThreadArray, IJobWokerBase)
	public:
		IJobThreadArray(void) :IJobWokerBase(ThreadProperty::ET_JOB_THREAD_ARRAY){}
		virtual ~IJobThreadArray(void){}
	public:
		virtual void PushJob(byte index, SmartPtr<IJob> job) = 0;
	};
}