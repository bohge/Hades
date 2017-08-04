#pragma once
#include "core/Singleton.hpp"
#include "core/IPlugin.h"

#include <EASTL/string.h>


namespace hc
{
	class IConfigure;
	class HADES_CORE_API IConfigSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IConfigSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IConfigSystem, IPlugin);
	private:
		IConfigure*		m_pDefualtConfigure;
	protected:
		IConfigSystem(void);
		virtual ~IConfigSystem(void);
	private:
		virtual IConfigure* _DoConfigureFactory(const eastl::string& path) = 0;
		virtual void _DoRecycleBin(IConfigure* cf) = 0;
	public:
		virtual void Exit();
	public:
		HADES_FORCEINLINE IConfigure* ConfigureFactory(const eastl::string& path);
		HADES_FORCEINLINE void RecycleBin(IConfigure* cf);
		HADES_FORCEINLINE void SetDefualtConfigure(const eastl::string& path);
		HADES_FORCEINLINE IConfigure* GetDefualtConfigure();
	};


	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IConfigure* IConfigSystem::ConfigureFactory(const eastl::string& path)
	{
		return _DoConfigureFactory(path);
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IConfigSystem::RecycleBin(IConfigure* cf)
	{
		ASSERT(cf != m_pDefualtConfigure);
		_DoRecycleBin(cf);
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IConfigSystem::SetDefualtConfigure(const eastl::string& path)
	{
		ASSERT(NULL == m_pDefualtConfigure);
		m_pDefualtConfigure = _DoConfigureFactory(path);
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IConfigure* IConfigSystem::GetDefualtConfigure()
	{
		return m_pDefualtConfigure;
	}
}
