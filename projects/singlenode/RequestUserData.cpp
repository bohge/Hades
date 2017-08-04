#include "serialize/ISerializeSystem.h"
#include "script/ICoroutineInvoker.h"
#include "script/IFunctionInvoker.h"
#include "script/IScriptInstance.h"
#include "network/server/IServer.h"
#include "SinglenodeApplication.h"
#include "script/IScriptSystem.h"
#include "RequestUserData.h"
#include "network/Message.h"
#include "RequestHandle.h"
#include "core/Types.h"
#include "Configure.h"




using namespace hc;
namespace hsna
{
	RTTI_TYPE_IMPLEMENT(RequestUserData);
	//---------------------------------------------------------------------------------------------------------
	RequestUserData::RequestUserData()
		: m_pVirtualMachine(NULL)
		, m_pSerializer(NULL)
		, m_pApplication(NULL)
		, m_pScriptInstance(NULL)
		, m_pServer(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	RequestUserData::~RequestUserData()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	hc::ICoroutineInvoker* RequestUserData::_CreateOrCached(CoroutineList& corous, const char* cname)
	{
		ICoroutineInvoker* res;
		if (corous.empty())
		{
			res = m_pScriptInstance->SignCoroutine(cname);
			res->SetupResume(RESUME_BOOL, Bool::SID());
			res->SetupResume(RESUME_ACESSOR, Instance::SID());
		}
		else
		{
			res = corous.back();
			corous.pop_back();
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool RequestUserData::Initialize(SinglenodeApplication* app, hc::IServer* server, const eastl::string& entry)
	{
		m_pSerializer = ISerializeSystem::Instance()->SerializerFactory();
		m_pVirtualMachine = IScriptSystem::Instance()->VirtualMachineFactory();
		m_pScriptInstance = m_pVirtualMachine->LoadScript(entry);
		if (NULL == m_pScriptInstance)
		{
			ISerializeSystem::Instance()->RecycleBin(m_pSerializer);
			IScriptSystem::Instance()->RecycleBin(m_pVirtualMachine);
			m_pSerializer = NULL;
			m_pVirtualMachine = NULL;
			return false;
		}
		else
		{
			for (int i = 0; i < RUDC_PREPARE_INVOKER_COUNT; ++i)
			{
				ICoroutineInvoker * connect = m_pScriptInstance->SignCoroutine(Configure::CONNECT_FUNCTION);
				connect->SetupResume(RESUME_BOOL, Bool::SID());
				connect->SetupResume(RESUME_ACESSOR, Instance::SID());
				ICoroutineInvoker * request = m_pScriptInstance->SignCoroutine(Configure::MESSAGE_FUNCTION);
				request->SetupResume(RESUME_BOOL, Bool::SID());
				request->SetupResume(RESUME_ACESSOR, Instance::SID());
				ICoroutineInvoker * disconn = m_pScriptInstance->SignCoroutine(Configure::DISCONNECT_FUNCTION);
				disconn->SetupResume(RESUME_BOOL, Bool::SID());
				disconn->SetupResume(RESUME_ACESSOR, Instance::SID());
				m_ConnectCoroutineArray.push_back(connect);
				m_pEntryCoroutineArray.push_back(request);
				m_pDisconnectCoroutineArray.push_back(disconn);
			}
			m_pVirtualMachine->SetGlobal(Configure::GLOBAL_SERVER, server, NULL, &ScriptValue::NullCleaner);
			m_pVirtualMachine->SetGlobal(Configure::GLOBAL_APPLICATION, app, NULL, &ScriptValue::NullCleaner);
			m_pVirtualMachine->SetGlobal(Configure::GLOBAL_MACHINE, m_pVirtualMachine, NULL, &ScriptValue::NullCleaner);
			m_pIFunctionInvoker = m_pScriptInstance->SignFunciton(Configure::INITALIZE_FUNCTION);
			m_pApplication = app;
			m_pServer = server;
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestUserData::Exit()
	{
		IScriptSystem::Instance()->RecycleBin(m_pVirtualMachine);
		ISerializeSystem::Instance()->RecycleBin(m_pSerializer);
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestUserData::DsctoryCoroutine(hc::ICoroutineInvoker* corous)
	{
		m_pScriptInstance->UnsignInvoker(corous);
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestUserData::ThreadInitialize()
	{
		if (NULL != m_pIFunctionInvoker)
		{
			m_pIFunctionInvoker->Invoke();
			m_pScriptInstance->UnsignInvoker(m_pIFunctionInvoker);
			m_pIFunctionInvoker = NULL;
		}
	}

}
