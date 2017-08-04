#pragma once
#include "core/Predefine.h"


#include <stdio.h>

namespace BufferUtility
{
	static void CompareBuffer(const byte* buf1, uint len1, const byte* buf2, uint len2, const char* msg)
	{
		bool isfail = false;
		if (len1 != len2)
		{
			isfail = true;
			printf("FAIL:%s failed! Buffer length not equal\r\n", msg);
		}
		else
		{
			for (int i = 0; i < len1; ++i)
			{
				if (buf1[i] != buf2[i])
				{
					isfail = true;
					printf("FAIL:%s failed! Buffer data not equl at %d\r\n", msg, i);
				}
			}
		}
		if (!isfail)
		{
			printf("SUCCESS:%s success!\r\n", msg);
		}
	}
}