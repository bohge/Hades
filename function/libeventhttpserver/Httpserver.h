#pragma once
#include "thread/IJobDispatcher.h"
#include "core/Singleton.hpp"
#include "core/SmartPtr.hpp"
#include "core/IFunction.h"
#include "HttpserverJob.h"

namespace hc{class IJobDispatcher;}
namespace hlhsp
{
	class HttpserverJob;
	class Httpserver : public hc::IFunction
	{
		SYSTEM_SINGLETON_DECLEAR(Httpserver);
		RTTI_DRIVER_TYPE_DECLEAR(Httpserver, hc::IFunction);
	private:
		hc::IJobDispatcher*				m_pJobThread;
		hc::SmartPtr<HttpserverJob>		m_spHttpserverJob;
	public:
		Httpserver();
		~Httpserver();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		HADES_FORCEINLINE void PushHandle(hc::IAgent* a);
		HADES_FORCEINLINE void StartServer(HttpserverJob::ServerType st, uint port, bool isauto);
		HADES_FORCEINLINE void Header(uint64 id, const char* key, const char* value);
		HADES_FORCEINLINE bool Reply(uint64 id, uint code, const char* buffer, uint len);
		HADES_FORCEINLINE const char* GetRequsetUrl(uint64 id);
		HADES_FORCEINLINE const char* GetAddress(uint64 id);
		HADES_FORCEINLINE eastl::string GetValue(uint64 id, const char* name, uint len);
		HADES_FORCEINLINE eastl::string GetType(uint64 id, const char* name, uint len);
		HADES_FORCEINLINE HttpserverJob::RequestCommand GetCommand(uint64 id);
		HADES_FORCEINLINE const char* GetRoot();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Httpserver::StartServer(HttpserverJob::ServerType st, uint port, bool isauto)
	{
		if (m_spHttpserverJob->Initialize(st, port, isauto))
		{
			m_pJobThread->PushJob(m_spHttpserverJob);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Httpserver::PushHandle(hc::IAgent* a)
	{
		m_spHttpserverJob->Pushhandle(a);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool Httpserver::Reply(uint64 id, uint code, const char* buffer, uint len)
	{
		return m_spHttpserverJob->Reply(id, code, buffer, len);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const char* Httpserver::GetAddress(uint64 id)
	{
		return m_spHttpserverJob->GetAddress(id);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const char* Httpserver::GetRequsetUrl(uint64 id)
	{
		return m_spHttpserverJob->GetRequsetUrl(id);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE eastl::string Httpserver::GetValue(uint64 id, const char* name, uint len)
	{
		return m_spHttpserverJob->GetValue(id, name, len);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE eastl::string Httpserver::GetType(uint64 id, const char* name, uint len)
	{
		return m_spHttpserverJob->GetType(id, name, len);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE HttpserverJob::RequestCommand Httpserver::GetCommand(uint64 id)
	{
		return m_spHttpserverJob->GetCommand(id);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const char* Httpserver::GetRoot()
	{
		return m_spHttpserverJob->GetRoot();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Httpserver::Header(uint64 id, const char* key, const char* value)
	{
		m_spHttpserverJob->Header(id, key, value);
	}
}