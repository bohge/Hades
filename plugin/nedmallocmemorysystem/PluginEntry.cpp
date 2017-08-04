#include "core/Predefine.h"
#include "core/Log.h"

#include "MemorySystem.h"

namespace hnm
{
	HADES_EXTERN HADES_PLUGIN hc::IPlugin* CreateSystem()
	{
		DEBUGLOG( "nedmalloc memory system loaded!\r\n" );
		return NEW MemorySystem;
	}
	HADES_EXTERN HADES_PLUGIN void DestroySystem(hc::IPlugin* plugin)
	{
		DEBUGLOG( "nedmalloc memory system release!\r\n" );
		SAFE_DELETE( plugin );
	}
}