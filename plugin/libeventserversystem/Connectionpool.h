#pragma once
#include "core/Singleton.hpp"

#include "concurrentqueue.h"


namespace hles
{
	class LibeventConnection;
	class Connectionpool
	{
		SINGLETON_DECLARE(Connectionpool);
	private:
		typedef moodycamel::ConcurrentQueue< LibeventConnection* > LibeventConnectionQueue;
	private:
		LibeventConnectionQueue		m_LibeventConnectionQueue;
	public:
		Connectionpool();
		~Connectionpool();
	public:
		LibeventConnection* GetLibeventConnection();
		void FreeLibeventConnection(LibeventConnection* obj);
	};
}
