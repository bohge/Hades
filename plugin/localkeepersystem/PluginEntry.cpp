#include "LocalKeeperSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hlks
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "local keeper system loaded!\r\n" );
		return NEW LocalKeeperSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem( hc::IPlugin* plugin )
	{
		DEBUGLOG( "local keeper system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}