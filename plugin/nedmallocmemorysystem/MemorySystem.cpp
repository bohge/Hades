#include "MemorySystem.h"


extern "C" void* nedmalloc_alloc(size_t size);
extern "C" void nedmalloc_release( void* ptr );
extern "C" void* nedmalloc_realloc(void* ptr, size_t size);


using namespace hc;

namespace hnm
{
	//---------------------------------------------------------------------------------------------------------
	MemorySystem::MemorySystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	MemorySystem::~MemorySystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IMemorySystem::AllocFunc MemorySystem::_GetAllocFunction() const
	{
		return &nedmalloc_alloc;
	}
	//---------------------------------------------------------------------------------------------------------
	IMemorySystem::ReleaseFunc MemorySystem::_GetReleaseFunction() const
	{
		return &nedmalloc_release;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IMemorySystem::ReallocFunc MemorySystem::_GetReallocFunction() const
	{
		return &nedmalloc_realloc;
	}

}
