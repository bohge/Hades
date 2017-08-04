#include "memory/IMemorySystem.h"
#include "memory/Allocator.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(IMemorySystem);
	//---------------------------------------------------------------------------------------------------------
	void* IMemorySystem::AllocMemory(size_t size)
	{
		return Allocator::s_pAlloc(size);
	}
	//---------------------------------------------------------------------------------------------------------
	void* IMemorySystem::ReallocMemory(void *ptr, size_t sz)
	{
		return Allocator::s_pRealloc(ptr, sz);
	}
	//---------------------------------------------------------------------------------------------------------
	void IMemorySystem::ReleaseMemory(void* ptr)
	{
		Allocator::s_pRelease(ptr);
	}
	//---------------------------------------------------------------------------------------------------------
	void IMemorySystem::Initialize()
	{
		Allocator::s_pAlloc = _GetAllocFunction();
		Allocator::s_pRealloc = _GetReallocFunction();
		Allocator::s_pRelease = _GetReleaseFunction();
	}
	//-------------------------------------------------------------------------------------------------------
	void IMemorySystem::Update()
	{

	}

	//---------------------------------------------------------------------------------------------------------
	void IMemorySystem::Exit()
	{
		Allocator::Reset();
	}
}