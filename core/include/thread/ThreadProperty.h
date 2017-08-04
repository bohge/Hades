#pragma once
#include "core/Object.hpp"

namespace hc
{
	namespace ThreadProperty
	{
		enum EntityType
		{
			ET_MUTEX = 0,
			ET_RWLOCK,
			ET_EVENT,
			ET_JOB_DISPATCHER,
			ET_THREAD_SPECIFIC,
			ET_JOB_THREAD_ARRAY,
		};
		enum SortType
		{
			ST_UNSORT = 0,
			ST_LESS,
			ST_GREATER,
		};
		enum ThreadPriority
		{
			TP_LOW = 0,
			TP_NORMAL,
			TP_HIGH,
		};
	};
}