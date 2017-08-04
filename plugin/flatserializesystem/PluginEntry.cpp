#include "FlatSerializerSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hfbs
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "flat serializer system loaded!\r\n" );
		return NEW FlatSerializerSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "flat serializer system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}