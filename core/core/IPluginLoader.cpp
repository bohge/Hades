#include "core/IPlugin.h"
#include "IPluginLoader.h"



using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT( IPluginLoader );
	//---------------------------------------------------------------------------------------------------------
	IPluginLoader::IPluginLoader(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IPluginLoader::~IPluginLoader(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IPlugin* IPluginLoader::LoadPlugin( const eastl::string& name )
	{
		IPlugin* res = _DoLoadPlugin( name );									// 创建一个新的插件DLL进程，并将DLL的进程ID和插件信息记录在m_LoadedPluginMap中
		if ( res )
		{
			IPluginMap::iterator it = m_IPluginMap.find( res->GetTypeID() );
			if ( it != m_IPluginMap.end() )
			{
				_DoReleasePlugin( res );										// 找到了，删除m_LoadedPluginMap中的刚通过_DoLoadPlugin()函数创建的插件（保留m_LoadedPluginMap之前加载过的同名插件）
				res = it->second;
			}
			else
			{

				m_IPluginMap.insert( make_pair( res->GetTypeID(), res ) );		// 没找到，同时向m_IPluginMap中插入新的插件信息
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void IPluginLoader::ReleasePlugin( IPlugin* plugin )
	{
		//是否重复删除
		m_IPluginMap.erase( plugin->GetTypeID() );
		_DoReleasePlugin( plugin );
	}

}