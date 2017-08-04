#include "core/Volatileintpool.h"


#include <EASTL/string.h>

namespace hc
{
	SINGLETON_IMPLEMENT(Volatileintpool)
	//---------------------------------------------------------------------------------------------------------
	Volatileintpool::Volatileintpool()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Volatileintpool::~Volatileintpool()
	{
		volatile int* res = NULL;
		if (!m_VolatileintQueue.try_dequeue(res))
		{
			SAFE_DELETE(res);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	volatile int* Volatileintpool::GetVolatileint(volatile int init)
	{
		volatile int* res = NULL;
		if (!m_VolatileintQueue.try_dequeue(res))
		{
			res = NEW volatile int;
		}
		*res = init;
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void Volatileintpool::FreeVolatileint(volatile int* obj)
	{
		ASSERT(NULL != obj);
		m_VolatileintQueue.enqueue(obj);
	}
}
