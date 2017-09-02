#include "ThulacFunction.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace htf
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "thulac function loaded!\r\n" );
		return NEW ThulacFunction;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "thulac function release!\r\n" );
		SAFE_DELETE( plugin );
	}
}