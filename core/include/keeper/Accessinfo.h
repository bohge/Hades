#pragma once
#include "keeper/IKeeperSystem.h"
#include "keeper/Accessor.h"


#include <EASTL/map.h>


namespace hc
{
	class Accessor;
	struct Accessinfo
	{
		uint64						GID;
		int							Timestamp;
		Accessor::AccessMode		eAccessMode;
		Accessor*					pAccessor;
		Accessinfo(uint64 gid, int timestamp, Accessor::AccessMode mode);
	};
	typedef eastl::map< uint64, Accessinfo > AccessinfoArray;
	typedef Unidelegate1<void, const AccessinfoArray&> AccessorRequestCallback;
}