#ifdef _PLATFORM_WINDOWS
#include "MachinestatusFunctionPlugin.h"
#else
#include "MachinestatusFunctionPluginLinux.h"
#endif
#include "core/Predefine.h"
#include "core/Log.h"


namespace hmsfp
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "machinestatus function loaded!\r\n" );
		return NEW MachinestatusFunctionPlugin;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "machinestatus function release!\r\n" );
		SAFE_DELETE( plugin );
	}
}