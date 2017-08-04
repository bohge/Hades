#pragma once
#include "thread/IJob.h"
#include "LibeventClient.h"
#include "timer/ITimerSystem.h"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"


struct event_base;
struct bufferevent;
namespace hlec
{
	class ClientJob : public hc::IJob
	{
	public:
		ClientJob();
		~ClientJob();
		bool Initialize(const eastl::string& IP, const int& Port);
	private:
		event_base*			m_pBase;
		bufferevent*		m_pBufferevent;
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, Object* userdata);
	public:
		HADES_FORCEINLINE void SetBase(event_base& base);
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ClientJob::SetBase(event_base& base)
	{
		m_pBase = &base;
	}

}
