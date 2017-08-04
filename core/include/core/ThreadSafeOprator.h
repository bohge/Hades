#pragma once
#include "Predefine.h"


namespace hc
{
	namespace ThreadSafeOprator
	{
		//static HADES_FORCEINLINE int Increment( volatile int& var );//返回新值
		//static HADES_FORCEINLINE int Decrement( volatile int& var );//返回新值
		//static HADES_FORCEINLINE int Add( volatile int& var, int add );//返回旧值
		//static HADES_FORCEINLINE int Swap( volatile int* dest, int value );//返回旧值
#define THREADSAFEOPRATOR
#ifdef _PLATFORM_WINDOWS
#include "WindowsThreadSafeOprator.h"
#else
#include "LinuxThreadSafeOprator.h"
#endif
#undef THREADSAFEOPRATOR
	}
}