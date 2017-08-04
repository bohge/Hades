#include "keeper/Accessinfo.h"


#include <EASTL/sort.h>

using namespace eastl;
namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	Accessinfo::Accessinfo(uint64 id, int timestamp, Accessor::AccessMode mode)
		: GID(id)
		, pAccessor(NULL)
		, Timestamp(timestamp)
		, eAccessMode(mode)
	{

	}
}