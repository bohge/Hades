#include "core/Predefine.h"
#include "core/Log.h"

#include "ThreadSystem.h"


namespace hpts
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "thread system loaded!\r\n" );
		return NEW ThreadSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "thread system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}