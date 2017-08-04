#include "network/socket/ISocketSystem.h"
#include "serialize/ISerializeSystem.h"
#include "network/socket/SocketProxy.h"
#include "network/RequestHandle.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(ISocketSystem);
	SYSTEM_SINGLETON_IMPLEMENT(ISocketSystem);
	//---------------------------------------------------------------------------------------------------------
	ISocketSystem::ISocketSystem()
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	ISocketSystem::~ISocketSystem()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//------------------------------------------------------------------------------------------------------
	void ISocketSystem::Initialize()
	{
		m_ISocketArray.Initialize(SS_SOCKET_SIZE);
		ISerializeSystem::Instance()->RegisterSerializable<SocketProxy>();
	}
	//------------------------------------------------------------------------------------------------------
	SocketProxy* ISocketSystem::SocketFactory(SocketType type)
	{
		SmartPtr<ISocket> isocket = _DoSocketFactory(type);
		ISocketPair* pair = m_ISocketPairlQueue.AllocObjcet();
		pair->first = isocket;
		pair->second = 0;
		SocketProxy* obj = static_cast<SocketProxy*>(ISerializeSystem::Instance()
			->GetSerializable(SocketProxy::RTTI().GetFactor()));
		uint64 gid = m_ISocketArray.Push(pair);
		obj->SetIndex(gid);
		return obj;
	}
	//------------------------------------------------------------------------------------------------------
	SocketProxy* ISocketSystem::AllocSocketProxy(uint64 gid)
	{
		SocketProxy* obj = static_cast<SocketProxy*>(ISerializeSystem::Instance()
			->GetSerializable(SocketProxy::RTTI().GetFactor()));
		obj->SetIndex(gid);
		return obj;
	}
	//------------------------------------------------------------------------------------------------------
	void ISocketSystem::RecycleBin(SocketProxy* ct)
	{
		uint64 gid = ct->GetIndex();
		if (m_ISocketArray.Have(gid))
		{
			ISocketPair* pair = m_ISocketArray.Get(gid);
			if (0 == ThreadSafeOprator::Decrement(&(pair->second)))
			{
				pair->first->Disconnect();
				_DoRecycleBin(pair->first);
				m_ISocketArray.Remove(gid);
				m_ISocketPairlQueue.FreeObject(pair);
			}
		}
		else
		{
			HADESERROR("recycle unkonw socket with gid %llu!", gid);
		}
		ISerializeSystem::Instance()->FreeSerializable(ct);
	}
	//------------------------------------------------------------------------------------------------------
	SmartPtr<ISocket> ISocketSystem::GetSocketReference(uint64 gid)
	{
		SmartPtr<ISocket> res;
		if (m_ISocketArray.Have(gid))
		{
			ISocketPair* pair = m_ISocketArray.Get(gid);
			ThreadSafeOprator::Increment(&(pair->second));
			res = pair->first;
		}
		else
		{
			HADESERROR("get unkonw socket with gid %llu!", gid);
		}
		return res;
	}
}