#pragma once
#include "core/SmartPtr.hpp"
#include "thread/IJob.h"


namespace hpts
{
	//这个为基础类,其他容器去继承这个，线程可以动态改变队列类型，但是目前没有需求，暂缓
	class IJobContainer
	{
	public:
		IJobContainer(void){}
		virtual ~IJobContainer(void){}
	public:
		virtual void Clear() = 0;
		virtual bool TryPopJob(hc::SmartPtr<hc::IJob>& job) = 0;
		virtual void PushJob(hc::SmartPtr<hc::IJob>& job) = 0;
		virtual uint Size() = 0;
	};
}
