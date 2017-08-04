#pragma once
#include "core/Singleton.hpp"
#include "concurrentqueue.h"
#include "core/Configure.h"

#include <EASTL/vector.h>


namespace hc
{
	class HADES_CORE_API Bufferpool
	{
		SINGLETON_DECLARE(Bufferpool);
	private:
		enum Constant
		{
			AC_BIT_LENGTH = 30,
			AC_VECTOR_SIZE = AC_BIT_LENGTH - 1,//0ц╩сп
			AC_MAX_MEMORY_SIZE = 1 << AC_BIT_LENGTH,
		};
	private:
		typedef moodycamel::ConcurrentQueue< byte* > BufferQueue;
		typedef eastl::vector< BufferQueue* > QueueVector;
	private:
		QueueVector		m_QueueVector;
		uint			m_MaxBufferSize;
	public:
		Bufferpool();
		~Bufferpool();
	private:
		uint _GetIndex(uint size);
	public:
		byte* Get(uint size, uint& realsize);
		void Free(uint size, byte* buf);
	public:
		HADES_FORCEINLINE void SetMaxBUfferSize(uint size);
	};


	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Bufferpool::SetMaxBUfferSize(uint size)
	{
		m_MaxBufferSize = size > MIN_BUFFER_SIZE ? size : MIN_BUFFER_SIZE;
	}
}