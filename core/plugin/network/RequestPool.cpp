#include "services/IServicesSystem.h"
#include "network/RequestHandle.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadRWLock.h"
#include "network/RequestPool.h"
#include "services/IAgent.h"




using namespace eastl;
namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	RequestPool::ServiceFunctionIndex::ServiceFunctionIndex(uint c, uint r, uint d)
		:connect(c)
		, request(r)
		, disconnect(d)
	{

	}

	SINGLETON_IMPLEMENT(RequestPool);
	//---------------------------------------------------------------------------------------------------------
	RequestPool::RequestPool()
		:m_pIThreadRWLock(NULL)
	{
		m_pIThreadRWLock = static_cast<IThreadRWLock*>(IThreadSystem::Instance()->
			ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
	}
	//---------------------------------------------------------------------------------------------------------
	RequestPool::~RequestPool()
	{
		IThreadSystem::Instance()->RecycleBin(m_pIThreadRWLock);
		FOR_EACH(RequestHandleMap, it, m_RequestHandleMap)
		{
			SAFE_DELETE(it->second);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	SmartPtr<RequestHandle> RequestPool::CreateRequestHandle(IAgent* agent)
	{
		const char* name = agent->GetTypeName();
		uint hash = Utility::HashCode(name);
		uint connect;
		uint requestfunc;
		uint disconnectfunc;
		m_pIThreadRWLock->ReadLock();
		RequestHandleMap::iterator it = m_RequestHandleMap.find(hash);
		m_pIThreadRWLock->Unlock();
		if (m_RequestHandleMap.end() != it)
		{
			connect = it->second->connect;
			requestfunc = it->second->request;
			disconnectfunc = it->second->disconnect;
		}
		else
		{
			connect = agent->GetFunctionIndex(Configure::CONNECT_FUNCTION);
			requestfunc = agent->GetFunctionIndex(Configure::MESSAGE_FUNCTION);
			disconnectfunc = agent->GetFunctionIndex(Configure::DISCONNECT_FUNCTION);
			m_pIThreadRWLock->WriteLock();
			m_RequestHandleMap.insert(make_pair(hash, NEW ServiceFunctionIndex(connect, requestfunc, disconnectfunc)));
			m_pIThreadRWLock->Unlock();
		}
		SmartPtr<RequestHandle> prh = SmartPtr<RequestHandle>(NULL);
		SmartPtr<IServices> services = IServicesSystem::Instance()->GetSerivices(agent->GetGID());
		if (services.isValid())
		{
			prh = SmartPtr<RequestHandle>(NEW RequestHandle(services, connect, requestfunc, disconnectfunc));
		}
		return prh;
	}
}