#include "WindowsPluginLoader.h"
#include "core/IPlugin.h"

using namespace eastl;
using namespace hc;
namespace hcw
{
	//---------------------------------------------------------------------------------------------------------
	WindowsPluginLoader::WindowsPluginLoader(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	WindowsPluginLoader::~WindowsPluginLoader(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	IPlugin* WindowsPluginLoader::_DoLoadPlugin(const eastl::string& name)
	{
		IPlugin* res = NULL;
		string fullname;
#ifdef _DEBUG
		fullname = name + "_d.dll";
#else
		fullname = name + ".dll";
#endif // _DEBUG		
		HMODULE hmodule = LoadLibraryA(fullname.c_str());
		ASSERT(NULL != hmodule);
		// 得到插件名name指定的dll中的"Create_Plugin"函数的返回地址，也就是得到插件的IPlugin* Create_Plugin()函数地址。
		IPluginLoader::EntryFuncPtr func = (IPluginLoader::EntryFuncPtr)GetProcAddress(hmodule, "CreateSystem");
		if (func)	// 创建插件成功
		{
			PluginData data;
			data.m_Name = name;
			data.m_pPlugin = func();
			data.m_Handle = hmodule;
			m_LoadedPluginMap.insert(make_pair(data.m_pPlugin->GetObjectID(), data));		// 向m_LoadedPluginMap中插入插件信息
			res = data.m_pPlugin;
		}
		return res;
	}

	//---------------------------------------------------------------------------------------------------------
	void WindowsPluginLoader::_DoReleasePlugin(IPlugin* plugin)
	{
		PluginMap::iterator pluginData = m_LoadedPluginMap.find(plugin->GetObjectID());
		// 得到插件名name指定的dll中的"Destroy_Plugin"函数的返回地址，也就是得到插件的void Destroy_Plugin()函数地址。
		IPluginLoader::ExitFuncPtr func = (IPluginLoader::ExitFuncPtr)GetProcAddress(pluginData->second.m_Handle, ("DestroySystem"));
		func(pluginData->second.m_pPlugin);								// 销毁插件
		FreeLibrary(pluginData->second.m_Handle);
		m_LoadedPluginMap.erase(plugin->GetObjectID());					// 从m_LoadedPluginMap中删除插件信息
	}

	//---------------------------------------------------------------------------------------------------------
	bool WindowsPluginLoader::LoadDefualtPlugin(const eastl::string& path, eastl::list< IPlugin* >& list)
	{
		IPlugin* p1 = LoadPlugin(path + "plugin/windowsfilesystem");
		IPlugin* p2 = LoadPlugin(path + "plugin/fileconsolelogsystem");
		IPlugin* p3 = LoadPlugin(path + "plugin/jsonconfiguresystem");	//加载默认的两个插件		
		if (p1 && p2 && p3)
		{
			list.push_back(p1);
			list.push_back(p2);
			list.push_back(p3);
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void WindowsPluginLoader::AddLibraryPath(const eastl::string& path)
	{
		SetDllDirectoryA(path.c_str());
	}
}