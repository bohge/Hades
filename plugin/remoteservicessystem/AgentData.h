#pragma once
#include "core/Object.hpp"
#include "Configure.h"


namespace hrss
{
	struct AgentData
	{
		char		ServiceFile[MAX_FILE_LENGTH];
		uint64		CurrentGID;//当前的gid号码
		uint64		OriginalGID;//原始号码，如果是remoteservice这两个号码是不一样的，下同
		uint		CurrentHash;
		uint		OriginalHash;
	};
}