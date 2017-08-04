#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "network/server/ServerProxy.h"
#include "services/IServicesSystem.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadRWLock.h"
#include "network/RequestPool.h"
#include "services/IAgent.h"




using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IServerSystem);
	SYSTEM_SINGLETON_IMPLEMENT(IServerSystem);
	//---------------------------------------------------------------------------------------------------------
	IServerSystem::IServerSystem()
		: m_pIServerRWLock(NULL)
		, m_MaxBufferSize(0)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IServerSystem::~IServerSystem()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void IServerSystem::Initialize()
	{
		RequestPool::Create();
		ISerializeSystem::Instance()->RegisterSerializable<ServerProxy>();
		m_pIServerRWLock = static_cast<IThreadRWLock*>(IThreadSystem::Instance()->
			ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
	}
	//---------------------------------------------------------------------------------------------------------
	void IServerSystem::Exit()
	{
		IThreadSystem::Instance()->RecycleBin(m_pIServerRWLock);
		RequestPool::Destroy();
	}
	//------------------------------------------------------------------------------------------------------
	ServerProxy* IServerSystem::ServerFactory(ServerType st)
	{
		IServer* ser = _DoServerFactory(st);
		m_pIServerRWLock->WriteLock();
		uint index = m_IServerArray.size();
		ServerPair* sp = NEW ServerPair;
		sp->first = ser;
		sp->second = 0;
		m_IServerArray.push_back(sp);
		m_pIServerRWLock->Unlock();
		ServerProxy* obj = static_cast<ServerProxy*>(ISerializeSystem::Instance()
			->GetSerializable(ServerProxy::RTTI().GetFactor()));
		obj->SetIndex(index);
		return obj;
	}
	//------------------------------------------------------------------------------------------------------
	ServerProxy* IServerSystem::AllocServerProxy(uint64 gid)
	{
		ServerProxy* obj = static_cast<ServerProxy*>(ISerializeSystem::Instance()
			->GetSerializable(ServerProxy::RTTI().GetFactor()));
		obj->SetIndex(gid);
		return obj;
	}
	//------------------------------------------------------------------------------------------------------
	void IServerSystem::RecycleBin(ServerProxy* is)
	{
		uint index = is->GetIndex();
		m_pIServerRWLock->ReadLock();
		ServerPair* sp = m_IServerArray[index];
		if (0 == ThreadSafeOprator::Decrement(&(sp->second)))
		{
			_DoRecycleBin(sp->first);
			m_IServerArray[index] = NULL;
			SAFE_DELETE(sp);
		}
		m_pIServerRWLock->Unlock();
		ISerializeSystem::Instance()->FreeSerializable(is);
	}
	//------------------------------------------------------------------------------------------------------
	IServer* IServerSystem::GetServerReference(uint index)
	{
		m_pIServerRWLock->ReadLock();
		ServerPair* sp = m_IServerArray[index];
		ThreadSafeOprator::Increment(&(sp->second));
		m_pIServerRWLock->Unlock();
		return sp->first;
	}
}