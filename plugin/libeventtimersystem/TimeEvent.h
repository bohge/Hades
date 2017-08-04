#pragma once
#include "timer/ITimerSystem.h"

#include <event2/event.h>
#include <event2/event_struct.h>
namespace hlet
{
	struct TimeEvent
	{
		hc::TimerCallback	Callback;
		struct event		Event;
		uint64				Userdata;
	};
}