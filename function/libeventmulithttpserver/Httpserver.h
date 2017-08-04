#pragma once
#include "thread/IJobDispatcher.h"
#include "core/Singleton.hpp"
#include "core/SmartPtr.hpp"
#include "log/ILogSystem.h"
#include "core/IFunction.h"
#include "HttpserverJob.h"



namespace hc
{
	class IAgent;
	class IJobDispatcher;
}
namespace hlhsp
{
	class HttpserverJob;
	class Httpserver : public hc::IFunction
	{
		SYSTEM_SINGLETON_DECLEAR(Httpserver);
		RTTI_DRIVER_TYPE_DECLEAR(Httpserver, hc::IFunction);
	private:
		enum Constant
		{
			HC_PERPARE_EVBUFFER = 5000,
		};
	private:
		struct RequestPair
		{
			uint64	GID;
			uint	ThreadIndex;
			RequestPair();
			RequestPair(uint i, uint64 g);
			RequestPair(int t);
		};
	private:
		typedef eastl::vector< eastl::pair<hc::IAgent*, uint> > AgentArray;
		typedef eastl::vector< hc::SmartPtr<HttpserverJob> > ServerJobArray;
		typedef hc::Recycleablevector< RequestPair > RequestSourceArray;
	private:
		AgentArray					m_AgentArray;
		ServerJobArray				m_spHttpserverArray;
		RequestSourceArray*			m_pRequestSourceArray;
		eastl::string				m_RootDir;
		eastl::string				m_TempDir;
		hc::IJobDispatcher*			m_pJobThread;
		uint						m_AgentIndex;
		uint						m_PostSize;
	public:
		Httpserver();
		~Httpserver();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		void OnRequest(uint threadindex, uint64 tguid);
	public:
		bool StartServer(uint threadcount, HttpserverJob::ServerType st, uint port, bool isauto);
		void PushHandle(hc::IAgent* a);
		void Header(uint64 id, const char* key, const char* value);
		bool Reply(uint64 id, uint code, const char* buffer, uint len);
		const char* GetRequsetUrl(uint64 id);
		const char* GetAddress(uint64 id);
		eastl::string GetValue(uint64 id, const char* name, uint len);
		eastl::string GetType(uint64 id, const char* name, uint len);
		HttpserverJob::RequestCommand GetCommand(uint64 id);
	public:
		HADES_FORCEINLINE const char* GetRoot();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const char* Httpserver::GetRoot()
	{
		return m_RootDir.c_str();
	}
}