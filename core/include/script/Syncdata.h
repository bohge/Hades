#pragma once
#include "keeper/Accessor.h"
#include <EASTL/string.h>


namespace hc
{
	struct Syncdata
	{
		uint		uHashCode;
		Accessor*	pAccessor;

		Syncdata()
			:pAccessor(NEW Accessor)
		{
		}
		~Syncdata()
		{
			SAFE_DELETE(pAccessor);
		}
	};
}