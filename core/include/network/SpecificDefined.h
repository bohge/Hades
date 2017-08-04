#pragma once
#include "core/Predefine.h"


//63为符号位置，不使用
//0-47为recycleablevector使用的标记位置
//可使用的位置为48-62
#define SD_CLIENT_FLAG		(((uint64)1)<<62)//连接进入的客户端
#define SD_SERVICE_FLAG		(((uint64)1)<<61)//连接进入的远程服务


#define GET_GID(a)			(0x0000FFFFFFFFFFFF&a)
#define SET_GID(a, b)		((0xFFFF000000000000&a)|b)
#define GET_FLAG(a)			(0xFFFF000000000000&a)
#define SET_FLAG(a, b)		((0x0000FFFFFFFFFFFF&a)|b)
#define IS_FLAG(a, b)		((a&b) != 0)

#define MC_USER_DEFINED_ID_MAX	50000