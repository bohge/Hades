#pragma once
#include "Configure.h"

#ifdef _PLATFORM_WINDOWS
#include <Windows.h>
#else
#include<stdio.h>
#endif


namespace hc
{
	static uint GetSize(uint size)
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
		++index;
		return ((uint)1) << index;
	}
}