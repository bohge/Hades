#pragma once
#include "core/Object.hpp"


#define RECAST_MAX_FILE_LENGTH 120
namespace hrnf
{
	struct SerializData
	{
		char	FileName[RECAST_MAX_FILE_LENGTH];
		int		Maxnode;
		uint	Hash;
	};
}