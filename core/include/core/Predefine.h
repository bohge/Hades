#pragma once
#ifndef __cplusplus
	#error( "just using in cpp file!" )
#endif
#include "memory/Allocator.h"
#include "VariableType.h"
#include "Plugindefine.h"
#include <new>

#ifndef  ASSERT
	#ifdef _DEBUG
		#include <assert.h>
		#define ASSERT(f) assert(f)
		#define DEBUGCODE(x) x
	#else
		#define ASSERT(f)
		#define DEBUGCODE(x)
	#endif
#endif


#ifdef _PLATFORM_WINDOWS
	#define HADES_FORCEINLINE __forceinline
#else
	#define HADES_FORCEINLINE __attribute__((always_inline)) inline
#endif

#ifndef NULL
	#define NULL 0
#endif

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif


#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = NULL;}

#define FOR_EACH( type, var, in ) for( type::iterator var = (in).begin(); var != (in).end(); ++ (var) )
#define FOR_EACH_REVERSE( type, var, in ) for( type::reverse_iterator var = (in).rbegin(); var != (in).rend(); ++ (var) )
#define FOR_EACH_CONST( type, var, in ) for( type::const_iterator var = (in).begin(); var != (in).end(); ++ (var) )
#define FOR_EACH_CONST_REVERSE( type, var, in ) for( type::const_reverse_iterator var = (in).rbegin(); var != (in).rend(); ++ (var) )

#include "Track.h"

#ifdef _TRACK_MEMORY
#define NEW new(__FILE__, __LINE__)
inline void *
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator new(size_t size, const char *file, int line) throw (std::bad_alloc)
{
	void* ptr = hc::Allocator::s_pAlloc(size);
	hc::MemoryTrack::AddTrack((int)ptr, size, file, line);
	return ptr;
};


inline void *
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator new[](size_t size, const char *file, int line) throw (std::bad_alloc)
{
	void* ptr = hc::Allocator::s_pAlloc(size);
	hc::MemoryTrack::AddTrack((int)ptr, size, file, line);
	return ptr;
};


inline void
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator delete(void *p) throw ()
{
	hc::MemoryTrack::RemoveTrack((int)p);
	hc::Allocator::s_pRelease(p);
};

inline void
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator delete[](void *p) throw ()
{
	hc::MemoryTrack::RemoveTrack((int)p);
	hc::Allocator::s_pRelease(p);
};


#else


#define NEW new
inline void *
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator new(size_t size)
#ifndef _PLATFORM_WINDOWS
throw (std::bad_alloc)
#endif
{
	return hc::Allocator::s_pAlloc(size);
};


inline void *
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator new[](size_t size)
#ifndef _PLATFORM_WINDOWS
throw (std::bad_alloc)
#endif
{
	return hc::Allocator::s_pAlloc(size);
};


inline void
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator delete(void *p)
#ifndef _PLATFORM_WINDOWS
throw ()
#endif
{
	hc::Allocator::s_pRelease(p);
};

inline void
#ifdef _PLATFORM_WINDOWS
__cdecl
#endif
operator delete[](void *p)
#ifndef _PLATFORM_WINDOWS
throw ()
#endif
{
	hc::Allocator::s_pRelease(p);
};
#endif
