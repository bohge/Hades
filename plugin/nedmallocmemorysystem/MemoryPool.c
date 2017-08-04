#include "nedmalloc.h"


void* nedmalloc_alloc(size_t size)
{
	return nedmalloc(size);
}

void nedmalloc_release(void* ptr)
{
	nedfree(ptr);
}


void* nedmalloc_realloc(void* ptr, size_t size)
{
	return nedrealloc(ptr, size);
}