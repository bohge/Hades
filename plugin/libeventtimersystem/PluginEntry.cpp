#include "LibeventTimerSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hlet
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "libevent timer system loaded!\r\n" );
		return NEW LibeventTimerSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "libevent timer system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}