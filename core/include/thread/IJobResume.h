#pragma once
#include "core/SmartPtr.hpp"
#include "IThreadSystem.h"
#include "thread/IJob.h"

namespace hc
{
	class HADES_CORE_API IJobResume : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IJobResume)
	public:
		IJobResume(){}
		virtual ~IJobResume(){}
	public:
		virtual void Resumework(SmartPtr<IJob>& job) = 0;
	};


}