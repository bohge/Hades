#include "MathFunctionPlugin.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hmfp
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "3d math function loaded!\r\n" );
		return NEW MathFunctionPlugin;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "3d math function release!\r\n" );
		SAFE_DELETE( plugin );
	}
}