#pragma once
#include "Predefine.h"
#include "Rtti.hpp"

#include <EASTL/string.h>

namespace hc
{
	class HADES_CORE_API Object//外链类禁止修改
	{
		static Rtti	s_Rtti;
	public:
		Object(){}
		virtual ~Object(){}
	public:
		static const Rtti& RTTI(){ return Object::s_Rtti; }
		static uint SID(){ return Object::s_Rtti.GetTypeID(); }\
	public:
		virtual uint GetTypeID() const = 0;//得到类信息ID
		virtual bool isType( const Rtti& rtti ) const = 0;//判断是否是某个类
		virtual const char* GetTypeName() const = 0;//返回类名
		virtual uint GetTypeSize() const = 0;//返回类大小
		virtual Object* Duplicate() const = 0;//返回一份副本
		virtual bool isTypeOrDriverType( const Rtti& rtti ) const = 0;//是否是相同的类型或者继承类
		virtual const Rtti& GetRtti() const = 0;
	public:
		intptr_t GetObjectID() const;
//#ifndef _TRACK_MEMORY
//	public:
//		void* operator new( uint size );
//		void operator delete( void* ptr );
//		void* operator new[]( uint size );
//		void operator delete[]( void* ptr );
//#endif
	};
}