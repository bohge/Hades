#pragma once
#include "core/IPlugin.h"


namespace hc
{
	class HADES_CORE_API IMemorySystem : public IPlugin
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IMemorySystem, IPlugin)
	private:
		typedef void* (*alloc_memory)(size_t size);
		typedef void* (*realloc_memory)(void *ptr, size_t sz);
		typedef void (*release_memory)(void* ptr);
	public:
		typedef alloc_memory AllocFunc;
		typedef realloc_memory ReallocFunc;
		typedef release_memory ReleaseFunc;
	public:
		static void* AllocMemory(size_t size);
		static void* ReallocMemory(void *ptr, size_t sz);
		static void ReleaseMemory(void* ptr);
	private:
		virtual AllocFunc _GetAllocFunction() const = 0;
		virtual ReallocFunc _GetReallocFunction() const = 0;
		virtual ReleaseFunc _GetReleaseFunction() const = 0;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	};
}
