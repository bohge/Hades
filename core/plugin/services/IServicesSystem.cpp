#include "services/IServicesSystem.h"
#include "services/ScriptServices.h"
#include "script/IVirtualMachine.h"
#include "thread/IJobDispatcher.h"
#include "script/ClassRegister.h"
#include "services/IServices.h"
#include "serialize/IEncoder.h"
#include "script/IArguments.h"
#include "services/IAgent.h"
#include "ThreadData.h"



namespace hc
{
	RTTI_TYPE_IMPLEMENT(IServicesSystem);
	SYSTEM_SINGLETON_IMPLEMENT(IServicesSystem);
	//---------------------------------------------------------------------------------------------------------
	IServicesSystem::IServicesSystem(void)
		: m_pWorkThread(NULL)
		, m_pProtocolQueue(NULL)
		, m_ServiceCacheSize(0)
		, m_pScriptServicesCacheMap(NULL)
		, m_pIThreadRWLock(NULL)
		, m_isOptimization(false)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IServicesSystem::~IServicesSystem(void)
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	Object* IServicesSystem::OnThreadCreate()
	{
		return NEW ThreadData;
	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::OnThreadDestory(Object* obj)
	{
		SAFE_DELETE(obj);
	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::Initialize()
	{
		m_pProtocolQueue = NEW  ProtocolQueue;
		m_pIServicesArray = NEW IServicesArray;
		m_pScriptServicesCacheMap = NEW ScriptServicesCacheMap;
		m_pSessionQueue = NEW SessionQueue;
		for (size_t i = 0; i < SSC_PREPARE_SIZE; i++)
		{
			m_pSessionQueue->enqueue(_DoAllocSession());
		}
		for (size_t i = 0; i < SSC_PREPARE_SIZE; i++)
		{
			m_pProtocolQueue->enqueue(NEW Protocol);
		}
		m_pIServicesArray->Initialize(SSC_SERVICES_SIZE);
		m_pIThreadRWLock =
			static_cast<IThreadRWLock*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
		m_pWorkThread =
			static_cast<IJobDispatcher*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
		m_pWorkThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pWorkThread->SetThreadDataCallback(
			MEMBER_FUNC_PTR(&IServicesSystem::OnThreadCreate),
			MEMBER_FUNC_PTR(&IServicesSystem::OnThreadDestory));
		m_pWorkThread->SetName("Services");
	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::Exit()
	{
		m_pWorkThread->Stop();
		IThreadSystem::Instance()->RecycleBin(m_pIThreadRWLock);
		IThreadSystem::Instance()->RecycleBin(m_pWorkThread);
		{
			Protocol* req = NULL;
			while (m_pProtocolQueue->try_dequeue(req))
			{
				SAFE_DELETE(req);
			}
		}
		{
			FOR_EACH(ScriptServicesCacheMap, it, *m_pScriptServicesCacheMap)
			{
				ScriptServices* ss = NULL;
				while (it->second->try_dequeue(ss))
				{
					SAFE_DELETE(ss);
				}
				SAFE_DELETE(it->second);
			}
		}
		ISession* req = NULL;
		while (m_pSessionQueue->try_dequeue(req))
		{
			_DoFreeSession(req);
		}
		SAFE_DELETE(m_pSessionQueue);
		SAFE_DELETE(m_pScriptServicesCacheMap);
		SAFE_DELETE(m_pIServicesArray);
		SAFE_DELETE(m_pProtocolQueue);
	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::RecycleScriptService(ScriptServices* ss)
	{
		ScriptServicesCache* cache = NULL;
		uint hash = Utility::HashCode(ss->GetServiceName());
		m_pIThreadRWLock->ReadLock();
		ScriptServicesCacheMap::iterator it = m_pScriptServicesCacheMap->find(hash);		
		if (m_pScriptServicesCacheMap->end() != it)
		{
			cache = it->second;
			m_pIThreadRWLock->Unlock();
		}
		else
		{
			m_pIThreadRWLock->Unlock();
			m_pIThreadRWLock->WriteLock();
			ScriptServicesCacheMap::iterator it = m_pScriptServicesCacheMap->find(hash);
			if (m_pScriptServicesCacheMap->end() != it)
			{
				cache = it->second;
			}
			else
			{
				cache = NEW ScriptServicesCache;
				m_pScriptServicesCacheMap->insert(eastl::make_pair(hash, cache));
			}
			m_pIThreadRWLock->Unlock();
		}
		cache->enqueue(ss);
	}
	//---------------------------------------------------------------------------------------------------------
	ScriptServices* IServicesSystem::_GetScriptServices(const eastl::string& file)
	{
		bool finded = true;
		ScriptServices* res = NULL;
		uint hash = Utility::HashCode(file);
		m_pIThreadRWLock->ReadLock();
		ScriptServicesCacheMap::iterator it = m_pScriptServicesCacheMap->find(hash);
		if (m_pScriptServicesCacheMap->end() == it)
		{
			finded = false;
		}
		else
		{
			finded = it->second->try_dequeue(res);
		}
		m_pIThreadRWLock->Unlock();
		if (finded)
		{
			res->Relive();
		}
		else
		{
			res = NEW ScriptServices(m_ServiceCacheSize, m_pWorkThread);
			if (!res->LoadScript(file))
			{
				SAFE_DELETE(res);
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	IAgent* IServicesSystem::_ServiceFactory(ScriptServices*& ss, const eastl::string& file)
	{
		IAgent* agn = NULL;
		if (file.size() >= MAX_FILE_LENGTH)
		{
			HADESERROR("Service's file name is out of range. Max length is %d, now is %d\r\n", MAX_FILE_LENGTH - 1, file.size());
		}
		{
			ScriptServices* pservices = _GetScriptServices(file);
			ss = pservices;
			if (pservices)
			{
				SmartPtr<IServices> service = _OnServiceFactory(file, pservices);
				uint64 id = m_pIServicesArray->Push(service);
				service->SetGID(id);
				service->SetServiceFile(file);
				agn = service->AllocAgent();
			}
		}
		return agn;
	}
	//---------------------------------------------------------------------------------------------------------
	IAgent* IServicesSystem::ServiceFactory(const eastl::string& file)
	{
		ScriptServices* pservices;
		return _ServiceFactory(pservices, file);
	}
	//---------------------------------------------------------------------------------------------------------
	IAgent* IServicesSystem::FixedServiceFactory(const eastl::string& file)
	{
		ScriptServices* pservices;
		IAgent* agn = _ServiceFactory(pservices, file);
		if (pservices)
		{
			pservices->FixedThread();
		}
		return agn;
	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::RecycleBin(IAgent* agent)
	{
		uint64 gid = agent->GetGID();
		if (!m_pIServicesArray->Have(gid))
		{
			HADESERROR("Free unkown agent gid %llu\r\n", gid);
		}
		else
		{
			SmartPtr<IServices> service = m_pIServicesArray->Get(gid);
			if (service.isValid())
			{
				service->FreeAgent(agent);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::StartServices(byte threadcount)
	{
		_OnServicesStart();
		m_pWorkThread->SetThreadCount(threadcount);
		m_pWorkThread->Start();
	}
	//---------------------------------------------------------------------------------------------------------
	bool IServicesSystem::RemoteCallBack(Protocol* ptc)
	{
		uint64 gid = ptc->GetCallerGID();
		if (!m_pIServicesArray->Have(gid))
		{
			FreeProtocol(ptc);
			//如果没有这个gid直接报错
			HADESERROR("callback unkown service gid %llu\r\n", gid);
		}
		else
		{
			SmartPtr<IServices> service = m_pIServicesArray->Get(gid);
			if (service.isValid())
			{
				return service->RemoteCallBack(ptc);
			}
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IServicesSystem::RemoteInvoke(uint64 gid, Protocol* ptc)
	{
		if (!m_pIServicesArray->Have(gid))
		{
			HADESERROR("Try get unkown service gid %llu\r\n", gid);
			if (nullid != ptc->GetCallerGID())
			{
				ptc->ClearBuffer();
				IServicesSystem::Instance()->RemoteCallBack(ptc);
			}
		}
		else
		{
			SmartPtr<IServices> service = m_pIServicesArray->Get(gid);
			if (service.isValid())
			{
				return service->RemoteInvoker(ptc);
			}
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void IServicesSystem::RemoveService(uint64 gid)
	{
		if (!m_pIServicesArray->Have(gid))
		{
			HADESERROR("Remove unkown service gid %llu\r\n", gid);
		}
		else
		{
			SmartPtr<IServices> service = m_pIServicesArray->Get(gid);
			if (service.isValid())
			{
				m_pIServicesArray->Remove(gid);
			}
		}
	}
}