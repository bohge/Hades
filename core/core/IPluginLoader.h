#pragma once
#include "core/Object.hpp"

#include <EASTL/map.h>
#include <EASTL/string.h>


namespace hc
{
	class IPlugin;
	class IPluginLoader : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IPluginLoader);
	private:
		typedef eastl::map< uint, IPlugin* > IPluginMap;
	public:
		typedef IPlugin* (* PluginEntry)();
		typedef PluginEntry EntryFuncPtr;
		typedef void (* PluginExit)(IPlugin*);
		typedef PluginExit ExitFuncPtr;
	private:
		IPluginMap	m_IPluginMap;
	public:
		IPluginLoader(void);
		~IPluginLoader(void);
	private:
		virtual IPlugin* _DoLoadPlugin( const eastl::string& name ) = 0;
		virtual void _DoReleasePlugin( IPlugin* plugin ) = 0;
	public:
		virtual void AddLibraryPath(const eastl::string& path) = 0;
		virtual bool LoadDefualtPlugin( const eastl::string& path, eastl::list< IPlugin* >& list ) = 0;
	public:
		IPlugin* LoadPlugin( const eastl::string& name );
		void ReleasePlugin( IPlugin* plugin );
	};
}
