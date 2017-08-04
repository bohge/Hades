#include "script/warp/socket/ISocketSystemWarp.hpp"
#include "network/socket/ISocketSystem.h"
#include "network/socket/SocketProxy.h"
#include "serialize/FactorConfigure.h"
#include "network/RequestHandle.h"



using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(SocketProxy, FACTOR_SOCKET);
	//---------------------------------------------------------------------------------------------------------
	SocketProxy::SocketProxy()
		:m_GIndex(nullid)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	SocketProxy::~SocketProxy()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void SocketProxy::SetIndex(uint64 gid)
	{
		m_GIndex = gid;
		m_spSocket = ISocketSystem::Instance()->GetSocketReference(gid);
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* SocketProxy::GetBuffer() const
	{
		return (byte*)&m_GIndex;
	}
	//---------------------------------------------------------------------------------------------------------
	uint SocketProxy::GetLength() const
	{
		return sizeof(m_GIndex);
	}
	//---------------------------------------------------------------------------------------------------------
	bool SocketProxy::FromBuffer(const byte* buff, uint rawlen)
	{
		SetIndex(*((uint64*)buff));
		return m_spSocket.isValid();
	}
	//---------------------------------------------------------------------------------------------------------
	void SocketProxy::OnRecycle()
	{
		m_spSocket = SmartPtr<ISocket>(NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	ScriptValue::ClearFunction SocketProxy::GetClearFunction()
	{
		return &ISocketCleaner;
	}
}