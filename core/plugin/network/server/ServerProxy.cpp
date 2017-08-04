#include "script/warp/server/IServerSystemWarp.hpp"
#include "network/server/IServerSystem.h"
#include "network/server/ServerProxy.h"
#include "serialize/FactorConfigure.h"



using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(ServerProxy, FACTOR_SERVER);
	//---------------------------------------------------------------------------------------------------------
	ServerProxy::ServerProxy()
		:m_rpServer(NULL)
		, m_Index(nullhandle)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ServerProxy::~ServerProxy()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ServerProxy::SetIndex(uint i)
	{
		m_Index = i;
		m_rpServer = IServerSystem::Instance()->GetServerReference(m_Index);
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* ServerProxy::GetBuffer() const
	{
		return (byte*)&m_Index;
	}
	//---------------------------------------------------------------------------------------------------------
	uint ServerProxy::GetLength() const
	{
		return sizeof(m_Index);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ServerProxy::FromBuffer(const byte* buff, uint rawlen)
	{
		SetIndex(*((uint*)buff));
		return NULL != m_rpServer;
	}
	//---------------------------------------------------------------------------------------------------------
	ScriptValue::ClearFunction ServerProxy::GetClearFunction()
	{
		return &IServerCleaner;
	}
}