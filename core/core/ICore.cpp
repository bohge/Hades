#include "network/message/IMessageSystem.h"
#include "network/server/IServerSystem.h"
#include "network/socket/ISocketSystem.h"
#include "serialize/ISerializeSystem.h"
#include "services/IServicesSystem.h"
#include "configure/IConfigSystem.h"
#include "memory/IMemorySystem.h"
#include "script/IScriptSystem.h"
#include "thread/IThreadSystem.h"
#include "configure/IConfigure.h"
#include "core/Volatileintpool.h"
#include "timer/ITimerSystem.h"
#include "file/IFileSystem.h"
#include "core/IFunction.h"
#include "core/Configure.h"
#include "log/ILogSystem.h"
#include "IPluginLoader.h"
#include "ICore.h"


#include <iostream>
#include <EASTL/list.h>


using namespace eastl;
namespace hc
{

	RTTI_TYPE_IMPLEMENT( ICore );
	//---------------------------------------------------------------------------------------------------------
	ICore::ICore( void )
		:m_pLoader(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ICore::~ICore( void )
	{
	}
	//---------------------------------------------------------------------------------------------------------
	bool ICore::Initialize( const eastl::string& root, const eastl::string& confname )
	{
		_AddSystemPlugin<IFileSystem>();
		_AddSystemPlugin<IConfigSystem>();
		_AddSystemPlugin<ILogSystem>();
		_AddSystemPlugin<IServerSystem>();
		_AddSystemPlugin<IMessageSystem>();
		_AddSystemPlugin<ISocketSystem>();
		_AddSystemPlugin<IScriptSystem>();
		_AddSystemPlugin<ISerializeSystem>();
		_AddSystemPlugin<IServicesSystem>();
		_AddSystemPlugin<IThreadSystem>();
		_AddSystemPlugin<ITimerSystem>();
		Volatileintpool::Create();
		m_pLoader = _CreateLoader();
		m_pLoader->AddLibraryPath(root + Configure::DEPENDENCY_PATH);
		list< IPlugin* > def;
		// 先加载默认插件
		if ( m_pLoader->LoadDefualtPlugin( root, def ) )
		{
			// 向SystemManager管理器中压入默认插件
			for( list< IPlugin* >::iterator it = def.begin();
				it != def.end();
				++ it )
			{
				_PushPlugin( *it );
				(*it)->Initialize();
			}
			// 读取配置表，加载剩余插件
			if (_ParseConfig(root, confname))
			{
				if (_VerifyPlugin())
				{
					FOR_EACH(LoadedPlugin, it, m_LoadedPlugin)
					{
						(*it)->Initialize();
					}
					return true;
				}
			}
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ICore::_VerifyPlugin()
	{
		bool res = true;
		FOR_EACH(LoadedPlugin, loaded, m_LoadedPlugin)
		{
			FOR_EACH(SystemPlugin, need, m_SystemPlugin)
			{
				if (need->first->isTypeOrDriverType((*loaded)->GetRtti()))
				{
					need->second = true;
				}
			}
		}
		FOR_EACH(SystemPlugin, need, m_SystemPlugin)
		{
			if (!need->second)
			{
				res = false;
				printf("system plugin %s is not loaded!\r\n", need->first->GetTypeName());
			}
			need->second = false;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ICore::_ParseConfig(const eastl::string& root, const eastl::string& confname)
	{
		vector<string> libs;
		IConfigure* conf = IConfigSystem::Instance()->ConfigureFactory(confname);
		if (!conf)
		{
			HADESERROR("Fail to parse config! lanuch failed!");
		}
		else
		{
			libs = conf->GetArrayString(Configure::LOAD_LIBRARY_ARRAY);// 得到"Librarys"数组中的每一个字符串（插件名）
			IConfigSystem::Instance()->RecycleBin(conf);
			_ReleasePlugin();
			FOR_EACH(vector<string>, path, libs)
			{
				IPlugin* plu = m_pLoader->LoadPlugin(root + *path);					// 加载"Librarys"数组中的每一个字符串所对应的插件
				if (plu)
				{
					_PushPlugin(plu);
				}
			}
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void ICore::_ReleasePlugin()
	{
		FOR_EACH_REVERSE( LoadedPlugin, plugin, m_LoadedPlugin )
		{
			(*plugin)->Exit();
		}
		FOR_EACH_REVERSE( LoadedPlugin, plugin, m_LoadedPlugin )
		{
			m_pLoader->ReleasePlugin( *plugin );
		}
		m_LoadedPlugin.clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void ICore::Exit()
	{
		_ReleasePlugin();
		_ReleaseLoader(m_pLoader);
		Volatileintpool::Destroy();
	}
	//---------------------------------------------------------------------------------------------------------
	void ICore::_PushPlugin( IPlugin* plugin )
	{
		if (plugin->isTypeOrDriverType(IFunction::RTTI()))
		{
			m_LoadedPlugin.push_back(plugin);
		}
		else
		{
			IPlugin* loaded = GetPlugin(plugin->GetRtti());
			if (NULL == loaded)
			{
				m_LoadedPlugin.push_back(plugin);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ICore::Dowork()
	{
		FOR_EACH_REVERSE(LoadedPlugin, plugin, m_LoadedPlugin)
		{
			(*plugin)->Update();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	IPlugin* ICore::GetPlugin( const Rtti& rtti )
	{
		IPlugin* res = NULL;
		FOR_EACH( LoadedPlugin, it, m_LoadedPlugin )
		{
			if ( (*it)->isTypeOrDriverType( rtti ) )
			{
				res = (*it);
				break;
			}
		}
		return res;
	}

}
