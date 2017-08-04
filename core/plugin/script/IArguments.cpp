#include "serialize/ISerializeSystem.h"
#include "script/IArguments.h"


using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IArguments);
	//-------------------------------------------------------------------------------------------------------
	IArguments::IArguments(void)
	{
		m_pISerializer = ISerializeSystem::Instance()->SerializerFactory();
	}
	//-------------------------------------------------------------------------------------------------------
	IArguments::~IArguments(void)
	{
		ISerializeSystem::Instance()->RecycleBin(m_pISerializer);
	}
}