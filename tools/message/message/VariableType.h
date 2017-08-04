#pragma once

typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;
typedef unsigned char		byte;
typedef unsigned int		bitmask;
typedef int					sint;
typedef float				clamp;
typedef unsigned int		handle;

#ifdef _WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long	int64;
typedef unsigned long long	uint64;
#endif

namespace hc
{
	static const int	nulltimestamp = -1;
	static const handle nullhandle = -1;
	static const uint64 nullid = -1;
}