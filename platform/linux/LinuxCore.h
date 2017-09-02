#pragma once
#include "core/ICore.h"




namespace hcl
{
	class LinuxCore : public hc::ICore
	{
	public:
		LinuxCore(void);
		~LinuxCore(void);
	private:
		virtual hc::IPluginLoader* _CreateLoader();
		virtual void _ReleaseLoader(hc::IPluginLoader* loder);
	public:
		virtual uint GetPID();
	};
}