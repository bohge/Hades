#include "core/Rtti.hpp"



namespace hc
{
	uint Rtti::MaxFactor(0);
	//-------------------------------------------------------------------------------------------------------
	Rtti::Rtti( uint size, const Rtti* base, const char* name )
		:m_TypeID(Utility::HashCode(name))
		, m_rpBase(base)
		, m_TypeSize(size)
		, m_Factor(nullhandle)
		, m_pisTypeOrDriverTypeFunc( NULL == base ? &Rtti::_NonDrivedClass : &Rtti::_DrivedClass )
	{
		_CloneName(name);
	}
	//-------------------------------------------------------------------------------------------------------
	Rtti::Rtti(uint size, const Rtti* base, const char* name, uint factor)
		:m_TypeID(Utility::HashCode(name))
		, m_rpBase(base)
		, m_TypeSize(size)
		, m_Factor(factor)
		, m_pisTypeOrDriverTypeFunc(NULL == base ? &Rtti::_NonDrivedClass : &Rtti::_DrivedClass)
	{
		_CloneName(name);
		MaxFactor = MaxFactor < factor ? factor : MaxFactor;
	}
	//-------------------------------------------------------------------------------------------------------
	Rtti::Rtti( const Rtti& rhs )
		:m_TypeID( rhs.m_TypeID ),
		m_rpBase( rhs.m_rpBase ),
		m_pisTypeOrDriverTypeFunc( rhs.m_pisTypeOrDriverTypeFunc )
	{
		_CloneName(rhs.m_TypeName);
	}
	//-------------------------------------------------------------------------------------------------------
	Rtti::~Rtti( void )
	{
		free(m_TypeName);
	}
	//-------------------------------------------------------------------------------------------------------
	void Rtti::_CloneName(const char* name)
	{
		uint size = strlen(name) + 1;
		m_TypeName = (char*)malloc(size*sizeof(char));
		memset(m_TypeName, 0, size);
		memcpy(m_TypeName, name, size);
	}
	//-------------------------------------------------------------------------------------------------------
	uint Rtti::GetTypeID() const//得到类信息ID
	{
		return m_TypeID;
	}
	//-------------------------------------------------------------------------------------------------------
	const char* Rtti::GetTypeName() const//返回类名
	{
		return m_TypeName;
	}
	//-------------------------------------------------------------------------------------------------------
	uint Rtti::GetTypeSize() const//返回类大小
	{
		return m_TypeSize;
	}
	//-------------------------------------------------------------------------------------------------------
	bool Rtti::isTypeOrDriverType( const Rtti& rtti ) const
	{
		return (this->*m_pisTypeOrDriverTypeFunc)( rtti );
	}
	//-------------------------------------------------------------------------------------------------------
	bool Rtti::_NonDrivedClass( const Rtti& rhs ) const
	{
		return GetTypeID() == rhs.GetTypeID();
	}
	//-------------------------------------------------------------------------------------------------------
	bool Rtti::_DrivedClass( const Rtti& rhs ) const
	{
		return _NonDrivedClass( rhs ) || m_rpBase->isTypeOrDriverType( rhs );
	}
	//-------------------------------------------------------------------------------------------------------
	uint Rtti::GetFactor() const
	{
		uint res = m_Factor;
		if (nullhandle == res)
		{
			ASSERT(NULL != m_rpBase);
			res = m_rpBase->GetFactor();
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	const Rtti* Rtti::GetBase() const
	{
		return m_rpBase;
	}
}