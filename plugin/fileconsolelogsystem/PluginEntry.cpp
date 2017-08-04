#include "FileconsoleSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hfcl
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG("file console log system loaded!\r\n");
		return NEW FileconsoleSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG("file console log system release!\r\n");
		SAFE_DELETE(plugin);
	}
}