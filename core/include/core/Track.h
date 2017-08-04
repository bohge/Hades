#pragma once
#include "Plugindefine.h"

#ifdef _DEBUG
//#define _TRACK_MEMORY
#endif

#ifdef WIN32
#ifndef __cplusplus
#error( "Don't include this file in any c file!" )
#endif

#ifdef _TRACK_MEMORY
namespace hc
{
	typedef struct 
	{ 
		int address; 
		int size; 
		char file[512];
		int line; 
	} ALLOC_INFO; 
	class HADES_CORE_API MemoryTrack
	{
	public:
		static void Initialize();
		static void AddTrack(int addr, int asize, const char *fname, int lnum) ;
		static void RemoveTrack(int addr) ;
		static void DumpUnfreed();
	};
}
#endif // _TRACK_MEMORY
#endif
