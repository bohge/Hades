#pragma once
#include "core/Singleton.hpp"
#include "concurrentqueue.h"
#include "keeper/Accessor.h"


#include <EASTL/vector.h>



namespace hlks
{
	class Accessorpool
	{
		SINGLETON_DECLARE(Accessorpool);
	private:
		typedef moodycamel::ConcurrentQueue< hc::Accessor* > AccessorQueue;
	private:
		AccessorQueue		m_AccessorQueue;
	private:
		Accessorpool();
		~Accessorpool();
	public:
		HADES_FORCEINLINE hc::Accessor* Get();
		HADES_FORCEINLINE void Free(hc::Accessor* acc);
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::Accessor* Accessorpool::Get()
	{
		hc::Accessor* res;
		if (!m_AccessorQueue.try_dequeue(res))
		{
			res = NEW hc::Accessor();
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Accessorpool::Free(hc::Accessor* acc)
	{
		ASSERT(NULL != acc);
		m_AccessorQueue.enqueue(acc);
	}
}