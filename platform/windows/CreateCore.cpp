#include "WindwosCore.h"
#include "WindowsDump.h"

//---------------------------------------------------------------------------------------------------------
hc::ICore* CreateCore()
{
	InitMinDump();
	return NEW hcw::WindowsCore;
}
//---------------------------------------------------------------------------------------------------------
void DestroyCore(hc::ICore* core)
{
	SAFE_DELETE( core );
}