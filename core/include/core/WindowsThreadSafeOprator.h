#ifndef THREADSAFEOPRATOR
#error( "Do not include this file in other files" )
#endif



extern "C" long __cdecl _InterlockedIncrement(long volatile *);
extern "C" long __cdecl _InterlockedDecrement(long volatile *);
extern "C" long __cdecl _InterlockedCompareExchange(long volatile *, long, long);
extern "C" long __cdecl _InterlockedExchange(long volatile *, long);
extern "C" long __cdecl _InterlockedExchangeAdd(long volatile *, long);

//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Increment(volatile int* var)
{
	return _InterlockedIncrement((volatile long*)var);
}
//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Decrement(volatile int* var)
{
	return _InterlockedDecrement((volatile long*)var);
}
//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Add(volatile int* var, int add)
{
	return _InterlockedExchangeAdd((volatile long*)var, add);
}
//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Swap(volatile int* dest, int value)
{
	return _InterlockedExchange((volatile long*)dest, value);
}