#pragma once
#include "Utility.hpp"
#include "Predefine.h"



#include <EASTL/string.h>



namespace hc
{
	class HADES_CORE_API Rtti//外链类禁止修改
	{
	public:
		static uint		MaxFactor;
	private:
		typedef bool (Rtti::*isTypeOrDriverTypeFunc)( const Rtti& ) const;
	private:
		isTypeOrDriverTypeFunc	m_pisTypeOrDriverTypeFunc;
		const Rtti*				m_rpBase;
		char*					m_TypeName;
		uint					m_TypeID;
		uint					m_TypeSize;
		uint					m_Factor;
	public:
		Rtti(uint size, const Rtti* base, const char* name);
		Rtti(uint size, const Rtti* base, const char* name, uint factor);
		Rtti( const Rtti& rhs );
		virtual ~Rtti(void);
	private:
		void _CloneName(const char* name);
		bool _NonDrivedClass( const Rtti& rhs ) const;
		bool _DrivedClass( const Rtti& rhs ) const;
	public:
		uint GetTypeID() const;//得到类信息ID
		const char* GetTypeName() const;//返回类名
		uint GetTypeSize() const;//返回类大小
		bool isTypeOrDriverType( const Rtti& rtti ) const;
		uint GetFactor() const;
		const Rtti* GetBase() const;
	};


	#define RTTI_TYPE_DECLEAR( type )\
	private:\
		static hc::Rtti			s_Rtti;\
	public:\
		static hc::Rtti _MakeRtti(){return hc::Rtti( sizeof(type), NULL, #type );}\
		static hc::Rtti _MakeRttiWithFacotr(uint f){ return hc::Rtti(sizeof(type), NULL, #type, f); }\
		static const hc::Rtti& RTTI(){return type::s_Rtti;}\
		static uint SID(){ return type::s_Rtti.GetTypeID(); }\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual hc::Object* Duplicate() const\
		{\
			type* res = NEW type(*this);\
			return res;\
		}\
		virtual bool isTypeOrDriverType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const hc::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}\
		virtual const hc::Rtti* GetBase() const\
		{\
			return type::s_Rtti.GetBase();\
		}


#define RTTI_ABSTRACT_TYPE_DECLEAR( type )\
	private:\
		static hc::Rtti			s_Rtti;\
	public:\
		static hc::Rtti _MakeRtti(){return hc::Rtti( sizeof(type), NULL, #type );}\
		static hc::Rtti _MakeRttiWithFacotr(uint f){ return hc::Rtti(sizeof(type), NULL, #type, f); }\
		static const hc::Rtti& RTTI(){return type::s_Rtti;}\
		static uint SID(){ return type::s_Rtti.GetTypeID(); }\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual hc::Object* Duplicate() const\
		{\
			ASSERT(false&&"Abstract class can not be duplicate");\
			return NULL;\
		}\
		virtual bool isTypeOrDriverType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const hc::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}\
		virtual const hc::Rtti* GetBase() const\
		{\
			return type::s_Rtti.GetBase(); \
		}



#define RTTI_DRIVER_TYPE_DECLEAR( type, base )\
	private:\
		static hc::Rtti			s_Rtti;\
	public:\
		static hc::Rtti _MakeRtti(){return hc::Rtti( sizeof(type), &base::RTTI(), #type );}\
		static hc::Rtti _MakeRttiWithFacotr(uint f){ return hc::Rtti(sizeof(type), &base::RTTI(), #type, f); }\
		static const hc::Rtti& RTTI(){return type::s_Rtti;}\
		static uint SID(){ return type::s_Rtti.GetTypeID(); }\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual hc::Object* Duplicate() const\
		{\
			type* res = NEW type(*this);\
			return res;\
		}\
		virtual bool isTypeOrDriverType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const hc::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}\
		virtual const hc::Rtti* GetBase() const\
		{\
			return type::s_Rtti.GetBase(); \
		}


#define RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( type, base )\
	private:\
		static hc::Rtti			s_Rtti;\
	public:\
		static hc::Rtti _MakeRtti(){return hc::Rtti( sizeof(type), &base::RTTI(), #type );}\
		static hc::Rtti _MakeRttiWithFacotr(uint f){ return hc::Rtti(sizeof(type), &base::RTTI(), #type, f); }\
		static const hc::Rtti& RTTI(){return type::s_Rtti;}\
		static uint SID(){ return type::s_Rtti.GetTypeID(); }\
	public:\
		virtual uint GetTypeID() const\
		{\
			return type::s_Rtti.GetTypeID();\
		}\
		virtual bool isType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.GetTypeID() == rtti.GetTypeID();\
		}\
		virtual const char* GetTypeName() const\
		{\
			return type::s_Rtti.GetTypeName();\
		}\
		virtual uint GetTypeSize() const\
		{\
			return type::s_Rtti.GetTypeSize();\
		}\
		virtual hc::Object* Duplicate() const\
		{\
			ASSERT(false&&"Abstract class can not be duplicate");\
			return NULL;\
		}\
		virtual bool isTypeOrDriverType( const hc::Rtti& rtti ) const\
		{\
			return type::s_Rtti.isTypeOrDriverType( rtti );\
		}\
		virtual const hc::Rtti& GetRtti() const\
		{\
			return s_Rtti;\
		}\
		virtual const hc::Rtti* GetBase() const\
		{\
			return type::s_Rtti.GetBase(); \
		}

#define RTTI_TYPE_IMPLEMENT( type ) hc::Rtti type::s_Rtti( type::_MakeRtti() );

#define RTTI_TYPE_IMPLEMENT_WITH_FACTOR( type, factor ) hc::Rtti type::s_Rtti( type::_MakeRttiWithFacotr(factor) );
}
