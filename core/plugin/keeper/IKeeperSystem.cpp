#include "keeper/IKeeperSystem.h"



namespace hc
{
	RTTI_TYPE_IMPLEMENT(IKeeperSystem);
	SYSTEM_SINGLETON_IMPLEMENT(IKeeperSystem);
	//---------------------------------------------------------------------------------------------------------
	IKeeperSystem::IKeeperSystem(void)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IKeeperSystem::~IKeeperSystem(void)
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void IKeeperSystem::Initialize()
	{		
	}
	//---------------------------------------------------------------------------------------------------------
	void IKeeperSystem::Exit()
	{
	}
}