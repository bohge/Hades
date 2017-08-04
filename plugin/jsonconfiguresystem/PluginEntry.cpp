#include "JsonConfigSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hjc
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "json configure system loaded!\r\n" );
		return NEW JsonConfigSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "json configure system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}