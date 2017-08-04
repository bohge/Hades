#pragma once


namespace hc{class ICore;}
extern hc::ICore* CreateCore();
extern void DestroyCore(hc::ICore* core);
