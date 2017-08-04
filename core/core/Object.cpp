#include "memory/Allocator.h"
#include "core/Object.hpp"


namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	Rtti Object::s_Rtti( sizeof(Object), NULL, "Object" );
	//---------------------------------------------------------------------------------------------------------
	intptr_t Object::GetObjectID() const
	{
		return (intptr_t)this;
	}
//#ifndef _TRACK_MEMORY
//	//---------------------------------------------------------------------------------------------------------
//	void* Object::operator new( uint size )
//	{
//		return Allocator::s_pAlloc( size );
//	}
//	//---------------------------------------------------------------------------------------------------------
//	void* Object::operator new[]( uint size )
//	{
//		return Allocator::s_pAlloc( size );
//	}
//	//---------------------------------------------------------------------------------------------------------
//	void Object::operator delete[]( void* ptr )
//	{
//		Allocator::s_pRelease( ptr );
//	}
//	//---------------------------------------------------------------------------------------------------------
//	void Object::operator delete( void* ptr )
//	{
//		Allocator::s_pRelease( ptr );
//	}
//#endif
}