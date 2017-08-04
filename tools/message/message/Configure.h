#pragma once
#include "Predefine.h"



namespace hc
{
	enum Constant
	{
		AC_BIT_LENGTH = 30,
		AC_VECTOR_SIZE = AC_BIT_LENGTH - 1,//0ц╩сп
		AC_MAX_MEMORY_SIZE = 1 << AC_BIT_LENGTH,
	};
	namespace Configure
	{
		static const uint FOURCC_NETPACKAGE_HEAD('h$3R');
	}
}