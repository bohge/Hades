#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "configure/IConfigSystem.h"
#include "script/IFunctionInvoker.h"
#include "script/IVirtualMachine.h"
#include "network/server/IServer.h"
#include "script/IScriptInstance.h"
#include "thread/IJobThreadArray.h"
#include "SinglenodeApplication.h"
#include "ScriptLibraryRegister.h"
#include "thread/IThreadSystem.h"
#include "configure/IConfigure.h"
#include "script/IScriptSystem.h"
#include "network/Messagepool.h"
#include "timer/ITimerSystem.h"
#include "file/IFileSystem.h"
#include "BalanceUserData.h"
#include "RequestUserData.h"
#include "BalanceHandle.h"
#include "RequestHandle.h"
#include "core/Types.h"
#include "Configure.h"





#include <windows.h>



using namespace hc;
using namespace eastl;
namespace hsna
{
	//---------------------------------------------------------------------------------------------------------
	SinglenodeApplication::SinglenodeApplication(void)
		:m_pIServer(NULL)
		, m_pRequestThread(NULL)
		, m_pRootScript(NULL)
		, m_pBalanceThread(NULL)
		, m_pIVirtualMachine(NULL)
		, m_pIScriptInstance(NULL)
		, m_pBalanceScript(NULL)
		, m_pMutex(NULL)
		, m_WorkThreadCount(0)
		, m_isUpdate(false)
		, m_pEventVector(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	SinglenodeApplication::~SinglenodeApplication(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string SinglenodeApplication::_Configurepath()
	{
		return Configure::CONFIG_FILE_PATH;
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::_OnInitialize()
	{
		for (size_t i = 0; i < SAC_PREPARE_HANDLE_SIZE; i++)
		{
			m_FreeRequestQueue.enqueue(SmartPtr<RequestHandle>(NEW RequestHandle));
		}
		for (size_t i = 0; i < SAC_PREPARE_HANDLE_SIZE; i++)
		{
			m_FreeBalanceQueue.enqueue(SmartPtr<BalanceHandle>(NEW BalanceHandle));
		}
		m_pEventVector = NEW EventVector;
		m_pEventVector->Initialize(SAC_PREPARE_HANDLE_SIZE);

		ScriptLibraryRegister();

		m_pRootScript = NEW string;
		m_pBalanceScript = NEW string;
		IConfigure* conf = IConfigSystem::Instance()->ConfigureFactory(_Configurepath());
		bool isdebug = conf->GetBool(Configure::DEBUG_FLAG);
		m_WorkThreadCount = conf->GetInt(Configure::WORK_THREAD_COUNT);
		string docs = conf->GetString(Configure::DOCUMENT_PATH);
		string scrs = conf->GetString(Configure::SCRIPT_PATH);
		*m_pRootScript = conf->GetString(Configure::MESSAGE_HANDLE);
		*m_pBalanceScript = conf->GetString(Configure::BALANCE_HANDLE);
		string timerjob = conf->GetString(Configure::TIMER_HANDLE);
		bool istls = conf->GetBool(Configure::USEING_TLS);
		m_WorkThreadCount = isdebug ? 1 : m_WorkThreadCount;

		IConfigSystem::Instance()->RecycleBin(conf);
		IFileSystem::Instance()->SetDocumentPath(docs);
		IFileSystem::Instance()->SetScriptPath(scrs);
		IScriptSystem::Instance()->SetDebug(isdebug);

		m_pMutex = static_cast<IThreadMutex*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_MUTEX));

		IServerSystem::ServerType st = istls ? IServerSystem::ST_OPENSSL_TLS : IServerSystem::ST_COMMON_TL;
		m_pIServer = IServerSystem::Instance()->ServerFactory(st);
		m_pIServer->SetServerCallback(MEMBER_FUNC_PTR(&SinglenodeApplication::OnReciveMessage));
		m_pBalanceThread =
			static_cast<IJobThreadArray*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_THREAD_ARRAY));
		m_pBalanceThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pBalanceThread->SetName("Balance");
		m_pBalanceThread->SetThreadCount(1);
		m_pBalanceThread->SetThreadDataCallback(
			MEMBER_FUNC_PTR(&SinglenodeApplication::CreateBalanceUserdata),
			MEMBER_FUNC_PTR(&SinglenodeApplication::RecycleBalanceUserdata));
		m_pBalanceThread->Start();

