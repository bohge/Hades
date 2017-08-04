#include "LinuxPluginLoader.h"
#include "core/IPlugin.h"

#include <dlfcn.h>


using namespace eastl;
using namespace hc;
namespace hcl
{
	//---------------------------------------------------------------------------------------------------------
	LinuxPluginLoader::LinuxPluginLoader(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	LinuxPluginLoader::~LinuxPluginLoader(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	IPlugin* LinuxPluginLoader::_DoLoadPlugin(const eastl::string& name)
	{
		IPlugin* res = NULL;
		string fullname;
		fullname = name + ".so";
		int index = fullname.find_last_of('/');
		fullname.insert(index+1, "lib");
		void* hmodule = dlopen(fullname.c_str(), RTLD_NOW);
		if (NULL == hmodule)
		{
			printf("fail %s\r\n", dlerror());
			fflush(stdout);
		}
		ASSERT(NULL != hmodule);
		// 得到插件名name指定的dll中的"Create_Plugin"函数的返回地址，也就是得到插件的IPlugin* Create_Plugin()函数地址。
		IPluginLoader::EntryFuncPtr func = (IPluginLoader::EntryFuncPtr)dlsym(hmodule, "CreateSystem");
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
	void LinuxPluginLoader::_DoReleasePlugin(IPlugin* plugin)
	{
		PluginMap::iterator pluginData = m_LoadedPluginMap.find(plugin->GetObjectID());
		// 得到插件名name指定的dll中的"Destroy_Plugin"函数的返回地址，也就是得到插件的void Destroy_Plugin()函数地址。
		IPluginLoader::ExitFuncPtr func = (IPluginLoader::ExitFuncPtr)dlsym(pluginData->second.m_Handle, ("DestroySystem"));
		func(pluginData->second.m_pPlugin);								// 销毁插件
		dlclose(pluginData->second.m_Handle);
		m_LoadedPluginMap.erase(plugin->GetObjectID());					// 从m_LoadedPluginMap中删除插件信息
	}

	//---------------------------------------------------------------------------------------------------------
	bool LinuxPluginLoader::LoadDefualtPlugin(const eastl::string& path, eastl::list< IPlugin* >& list)
	{
		IPlugin* p1 = LoadPlugin(path + "plugin/linuxfilesystem");
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
	void LinuxPluginLoader::AddLibraryPath(const eastl::string& path)
	{
	}
}