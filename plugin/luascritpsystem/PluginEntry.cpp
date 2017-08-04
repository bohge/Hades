#include "LuaScriptSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hlss
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "lua script system loaded!\r\n" );
		return NEW LuaScriptSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem( hc::IPlugin* plugin )
	{
		DEBUGLOG( "lua script system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}