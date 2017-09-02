#include "memory/IMemorySystem.h"
#include "script/IScriptSystem.h"
#include "AllRegister.hpp"
#include "Httpserver.h"

#include <event2/event.h>

using namespace hc;
using namespace eastl;

namespace hlhsp
{
	RTTI_TYPE_IMPLEMENT(Httpserver);
	SYSTEM_SINGLETON_IMPLEMENT(Httpserver);
	//---------------------------------------------------------------------------------------------------------
	Httpserver::Httpserver()
		:m_spHttpserverJob(SmartPtr<HttpserverJob>(NEW HttpserverJob))
		, m_pJobThread(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	Httpserver::~Httpserver()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void Httpserver::Initialize()
	{
		{
			hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
			Register_All("httpserver", *sys);
		}
		event_set_mem_functions(
			&IMemorySystem::AllocMemory,
			&IMemorySystem::ReallocMemory,
			&IMemorySystem::ReleaseMemory
			);
#ifdef WIN32
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);
#endif
		m_pJobThread =
			static_cast<IJobDispatcher*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
		m_pJobThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pJobThread->SetThreadCount(1);
		m_pJobThread->SetName("Http server");
		m_pJobThread->Start();
	}
	//---------------------------------------------------------------------------------------------------------
	void Httpserver::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void Httpserver::Exit()
	{
		m_pJobThread->Stop();
		IThreadSystem::Instance()->RecycleBin(m_pJobThread);
	}
}
