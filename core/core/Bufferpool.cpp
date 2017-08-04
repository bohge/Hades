#include "core/Bufferpool.h"
#include "core/Configure.h"
#include "log/ILogSystem.h"

#ifdef _PLATFORM_WINDOWS
#include <Windows.h>
#else
#include<stdio.h>
#endif


namespace hc
{
	SINGLETON_IMPLEMENT(Bufferpool);
	//---------------------------------------------------------------------------------------------------------
	Bufferpool::Bufferpool()
		:m_MaxBufferSize(0)
	{
		m_QueueVector.reserve(AC_VECTOR_SIZE);
		for (int i = 0; i < AC_VECTOR_SIZE; ++i)
		{
			m_QueueVector.push_back(NEW BufferQueue);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	Bufferpool::~Bufferpool()
	{
		FOR_EACH(QueueVector, it, m_QueueVector)
		{
			byte* res;
			while ((*it)->try_dequeue(res))
			{
				SAFE_DELETE_ARRAY(res);
			}
			SAFE_DELETE(*it);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	uint Bufferpool::_GetIndex(uint size)
	{
		ASSERT(size <= AC_MAX_MEMORY_SIZE && size > 0);
		//--size;
		//size |= size >> 1;
		//size |= size >> 2;
		//size |= size >> 4;
		//size |= size >> 8;
		//size |= size >> 16;
		//++size;

		//__builtin_clz;

		uint local(size - 1);
		unsigned long index;
#ifdef _PLATFORM_WINDOWS
		BitScanReverse(&index, local);
#else
		index = 31 - __builtin_clz(local);
#endif
		return ++index;
	}
	//---------------------------------------------------------------------------------------------------------
	byte* Bufferpool::Get(uint size, uint& realsize)
	{
		byte* res = NULL;
		if (size > m_MaxBufferSize || size <= 0)
		{
			HADESERROR("Bufferpool try get buffer size(%d) is out of maximun(%d)", size, m_MaxBufferSize);
		}
		else
		{
			uint index = _GetIndex(size) - 1;
			if (index < AC_VECTOR_SIZE && index >= 0)
			{
				realsize = 1 << (index + 1);
				BufferQueue* queue = m_QueueVector[index];
				if (!queue->try_dequeue(res))
				{
					res = NEW byte[realsize];
				}
				memset(res, 0, realsize);
			}
			else
			{
				HADESERROR("Bufferpool get index out of range %d", index);
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void Bufferpool::Free(uint size, byte* buf)
	{
		if (NULL != buf)
		{
			uint index = _GetIndex(size) - 1;
			if (index < AC_VECTOR_SIZE && index >= 0)
			{
				BufferQueue* queue = m_QueueVector[index];
				queue->enqueue(buf);
			}
			else
			{
				SAFE_DELETE(buf);
				HADESERROR("Bufferpool free index out of range %d", index);
			}
		}
	}

}