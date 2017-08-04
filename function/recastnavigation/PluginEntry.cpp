#include "RecastnavigationPlugin.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hrnf
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "Recastnavigation function loaded!\r\n" );
		return NEW Recastnavigation;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "Recastnavigation function release!\r\n" );
		SAFE_DELETE( plugin );
	}
}