#pragma once
#include "memory/IMemorySystem.h"

namespace hnm
{
	class MemorySystem : public hc::IMemorySystem
	{
	public:
		MemorySystem(void);
		~MemorySystem(void);
	private:
		virtual hc::IMemorySystem::AllocFunc _GetAllocFunction() const;
		virtual hc::IMemorySystem::ReleaseFunc _GetReleaseFunction() const;
		virtual hc::IMemorySystem::ReallocFunc _GetReallocFunction() const;
	};
}

