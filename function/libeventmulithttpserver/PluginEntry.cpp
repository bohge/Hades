#include "core/Predefine.h"
#include "HttpserverJob.h"
#include "Httpserver.h"
#include "core/Log.h"


namespace hlhsp
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "libevent http server system loaded!\r\n" );
		return NEW Httpserver;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem( hc::IPlugin* plugin )
	{
		DEBUGLOG( "libevent http server system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}