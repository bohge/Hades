#include "LibeventClientSystem.h"
#include "core/Log.h"


namespace hlec
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "libevent client system loaded!\r\n" );
		return NEW LibeventClientSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem( hc::IPlugin* plugin )
	{
		DEBUGLOG( "libevent client system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}