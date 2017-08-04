#include "application/IApplication.h"
#include "configure/IConfigSystem.h"
#include "core/Bufferpool.h"
#include "CreateCore.h"
#include "ICore.h"

#ifdef _PLATFORM_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT( IApplication );
	//---------------------------------------------------------------------------------------------------------
	IApplication::IApplication( void )
		:m_pICore(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	IApplication::~IApplication( void )
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::Initialize(const eastl::string& root, const eastl::string& args)
	{
		m_pICore = CreateCore();
		if (m_pICore->Initialize(root, _Configurepath()))
		{
			IConfigSystem::Instance()->SetDefualtConfigure(_Configurepath());
			Bufferpool::Create();
			_OnInitialize(args);			
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::Dowork()
	{
		volatile bool isrunning = true;
		while( isrunning )
		{
			m_pICore->Dowork();
			isrunning = _OnDowork();
#ifdef _PLATFORM_WINDOWS
			Sleep(100);
#else
			usleep(100 * 1000);
#endif
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IApplication::Exit()
	{
		_OnExit();
		Bufferpool::Destroy();
		m_pICore->Exit();
		DestroyCore( m_pICore );
	}

}
