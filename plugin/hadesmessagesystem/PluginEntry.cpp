#include "HadesMessageSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hms
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG("hades message system loaded!\r\n");
		return NEW HadesMessageSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG("hades message system release!\r\n");
		SAFE_DELETE(plugin);
	}
}