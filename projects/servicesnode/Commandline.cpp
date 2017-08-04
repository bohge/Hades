#include "services/IServicesSystem.h"
#include "script/IArguments.h"
#include "services/Protocol.h"
#include "services/IAgent.h"
#include "Commandline.h"


#include <string>
#include <iostream>

using namespace hc;
namespace hsna
{
	//---------------------------------------------------------------------------------------------------------
	Commandline::Commandline()
		:m_pIAgent(NULL)
		, m_isRunning(true)
		, m_FuncCommandlineIndex(hc::nullhandle)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Commandline::~Commandline()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void Commandline::_DoYieldJob()
	{
		m_isRunning = false;
		HADESLOG("Press Enter to exit");
	}
	//---------------------------------------------------------------------------------------------------------
	void Commandline::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* data)
	{
		do 
		{
			std::string line;
			std::getline(std::cin, line);
			if (m_isRunning)
			{
				IArguments* argus = IScriptSystem::Instance()->AllocArguments();
				argus->PushString(line.c_str(), line.size());
				Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
				ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
				ptc->SetFunction(m_FuncCommandlineIndex);
				m_pIAgent->RemoteInvoker(ptc);
				IScriptSystem::Instance()->FreeArguments(argus);
			}
		} while (m_isRunning);
	}
	//---------------------------------------------------------------------------------------------------------
	void Commandline::Initialize(hc::IAgent* agent, uint index)
	{
		m_pIAgent = agent;
		m_FuncCommandlineIndex = index;
	}

}