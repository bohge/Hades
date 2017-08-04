#pragma once
#include "core/Singleton.hpp"
#include "core/Rtti.hpp"


#include <EASTL/map.h>

namespace hc
{
	class IThreadMutex;
	class ClassRegister;
	class IVirtualMachine;
	class IScriptInstance;
}
namespace hlss
{
	class ClassCache
	{
		SINGLETON_DECLARE_ONCREATE_ONDESTROY(ClassCache);
	public:
		typedef eastl::pair< hc::Rtti*, hc::ClassRegister* > InfoPair;
	private:		
		typedef eastl::map< uint, InfoPair* > ClassinfoMap;
	private:
		ClassinfoMap			m_ClassinfoMap;
		hc::IThreadMutex*			m_pIThreadMutex;
	public:
		ClassCache(void);
		~ClassCache(void);
	private:
		void _OnCreate();
		void _OnDestroy();
	private:
		const InfoPair* _NewClassInfo(const eastl::string& file, hc::IScriptInstance* instance);
	public:
		const InfoPair* GetClassCached(const eastl::string& file, hc::IScriptInstance* instance);
	};
}
