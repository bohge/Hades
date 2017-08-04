#include "ClientJob.h"


#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/event_struct.h>

namespace hlec
{
	//---------------------------------------------------------------------------------------------------------
	ClientJob::ClientJob()
	{
	}

	//---------------------------------------------------------------------------------------------------------
	ClientJob::~ClientJob()
	{
	}

	//---------------------------------------------------------------------------------------------------------
	void ClientJob::_DoJob(hc::SmartPtr<hc::IJob>& self, Object* userdata)
	{

		event_base_dispatch(m_pBase);
		event_base_free(m_pBase);
	}

	//---------------------------------------------------------------------------------------------------------
	bool ClientJob::Initialize(const eastl::string& IP, const int& Port)
	{
		return true;
	}

	//---------------------------------------------------------------------------------------------------------
	void ClientJob::_DoYieldJob()
	{
	}

}