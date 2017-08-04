#include "RemoteServicesSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hrss
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "remote services system loaded!\r\n" );
		return NEW RemoteServicesSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "remote services system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}