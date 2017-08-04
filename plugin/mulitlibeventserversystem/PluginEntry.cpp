#include "LibeventServerSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hles
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "mulit-io libevent server system loaded!\r\n" );
		return NEW LibeventServerSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem( hc::IPlugin* plugin )
	{
		DEBUGLOG( "mulit-io libevent server system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}