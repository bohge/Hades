#include "serialize/ISerializeSystem.h"
#include "script/ICoroutineInvoker.h"
#include "services/IServicesSystem.h"
#include "services/ScriptServices.h"
#include "script/IVirtualMachine.h"
#include "serialize/ISerializer.h"
#include "thread/IJobDispatcher.h"
#include "script/IScriptSystem.h"
#include "thread/IThreadMutex.h"
#include "services/IAgent.h"
#include "log/ILogSystem.h"



namespace hc
{
	RTTI_TYPE_IMPLEMENT(ScriptServices);
	//---------------------------------------------------------------------------------------------------------
	ScriptServices::ScriptServices(uint size, IJobDispatcher* thread)
		: m_pVirtualMachine(NULL)
		, m_pIScriptInstance(NULL)
		, m_rpIJobDispatcher(thread)
		, m_isExecuting(false)
		, m_isLiving(true)
		, m_pIThreadMutex(NULL)
		, m_spExecuteJob(SmartPtr<ExecuteJob>(NEW ExecuteJob))
		, m_pSessionArray(NEW SessionArray)
		, m_pUnhandleSessiones(NEW UnhandleSessiones)
		, m_CacheSize(size)
		, m_SessionCount(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ScriptServices::~ScriptServices(void)
	{
		m_spExecuteJob = SmartPtr<ExecuteJob>(NULL);
		SAFE_DELETE(m_pSessionArray);
		SAFE_DELETE(m_pUnhandleSessiones);
		IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
		m_pVirtualMachine->ReleaseScript(m_pIScriptInstance);
		IScriptSystem::Instance()->RecycleBin(m_pVirtualMachine);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ScriptServices::LoadScript(const eastl::string& file)
	{
		m_pSessionArray->Initialize(SS_SESSION_SIZE);
		ASSERT(NULL == m_pVirtualMachine);
		ASSERT(NULL == m_pIScriptInstance);
		m_ServiceName = file;
		m_pIThreadMutex = static_cast<IThreadMutex*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_MUTEX));
		m_pVirtualMachine = IScriptSystem::Instance()->VirtualMachineFactory();
		m_pIScriptInstance = m_pVirtualMachine->LoadScript(file);
		if (NULL != m_pIScriptInstance)
		{
			m_pVirtualMachine->SetUserData(&m_MachineUserData);
			m_spExecuteJob->SetInstance(m_pIScriptInstance);
			m_spExecuteJob->SetVirtualMachine(m_pVirtualMachine);
		}
		return NULL != m_pIScriptInstance;
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptServices::SetSelfHandle(const SmartPtr<ScriptServices>& self)
	{
		m_spExecuteJob->SetScriptServices(self);
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptServices::OnExecuteDone(ISession* ss)
	{
		//没有运行完就推出了
		switch (ss->GetState())
		{
		case ISession::SS_DONE:
		case ISession::SS_ERROR:
		{
								  m_pSessionArray->Remove(ss->GetSessionID());
								  IServicesSystem::Instance()->FreeSession(ss);
								  ThreadSafeOprator::Decrement(&m_SessionCount);
								  break;
		}
		case ISession::SS_PASUE: break;
		case ISession::SS_ENTRY:
		default:ASSERT(false); break;
		}
		m_pIThreadMutex->Lock();
		if (!m_isLiving)
		{
			_DoCleanup();
			m_pIThreadMutex->Unlock();
		}
		else
		{
			if (!m_pUnhandleSessiones->empty())
			{
				_ExecuteSessionWithUnlock();
			}
			else
			{
				m_isExecuting = false;
				m_pIThreadMutex->Unlock();
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptServices::_ExecuteSessionWithUnlock()
	{
		ASSERT(!m_pUnhandleSessiones->empty());
		uint64 ssid = m_pUnhandleSessiones->front();
		m_pUnhandleSessiones->pop_front();
		m_pIThreadMutex->Unlock();
		ISession* session = m_pSessionArray->Get(ssid);
		m_spExecuteJob->SetSession(session);
		m_rpIJobDispatcher->PushJob(m_spExecuteJob);
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptServices::_DoCleanup()
	{
		m_spExecuteJob->SetScriptServices(SmartPtr<ScriptServices>(NULL));
		while (!m_pUnhandleSessiones->empty())
		{
			uint64 ssid = m_pUnhandleSessiones->front();
			m_pUnhandleSessiones->pop_front();
			ISession* session = m_pSessionArray->Get(ssid);
			m_pSessionArray->Remove(ssid);
			Protocol* ptc = session->GetCallerProtocl();
			if (nullid != ptc->GetCallerGID())
			{
				ptc->ClearBuffer();
				IServicesSystem::Instance()->RemoteCallBack(ptc);
			}
		}
		m_pIScriptInstance->ClearMember();
		m_pUnhandleSessiones->clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptServices::Cleanup()
	{
		m_pIThreadMutex->Lock();
		m_isLiving = false;
		if (!m_isExecuting)
		{
			_DoCleanup();
		}
		m_pIThreadMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptServices::FixedThread()
	{
		byte index = m_rpIJobDispatcher->GetFixedIndex();
		m_spExecuteJob->SetThreadIndex(index);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ScriptServices::RemoteInvoker(Protocol* ptc)
	{
		if (m_CacheSize < m_SessionCount)
		{
			HADESWARNING("Service %s gid %llu is over cached!\r\n", m_ServiceName.c_str(), m_MachineUserData.first);
			if (nullid != ptc->GetCallerGID())
			{
				ptc->ClearBuffer();
				IServicesSystem::Instance()->RemoteCallBack(ptc);
			}
			return false;
		}
		else
		{
			ThreadSafeOprator::Increment(&m_SessionCount);
			ISession* session = IServicesSystem::Instance()->AllocSession();
			session->SetArgument(ptc);
			uint64 sessionid = m_pSessionArray->Push(session);
			session->SetSessionID(sessionid);

			m_pIThreadMutex->Lock();
			m_pUnhandleSessiones->push_back(sessionid);
			if (!m_isLiving)
			{
				m_pIThreadMutex->Unlock();
				HADESWARNING("Service %s is already dead\r\n", m_ServiceName.c_str());
			}
			else
			{
				if (m_isExecuting)
				{
					m_pIThreadMutex->Unlock();
				}
				else
				{
					m_isExecuting = true;
					_ExecuteSessionWithUnlock();
				}
			}
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ScriptServices::RemoteCallBack(Protocol* ptc)
	{
		uint64 ssid = ptc->GetCallerSession();
		if (!m_pSessionArray->Have(ssid))
		{
			HADESERROR("Service %s gid %llu got unkown callback with session gid %llu!\r\n", m_ServiceName.c_str(), m_MachineUserData.first);
			IServicesSystem::Instance()->FreeProtocol(ptc);
			return false;
		}
		else
		{
			ISession* session = m_pSessionArray->Get(ssid);
			session->SetResult(ptc);

			m_pIThreadMutex->Lock();
			m_pUnhandleSessiones->push_back(ssid);
			if (!m_isLiving)
			{
				m_pIThreadMutex->Unlock();
				HADESWARNING("Service %s is already dead\r\n", m_ServiceName.c_str());
			}
			else
			{
				if (m_isExecuting)
				{
					m_pIThreadMutex->Unlock();
				}
				else
				{
					m_isExecuting = true;
					_ExecuteSessionWithUnlock();
				}
			}
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptServices::Initialize(const ClassRegister* clr)
	{
		m_spExecuteJob->Initialize(clr);
	}
}