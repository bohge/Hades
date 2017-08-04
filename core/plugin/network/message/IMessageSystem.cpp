#include "network/message/IMessageSystem.h"





using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IMessageSystem);
	SYSTEM_SINGLETON_IMPLEMENT(IMessageSystem);
	//---------------------------------------------------------------------------------------------------------
	IMessageSystem::IMessageSystem()
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IMessageSystem::~IMessageSystem()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
}