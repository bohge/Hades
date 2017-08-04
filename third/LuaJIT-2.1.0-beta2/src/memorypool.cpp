#include "memorypool.h"
#include "concurrentqueue.h"

#include <stdio.h>

static moodycamel::ConcurrentQueue<void*>* s_ppool = NULL;
static size_t DEFAULT_GRANULARITY;
//---------------------------------------------------------------------------------------------------------
void pool_init(size_t size)
{
	DEFAULT_GRANULARITY = size;
	s_ppool = new moodycamel::ConcurrentQueue<void*>;
}
//---------------------------------------------------------------------------------------------------------
void* pool_alloc_memory(size_t size)
{
	void* ptr = NULL;
	if (size > DEFAULT_GRANULARITY)
	{
		printf("luajit_alloc_memory fail! need min %d pass %d\r\n", DEFAULT_GRANULARITY, size);
	}
	else
	{
		s_ppool->try_dequeue(ptr);
	}
	printf("pool_alloc_memory %lu, %p\r\n", size, ptr);
	return ptr;
}
//---------------------------------------------------------------------------------------------------------
int pool_free_memory(void *ptr, size_t size)
{
	s_ppool->enqueue(ptr);
	return 0;
}