#include "LinuxFileSystem.h"
#include "core/Predefine.h"
#include "core/Log.h"


namespace hfl
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG("file system loaded!\r\n");
		return NEW LinuxFileSystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG("file system release!\r\n");
		SAFE_DELETE(plugin);
	}
}