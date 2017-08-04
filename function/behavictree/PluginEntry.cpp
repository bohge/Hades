#include "BehavictreePlugin.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hbtf
{
	//现在只实现了一个本地的行为树，没办法跨机器复杂平衡
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "Local behavictree function loaded!\r\n" );
		return NEW BehavictreePlugin;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "Local behavictree function release!\r\n" );
		SAFE_DELETE( plugin );
	}
}