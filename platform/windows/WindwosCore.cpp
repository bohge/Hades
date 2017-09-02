
#include "WindowsPluginLoader.h"
#include "WindwosCore.h"






using namespace hc;
namespace hcw
{
	//---------------------------------------------------------------------------------------------------------
	WindowsCore::WindowsCore(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	WindowsCore::~WindowsCore(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	IPluginLoader* WindowsCore::_CreateLoader()
	{
		return NEW WindowsPluginLoader;
	}

	//---------------------------------------------------------------------------------------------------------
	void WindowsCore::_ReleaseLoader( IPluginLoader* loder )
	{
		SAFE_DELETE( loder );
	}
	//---------------------------------------------------------------------------------------------------------
	uint WindowsCore::GetPID()
	{
		return GetCurrentProcessId();
	}
}