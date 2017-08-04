#include "network/socket/ISocketSystem.h"
#include "script/IFunctionInvoker.h"
#include "configure/IConfigSystem.h"
#include "thread/IJobThreadArray.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"
#include "network/socket/ISocket.h"
#include "ScriptLibraryRegister.h"
#include "thread/IJobDispatcher.h"
#include "thread/IThreadSystem.h"
#include "configure/IConfigure.h"
#include "timer/ITimerSystem.h"
#include "MonkeyApplication.h"
#include "file/IFileSystem.h"
#include "log/ILogSystem.h"
#include "MonkeyHandle.h"
#include "core/Types.h"
#include "Configure.h"


#include <iostream>
#include <sstream>


using namespace hc;
namespace hmka
{
	//---------------------------------------------------------------------------------------------------------
	MonkeyApplication::MonkeyApplication(void)
		: m_pMonkeyThread(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	MonkeyApplication::~MonkeyApplication(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string MonkeyApplication::_Configurepath()
	{
		return Configure::CONFIG_FILE_PATH;
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyApplication::_OnInitialize()
	{
		ScriptLibraryRegister();

		IConfigSystem::Instance()->SetDefualtConfigure(_Configurepath());
		IConfigure* conf = IConfigSystem::Instance()->GetDefualtConfigure();
		int workcout = conf->GetInt(Configure::WORK_THREAD_COUNT);
		int connectcout = conf->GetInt(Configure::CONNECT_THREAD_COUNT);
		int monkycount = conf->GetInt(Configure::MONKEY_COUNT);
		int freq = conf->GetInt(Configure::LOOP_FREQUNCY);
		bool istls = conf->GetBool(Configure::USEING_TLS);
		uint logsize = conf->GetInt(Configure::LOG_FILE_SIZE);
		eastl::string logfile = conf->GetString(Configure::LOG_FILE_NAME);
		eastl::string docs = conf->GetString(Configure::DOCUMENT_PATH);
		eastl::string scrs = conf->GetString(Configure::SCRIPT_PATH);
		eastl::string monkeyjob = conf->GetString(Configure::MONKEY_HANDLE);

		IFileSystem::Instance()->SetDocumentPath(docs);
		IFileSystem::Instance()->SetScriptPath(scrs);
		ILogSystem::Instance()->SetLogFile(logsize, logfile);

		//Ïß³Ì·¢ËÍ
		m_pMonkeyThread =
			static_cast< IJobDispatcher* >(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));

		m_pMonkeyThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pMonkeyThread->SetName("Worker");
		m_pMonkeyThread->SetThreadCount(workcout);
		m_pMonkeyThread->Start();


		m_MonkeyArray.reserve(monkycount);
		for (int i = 0; i < monkycount; i++){
			hc::SmartPtr<MonkeyHandle> h = SmartPtr<MonkeyHandle>(NEW MonkeyHandle);
			h->Initialize(monkeyjob);
			m_MonkeyArray.push_back(h);
		}

		float time = freq;
		time = 60.0f * 60.0f / time;
		ITimerSystem::Instance()->
			RegisterTimer(time, nullid,
			MEMBER_FUNC_PTR(&MonkeyApplication::OnTimerCallback));
	}
	//---------------------------------------------------------------------------------------------------------
	bool MonkeyApplication::_OnDowork()
	{
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyApplication::_OnExit()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyApplication::OnTimerCallback(uint64 gid)
	{
		for (int i = 0; i < m_MonkeyArray.size(); ++i)
		{
			hc::SmartPtr<MonkeyHandle>& h = m_MonkeyArray[i];
			if (h->CheckPushJob())
			{
				m_pMonkeyThread->PushJob(h);
			}
		}
	}
}