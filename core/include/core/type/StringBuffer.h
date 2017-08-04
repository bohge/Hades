#pragma once
#include "Type.h"

namespace hc
{
	struct HADES_CORE_API StringBuffer	//脚本与native传递const char*时候使用，保存指针与长度
	{
	public:
		const void*		pBuffer;
		uint			Length;
	};
	
}