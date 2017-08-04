#include "memory/Allocator.h"
#include "stdlib.h"


namespace hc
{
	//c++ 静态变量初始化没有先后顺序，所以，请不要使用需要全局静态并且含有NEW的变量
	Allocator::alloc_memory Allocator::s_pAlloc = &malloc;
	Allocator::realloc_memory Allocator::s_pRealloc = &realloc;
	Allocator::release_memory Allocator::s_pRelease = &free;
	//---------------------------------------------------------------------------------------------------------
	void Allocator::Reset()
	{
		Allocator::s_pAlloc = &malloc;
		Allocator::s_pRealloc = &realloc;
		Allocator::s_pRelease = &free;
	}

}