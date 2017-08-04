#include "configure/IConfigSystem.h"
#include "configure/IConfigure.h"
#include "log/ILogSystem.h"


using namespace eastl;

namespace hc
{
	RTTI_TYPE_IMPLEMENT( IConfigure );
	RTTI_TYPE_IMPLEMENT( IConfigSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IConfigSystem );
	//-------------------------------------------------------------------------------------------------------
	IConfigSystem::IConfigSystem( void )
		:m_pDefualtConfigure(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//-------------------------------------------------------------------------------------------------------
	IConfigSystem::~IConfigSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//-------------------------------------------------------------------------------------------------------
	void IConfigSystem::Exit()
	{
		if (NULL != m_pDefualtConfigure)
		{
			_DoRecycleBin(m_pDefualtConfigure);
		}
	}
}