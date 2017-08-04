#include "core/Object.hpp"


#include <EASTL/list.h>

namespace hc
{
	class IPlugin;
	class ISystemPlugin;
	class IPluginLoader;
	class SystemManager;
	class ICore : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( ICore );
	private:
		typedef eastl::list< IPlugin* > LoadedPlugin;
		typedef eastl::list< eastl::pair<const Rtti*, bool> >	SystemPlugin;
	private:
		LoadedPlugin		m_LoadedPlugin;
		SystemPlugin		m_SystemPlugin;
		IPluginLoader*		m_pLoader;
	public:
		ICore(void);
		~ICore(void);
	private:
		virtual IPluginLoader* _CreateLoader() = 0;
		virtual void _ReleaseLoader( IPluginLoader* loder ) = 0;
	private:
		void _PushPlugin( IPlugin* plugin );						// 加载插件
		bool _ParseConfig( const eastl::string& root, const eastl::string& confname );		// 解析配置表Config.json
		void _ReleasePlugin();
		bool _VerifyPlugin();
	public:
		bool Initialize( const eastl::string& root, const eastl::string& confname );
		void Exit();
		void Dowork();
		IPlugin* GetPlugin( const Rtti& rtti );
	private:
		template<typename T> HADES_FORCEINLINE void _AddSystemPlugin()
		{
			m_SystemPlugin.push_back(eastl::make_pair(&T::RTTI(), false));
		}
	};
}