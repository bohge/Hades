#pragma once
#include "core/Singleton.hpp"

#include "concurrentqueue.h"



namespace hc
{
	class HADES_CORE_API Volatileintpool
	{
		SINGLETON_DECLARE(Volatileintpool);
	private:
		typedef moodycamel::ConcurrentQueue< volatile int* > VolatileintQueue;
	private:
		VolatileintQueue		m_VolatileintQueue;
	public:
		Volatileintpool();
		~Volatileintpool();
	public:
		volatile int* GetVolatileint(volatile int init);
		void FreeVolatileint(volatile int* obj);
	};
}
