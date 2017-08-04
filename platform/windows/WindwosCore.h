#pragma once
#include "core/ICore.h"




namespace hcw
{
	class WindowsCore : public hc::ICore
	{
	public:
		WindowsCore(void);
		~WindowsCore(void);
	private:
		virtual hc::IPluginLoader* _CreateLoader();
		virtual void _ReleaseLoader(hc::IPluginLoader* loder);
	};
}