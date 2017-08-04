#pragma once
#include "core/Singleton.hpp"


#include <EASTL/map.h>
#include <EASTL/string.h>

namespace hc
{
	class IThreadMutex;
}
namespace hlss
{
	class Scriptfilecache
	{
		SINGLETON_DECLARE(Scriptfilecache);
	private:
		typedef eastl::map<uint, eastl::string* > FileMap;
	private:
		FileMap				m_FileMap;
		hc::IThreadMutex*	m_pIThreadMutex;
	public:
		Scriptfilecache();
		~Scriptfilecache();
	public:
		const eastl::string& GetScriptFile(uint hash, const eastl::string& path);
	};
}