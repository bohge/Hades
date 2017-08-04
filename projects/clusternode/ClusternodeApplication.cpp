#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "services/IServicesSystem.h"
#include "configure/IConfigSystem.h"
#include "ClusternodeApplication.h"
#include "script/IVirtualMachine.h"
#include "network/server/IServer.h"
#include "serialize/ISerializer.h"
#include "ScriptLibraryRegister.h"
#include "configure/IConfigure.h"
#include "script/IScriptSystem.h"
#include "thread/IThreadMutex.h"
#include "serialize/IDecoder.h"
#include "timer/ITimerSystem.h"
#include "file/IFileSystem.h"
#include "services/IAgent.h"
#include "RequestHandle.h"
#include "Configure.h"





using namespace hc;
using namespace eastl;
namespace hsna
{
	//---------------------------------------------------------------------------------------------------------
	ClusternodeApplication::ClusternodeApplication(void)
		:m_pIAgent(NULL)
		, m_FuncInitIndex(nullhandle)
		, m_FuncConnectIndex(nullhandle)
		, m_pTimerMutex(NULL)
		, m_pRequestHandleArray(NULL)
		, m_pIServer(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ClusternodeApplication::~ClusternodeApplication(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string ClusternodeApplication::_Configurepath()
	{
		return Configure::CONFIG_FILE_PATH;
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::_OnInitialize()
	{
		ScriptLibraryRegister();
		m_pRequestHandleArray = NEW RequestHandleArray;
		m_pRequestHandleArray->Initialize(SNAC_CLIENT_CACHE_SIZE);

		IConfigSystem::Instance()->SetDefualtConfigure(_Configurepath());
		IConfigure* conf = IConfigSystem::Instance()->GetDefualtConfigure();
		int workthread = conf->GetInt(Configure::WORK_THREAD_COUNT);
		int cachesize = conf->GetInt(Configure::SERVICES_CACHE_SIZE);
		string docs = conf->GetString(Configure::DOCUMENT_PATH);
		string scrs = conf->GetString(Configure::SCRIPT_PATH);
		string scriptmain = conf->GetString(Configure::SCRIPT_MAIN);
		string logfile = conf->GetString(Configure::LOG_FILE_NAME);
		bool istls = conf->GetBool(Configure::USEING_TLS);
		bool isopt = conf->GetBool(Configure::RPC_OPTIMIZATION);


		IServicesSystem::Instance()->SetRPCOptimization(isopt);
		ILogSystem::Instance()->SetLogFileName(logfile);
		ISerializeSystem::Instance()->PrepareSerializable(Message::RTTI().GetFactor(), SNAC_MESSAGE_CACHE_SIZE);
		IFileSystem::Instance()->SetDocumentPath(docs);
		IFileSystem::Instance()->SetScriptPath(scrs);

		IServicesSystem::Instance()->SetCacheSize(cachesize);
		IServicesSystem::Instance()->StartServices(workthread);
		IServicesSystem::Instance()->SetOnCreateCallback(MEMBER_FUNC_PTR(&ClusternodeApplication::OnCreateServices));

		m_pTimerMutex = static_cast<IThreadMutex*>(IThreadSystem::Instance()->
			ThreadEntityFactory(ThreadProperty::ET_MUTEX));
		IServerSystem::ServerType st = istls ? IServerSystem::ST_OPENSSL_TLS : IServerSystem::ST_COMMON_TL;
		m_pIServer = IServerSystem::Instance()->ServerFactory(st);

		m_pIAgent = IServicesSystem::Instance()->ServiceFactory(scriptmain);
		if (m_pIAgent)
		{
			m_FuncInitIndex = m_pIAgent->GetFunctionIndex(Configure::INITALIZE_FUNCTION);
			m_FuncConnectIndex = m_pIAgent->GetFunctionIndex(Configure::CONNECT_FUNCTION);
			m_FuncDisconnectIndex = m_pIAgent->GetFunctionIndex(Configure::DISCONNECT_FUNCTION);
			Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
			ptc->SetFunction(m_FuncInitIndex);
			m_pIAgent->RemoteInvoker(ptc);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool ClusternodeApplication::_OnDowork()
	{
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::_OnExit()
	{
		SAFE_DELETE(m_pRequestHandleArray);
		m_pIServer->Stop();
		IServerSystem::Instance()->RecycleBin(m_pIServer);
		IServicesSystem::Instance()->RecycleBin(m_pIAgent);
		IThreadSystem::Instance()->RecycleBin(m_pTimerMutex);
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::ScriptInitialized()
	{
		IConfigure* conf = IConfigSystem::Instance()->GetDefualtConfigure();
		int connectcout = conf->GetInt(Configure::CONNECT_THREAD_COUNT);
		int port = conf->GetInt(Configure::LOCAL_PORT);
		m_pIServer->SetServerCallback(MEMBER_FUNC_PTR(&ClusternodeApplication::OnReciveMessage));
		m_pIServer->Start(connectcout, port);
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::OnCreateServices(hc::IVirtualMachine* vm)
	{
		vm->SetGlobal(Configure::GLOBAL_SERVER, m_pIServer, NULL, ScriptValue::NullCleaner);
		vm->SetGlobal(Configure::GLOBAL_APPLICATION, this, NULL, ScriptValue::NullCleaner);
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::OnReciveMessage(uint64 id, hc::CallbackType ct, hc::Message* message)
	{
		Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
		ptc->SetClient(id);
		ptc->SetMessage(message);
		switch (ct)
		{
		case SCT_CONNECT:
		{
							ptc->SetFunction(m_FuncConnectIndex);
							m_pIAgent->RemoteInvoker(ptc);
							break;
		}
		case SCT_REQUEST:
		{
							uint64 handlegid = m_pIServer->GetSpecific(id);
							if (!m_pRequestHandleArray->Have(handlegid))
							{
								HADESERROR("empty gid handle %llu\r\n", handlegid);
							}
							else
							{
								SmartPtr<RequestHandle> prh = m_pRequestHandleArray->Get(handlegid);
								ptc->SetFunction(prh->OnRequestFunction);
								prh->pAgent->RemoteInvoker(ptc);
							}
							break;
		}
		case SCT_DISCONNECT:
		{
							   uint64 handlegid = m_pIServer->GetSpecific(id);
							   if (m_pRequestHandleArray->Have(handlegid))
							   {
								   SmartPtr<RequestHandle> prh = m_pRequestHandleArray->Get(handlegid);
								   ptc->SetFunction(m_FuncDisconnectIndex);
								   m_pIAgent->RemoteInvoker(ptc);
								   m_pRequestHandleArray->Remove(handlegid);
							   }
							   break;
		}
		default:ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::SetClientHandle(uint64 gid, hc::IAgent* agent)
	{
		const char* name = agent->GetTypeName();
		uint hash = Utility::HashCode(name);
		uint funcindex;
		RequestHandleMap::iterator it = m_RequestHandleMap.find(hash);
		if (m_RequestHandleMap.end() != it)
		{
			funcindex = it->second;
		}
		else
		{
			funcindex = agent->GetFunctionIndex(Configure::MESSAGE_FUNCTION);
			m_RequestHandleMap.insert(make_pair(hash, funcindex));
		}
		SmartPtr<RequestHandle> prh = SmartPtr<RequestHandle>(NEW RequestHandle);
		SmartPtr<IServices> services = IServicesSystem::Instance()->GetSerivices(agent->GetGID());
		if (services.isValid())
		{
			prh->pAgent = services->AllocAgent();
			prh->OnRequestFunction = funcindex;
			uint64 handlegid = m_pRequestHandleArray->Push(prh);
			m_pIServer->SetSpecific(gid, handlegid);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::OnTimerCallback(uint64 gid)
	{
		m_pTimerMutex->Lock();
		UpdateMap::iterator it = m_UpdateMap.find(gid);
		if (m_UpdateMap.end() != it)
		{
			for (ServiceFunctionList::iterator sf = it->second->begin();
				it->second->end() != sf; )
			{
				if (!IServicesSystem::Instance()->hasSerivices(sf->first))
				{
					ServiceFunctionList::iterator was = sf;
					++sf;
					it->second->erase(was);
				}
				else
				{
					Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
					ptc->SetFunction(sf->second);
					IServicesSystem::Instance()->RemoteInvoke(sf->first, ptc);
					++sf;
				}
			}
		}
		m_pTimerMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void ClusternodeApplication::RegisterTimer(hc::IAgent* agent, uint freq, const eastl::string& function)
	{
		if (NULL != agent)
		{
			uint funcid = agent->GetFunctionIndex(function);
			uint64 servicesid = agent->GetGID();
			if (0 != freq)
			{
				m_pTimerMutex->Lock();
				ServiceFunctionList*  sflist;
				UpdateMap::iterator it = m_UpdateMap.find(freq);
				if (m_UpdateMap.end() != it)
				{
					sflist = it->second;
				}
				else
				{
					sflist = NEW ServiceFunctionList;
					m_UpdateMap.insert(make_pair(freq, sflist));
					float time = freq;
					time = 60.0f * 60.0f / time;
					ITimerSystem::Instance()->
						RegisterTimer(time, freq,
						MEMBER_FUNC_PTR(&ClusternodeApplication::OnTimerCallback));
				}
				sflist->push_back(make_pair(servicesid, funcid));
				m_pTimerMutex->Unlock();
			}
		}
	}
};