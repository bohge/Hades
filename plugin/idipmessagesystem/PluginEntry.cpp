#include "IDIPMessageSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hms
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG("idip message system loaded!\r\n");
		return NEW IDIPMessageSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG("idip message system release!\r\n");
		SAFE_DELETE(plugin);
	}
}