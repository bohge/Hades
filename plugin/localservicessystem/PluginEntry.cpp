#include "LocalServicesSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hlss
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "local services system loaded!\r\n" );
		return NEW LocalServicesSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "local services system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}