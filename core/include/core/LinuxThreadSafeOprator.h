#ifndef THREADSAFEOPRATOR
#error( "Do not include this file in other files" )
#endif


//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Increment(volatile int* var)
{
	return __sync_add_and_fetch(var, 1);
}
//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Decrement(volatile int* var)
{
	return __sync_sub_and_fetch(var, 1);
}
//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Add(volatile int* var, int add)
{
	return __sync_add_and_fetch(var, add);
}
//------------------------------------------------------------------------------------------------------
static HADES_FORCEINLINE int Swap(volatile int* dest, int value)
{
	return __sync_lock_test_and_set(dest, value);
}