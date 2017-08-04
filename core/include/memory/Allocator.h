#pragma once
#include "core/Plugindefine.h"
#include <EASTL/vector.h>


namespace hc
{
	class HADES_CORE_API Allocator
	{
	public:
		typedef void* (*alloc_memory)(size_t size);
		typedef void* (*realloc_memory)(void *ptr, size_t sz);
		typedef void (*release_memory)(void* ptr);
	public:
		static alloc_memory	s_pAlloc;
		static realloc_memory s_pRealloc;
		static release_memory s_pRelease;
	public:
		static void Reset();
	};
}