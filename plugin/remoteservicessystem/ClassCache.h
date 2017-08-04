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
namespace hrss
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
		hc::IVirtualMachine*	m_pIVirtualMachine;
		hc::IThreadMutex*		m_pIThreadMutex;
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
	public:
		HADES_FORCEINLINE const InfoPair* GetClassCached(const eastl::string& file);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const ClassCache::InfoPair* ClassCache::GetClassCached(const eastl::string& file)
	{
		return GetClassCached(file, NULL);
	}
}
