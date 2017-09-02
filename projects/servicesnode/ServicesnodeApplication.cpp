#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "services/IServicesSystem.h"
#include "network/message/IMessage.h"
#include "ServicesnodeApplication.h"
#include "configure/IConfigSystem.h"
#include "thread/IJobDispatcher.h"
#include "configure/IConfigure.h"
#include "script/IArguments.h"
#include "file/IFileSystem.h"
#include "services/IAgent.h"
#include "core/Bufferpool.h"
#include "Commandline.h"
#include "Configure.h"





using namespace hc;
using namespace eastl;
namespace hsna
{
	//---------------------------------------------------------------------------------------------------------
	ServicesnodeApplication::ServicesnodeApplication(void)
		: m_pIAgent(NULL)
		, m_FuncInitIndex(nullhandle)
		, m_FuncCommandlineIndex(nullhandle)
		, m_pCommandlineThread(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ServicesnodeApplication::~ServicesnodeApplication(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void ServicesnodeApplication::SetExecutableName(const eastl::string& exename)
	{
		char buffer[512];
		sprintf(buffer, Configure::CONFIG_FILE_PATH, exename.c_str());
		m_ConfigureName = buffer;
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string ServicesnodeApplication::_Configurepath()
	{
		return m_ConfigureName;
	}
	//---------------------------------------------------------------------------------------------------------
	void ServicesnodeApplication::_OnInitialize(uint pid, bool daemon, const eastl::string& args)
	{
		IConfigure* conf = IConfigSystem::Instance()->GetDefualtConfigure();
		uint buffersize = conf->GetInt(Configure::MAX_NET_BUFFER_SIZE);
		int workthread = conf->GetInt(Configure::WORK_THREAD_COUNT);
		int cachesize = conf->GetInt(Configure::SERVICES_CACHE_SIZE);
		string docs = conf->GetString(Configure::DOCUMENT_PATH);
		string scrs = conf->GetString(Configure::SCRIPT_PATH);
		string scriptmain = conf->GetString(Configure::SCRIPT_MAIN);
		string logfile = conf->GetString(Configure::LOG_FILE_NAME);
		uint filesize = conf->GetInt(Configure::LOG_FILE_SIZE);
		bool isopt = conf->GetBool(Configure::RPC_OPTIMIZATION);

		Bufferpool::Instance()->SetMaxBUfferSize(buffersize);
		IServerSystem::Instance()->SetMaxBUfferSize(buffersize);
		IServicesSystem::Instance()->SetRPCOptimization(isopt);
		ILogSystem::Instance()->SetLogFile(filesize, daemon ? pid : 0, logfile);
		ISerializeSystem::Instance()->PrepareSerializable(IMessage::RTTI().GetFactor(), SNAC_MESSAGE_CACHE_SIZE);
		IFileSystem::Instance()->SetDocumentPath(docs);
		IFileSystem::Instance()->SetScriptPath(scrs);

		IServicesSystem::Instance()->SetCacheSize(cachesize);
		IServicesSystem::Instance()->StartServices(workthread);

		m_pIAgent = IServicesSystem::Instance()->ServiceFactory(scriptmain);
		if (m_pIAgent)
		{
			m_FuncInitIndex = m_pIAgent->GetFunctionIndex(Configure::INITALIZE_FUNCTION);
			m_FuncCommandlineIndex = m_pIAgent->GetFunctionIndex(Configure::COMMANDLINE_FUNCTION);
			if (!daemon)_InitializeCommandline();
			Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
			ptc->SetFunction(m_FuncInitIndex);
			IArguments* argus = IScriptSystem::Instance()->AllocArguments();
			argus->PushString(args.c_str(), args.size());
			ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
			IScriptSystem::Instance()->FreeArguments(argus);
			m_pIAgent->RemoteInvoker(ptc);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ServicesnodeApplication::_InitializeCommandline()
	{
		if (nullhandle != m_FuncCommandlineIndex)
		{
			m_pCommandlineThread =
				static_cast<IJobDispatcher*>(
				IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
			m_pCommandlineThread->SetSortType(ThreadProperty::ST_UNSORT);
			m_pCommandlineThread->SetThreadCount(1);
			m_pCommandlineThread->SetName("Commandline");
			m_pCommandlineThread->Start();

			m_spCommandlineJob = hc::SmartPtr<Commandline>(NEW Commandline);
			m_spCommandlineJob->Initialize(m_pIAgent, m_FuncCommandlineIndex);
			m_pCommandlineThread->PushJob(m_spCommandlineJob);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool ServicesnodeApplication::_OnDowork()
	{
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void ServicesnodeApplication::_OnExit()
	{
		IServicesSystem::Instance()->RecycleBin(m_pIAgent);	
		IThreadSystem::Instance()->RecycleBin(m_pCommandlineThread);
	}
};