		m_pRequestThread =
			static_cast<IJobThreadArray*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_THREAD_ARRAY));
		m_pRequestThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pRequestThread->SetName("Worker");
		m_pRequestThread->SetThreadCount(m_WorkThreadCount);
		m_pRequestThread->SetThreadDataCallback(
			MEMBER_FUNC_PTR(&SinglenodeApplication::CreateRequestUserdata),
			MEMBER_FUNC_PTR(&SinglenodeApplication::RecycleRequestUserdata));
		m_pIVirtualMachine = IScriptSystem::Instance()->VirtualMachineFactory();
		m_pIScriptInstance = m_pIVirtualMachine->LoadScript(timerjob);
		m_pIVirtualMachine->SetGlobal(Configure::GLOBAL_APPLICATION, this, NULL, &ScriptValue::NullCleaner);
		m_pIVirtualMachine->SetGlobal(Configure::GLOBAL_MACHINE, m_pIVirtualMachine, NULL, &ScriptValue::NullCleaner);
		IFunctionInvoker* func = m_pIScriptInstance->SignFunciton(Configure::INITALIZE_FUNCTION);
		m_pRequestThread->Start();
		func->Invoke();
		m_pIScriptInstance->UnsignInvoker(func);
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::ScriptInitialized()
	{
		IConfigure* conf = IConfigSystem::Instance()->ConfigureFactory(_Configurepath());
		int connectcout = conf->GetInt(Configure::CONNECT_THREAD_COUNT);
		int port = conf->GetInt(Configure::LOCAL_PORT);
		IConfigSystem::Instance()->RecycleBin(conf);
		m_pIServer->Start(connectcout, port);
		m_isUpdate = true;
	}
	//---------------------------------------------------------------------------------------------------------
	bool SinglenodeApplication::_OnDowork()
	{
		Sleep(100);
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::_OnExit()
	{
		m_pIServer->Stop();
		m_pRequestThread->Stop();
		IScriptSystem::Instance()->RecycleBin(m_pIVirtualMachine);
		IThreadSystem::Instance()->RecycleBin(m_pRequestThread);
		IServerSystem::Instance()->RecycleBin(m_pIServer);
		IThreadSystem::Instance()->RecycleBin(m_pMutex);
		SAFE_DELETE(m_pRootScript);
		SAFE_DELETE(m_pBalanceScript);
		SAFE_DELETE(m_pEventVector);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::Object* SinglenodeApplication::CreateRequestUserdata()
	{
		RequestUserData* data = NEW RequestUserData;
		if (!data->Initialize(this, m_pIServer, *m_pRootScript))
		{
			SAFE_DELETE(data);
		}
		else
		{
			data->ThreadInitialize();
		}
		return data;
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::RecycleRequestUserdata(hc::Object* ud)
	{
		RequestUserData* data = static_cast<RequestUserData*>(ud);
		data->Exit();
		SAFE_DELETE(data);
	}
	//---------------------------------------------------------------------------------------------------------
	Object* SinglenodeApplication::CreateBalanceUserdata()
	{
		BalanceUserData* data = NEW BalanceUserData;
		data->rpApplication = this;
		data->pMachine = IScriptSystem::Instance()->VirtualMachineFactory();
		data->pInstance = data->pMachine->LoadScript(*m_pBalanceScript);
		data->pMachine->SetGlobal(Configure::GLOBAL_APPLICATION, this, NULL, &ScriptValue::NullCleaner);
		data->pFunction = data->pInstance->SignFuncitonWithReturn(hc::UInt::SID(), Configure::MESSAGE_FUNCTION, hc::UInt64::SID(), hc::UInt::SID(), hc::Message::SID());
		IFunctionInvoker* func = data->pInstance->SignFunciton(Configure::INITALIZE_FUNCTION, hc::UInt::SID());
		func->Invoke(&m_WorkThreadCount);
		data->pInstance->UnsignInvoker(func);
		return data;
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::RecycleBalanceUserdata(Object* vm)
	{
		BalanceUserData* data = static_cast<BalanceUserData*>(vm);
		IScriptSystem::Instance()->RecycleBin(data->pMachine);
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::OnReciveMessage(uint64 id, hc::CallbackType ct, hc::Message* message)
	{
		message = NULL != message
			? message
			: Messagepool::Instance()->AllocMessage();
		hc::SmartPtr<BalanceHandle> h;
		_GetBalanceHandle(h);
		h->SetMessage(message);
		h->SetClient(id);
		h->SetCallbackType(ct);
		m_pBalanceThread->PushJob(0, h);
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::DispatchRequest(uint thread, uint64 id, hc::CallbackType ct, hc::Message* message)
	{
		hc::SmartPtr<RequestHandle> h;
		_GetRequestHandle(h);
		h->SetMessage(message);
		h->SetClient(id);
		h->SetCallbackType(ct);
		m_pRequestThread->PushJob(thread >= m_WorkThreadCount ? 0 : thread, h);
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::OnTimerCallback(uint64 h)
	{
		//我们的更新选择在更新线程中发送数据到计算线程
		//选择这种模式，而不是在更新线程直接更新最大的原因
		//是应为将来我们使用分布式系统，将相同内存指针的操作放到对应的计算服务组效率比较高
		if (m_isUpdate)
		{
			if (m_pEventVector->Have(h))
			{
				m_pIVirtualMachine->Update();
				EventPair& data = m_pEventVector->Get(h);
				data.second->Invoke();
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 SinglenodeApplication::RegisterTimer(float sec, int funcid)
	{
		m_pMutex->Lock();
		IFunctionInvoker* func = m_pIScriptInstance->SignFunciton(funcid);
		uint64 index = m_pEventVector->Push(make_pair((CallbackHandle)NULL, func));
		CallbackHandle handle = ITimerSystem::Instance()->RegisterTimer(sec, index, MEMBER_FUNC_PTR(&SinglenodeApplication::OnTimerCallback));
		EventPair& data = m_pEventVector->Get(index);
		data.first = handle;
		m_pMutex->Unlock();
		return index;
	}
	//---------------------------------------------------------------------------------------------------------
	void SinglenodeApplication::UnregisterTimer(uint64 h)
	{
		m_pMutex->Lock();
		if (m_pEventVector->Have(h))
		{
			EventPair& data = m_pEventVector->Get(h);
			ITimerSystem::Instance()->UnregisterTimer(data.first);
			m_pIScriptInstance->UnsignInvoker(data.second);
			m_pEventVector->Remove(h);
		}
		m_pMutex->Unlock();
	}
}