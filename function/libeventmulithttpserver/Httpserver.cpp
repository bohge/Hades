#include "services/IServicesSystem.h"
#include "configure/IConfigSystem.h"
#include "script/IScriptSystem.h"
#include "configure/IConfigure.h"
#include "memory/IMemorySystem.h"
#include "script/IArguments.h"
#include "file/IFileSystem.h"
#include "services/IAgent.h"
#include "AllRegister.hpp"
#include "Httpserver.h"

#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <event2/keyvalq_struct.h>
#include <event2/event_struct.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include <event2/thread.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/util.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace hc;
using namespace eastl;

namespace hlhsp
{
	RTTI_TYPE_IMPLEMENT(Httpserver);
	SYSTEM_SINGLETON_IMPLEMENT(Httpserver);
	//---------------------------------------------------------------------------------------------------------
	Httpserver::RequestPair::RequestPair(uint i, uint64 g)
		:GID(g)
		, ThreadIndex(i)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	Httpserver::RequestPair::RequestPair(int t)
		:GID(hc::nullid)
		, ThreadIndex(hc::nullhandle)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	Httpserver::RequestPair::RequestPair()
		:GID(hc::nullid)
		, ThreadIndex(hc::nullhandle)
	{

	}

	//---------------------------------------------------------------------------------------------------------
	Httpserver::Httpserver()
		:m_AgentIndex(0)
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
		m_pRequestSourceArray = NEW RequestSourceArray;
		m_pRequestSourceArray->SetBeginUUID(3252);
		m_pRequestSourceArray->Initialize(HC_PERPARE_EVBUFFER);
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
		SSL_library_init();
		SSL_load_error_strings();
		OpenSSL_add_all_algorithms();
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
		FOR_EACH(AgentArray, agent, m_AgentArray)
		{
			IServicesSystem::Instance()->RecycleBin(agent->first);
		}
		SAFE_DELETE(m_pRequestSourceArray);
	}
	//---------------------------------------------------------------------------------------------------------
	void Httpserver::PushHandle(IAgent* agent)
	{
		if (NULL != m_pJobThread)
		{
			HADESERROR("Push handler befor http server start!");
		}
		else
		{
			if (NULL != agent)
			{
				IAgent* na = (IAgent*)agent->Duplicate();
				na->FromBuffer(agent->GetBuffer(), agent->GetLength());
				uint reqfunc = na->GetFunctionIndex("OnRequest");
				m_AgentArray.push_back(eastl::make_pair(na, reqfunc));
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Httpserver::OnRequest(uint threadindex, uint64 tguid)
	{
		if (!m_AgentArray.empty())
		{
			uint index = m_AgentIndex;
			eastl::pair<IAgent*, uint>& agent = m_AgentArray[index];
			++index;
			m_AgentIndex = index < m_AgentArray.size() ? index : 0;

			uint64 uid = m_pRequestSourceArray->Push(RequestPair(threadindex, tguid));
			Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
			IArguments* argus = IScriptSystem::Instance()->AllocArguments();
			argus->PushUInt64(uid);
			ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
			IScriptSystem::Instance()->FreeArguments(argus);
			ptc->SetFunction(agent.second);
			if (!agent.first->RemoteInvoker(ptc))
			{
				Reply(uid, 408, "Over time!", 10);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool Httpserver::StartServer(uint threadcount, HttpserverJob::ServerType st, uint port, bool isauto)
	{
		string crt;
		string key;
		IConfigure* conf = IConfigSystem::Instance()->ConfigureFactory("root:HTTP.json");
		if (!conf)
		{
			HADESERROR("cannot parse config root:HTTP.json");
			return false;
		}
		m_PostSize = conf->GetInt("MAXSIZE");
		string script = conf->GetString("HANDLER");
		m_TempDir = conf->GetString("TEMPDIR");
		m_RootDir = conf->GetString("HTTPROOT");
		if (HttpserverJob::ST_HTTPS == st)
		{
			crt = conf->GetString("OPENSSL_CRT");
			key = conf->GetString("OPENSSL_KEY");
			crt = IFileSystem::Instance()->PathAssembly(crt);
			key = IFileSystem::Instance()->PathAssembly(key);
		}
		IConfigSystem::Instance()->RecycleBin(conf);
		m_TempDir = IFileSystem::Instance()->PathAssembly(m_TempDir);
		m_RootDir = IFileSystem::Instance()->PathAssembly(m_RootDir);

		/* Now we tell the evhttp what port to listen on */
		evutil_socket_t fd = evhttp_bind_listen("0.0.0.0", port);
		if (-1 == fd) {
			HADESERROR("couldn't bind to port %d. Exiting.\n", (int)port);
			return false;
		}
		HADESLOG("HTTP server listening on %d", port);
		//Æô¶¯Ïß³Ì
		m_pJobThread =
			static_cast<IJobDispatcher*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
		m_pJobThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pJobThread->SetThreadCount(threadcount);
		m_pJobThread->SetName("Http server");
		m_pJobThread->Start();

		for (int i = 0; i < threadcount; ++i)
		{
			SmartPtr<HttpserverJob> hsj = SmartPtr<HttpserverJob>(NEW HttpserverJob(i, m_PostSize, m_RootDir, m_TempDir));
			hsj->SetRequestCallback(MEMBER_FUNC_PTR(&Httpserver::OnRequest));
			hsj->Initialize(fd, st, isauto, crt, key);
			m_spHttpserverArray.push_back(hsj);
			m_pJobThread->PushJob(hsj);
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void  Httpserver::Header(uint64 id, const char* key, const char* value)
	{
		if (m_pRequestSourceArray->Have(id))
		{
			RequestPair p = m_pRequestSourceArray->Get(id);
			if (hc::nullid != p.GID && hc::nullhandle != p.ThreadIndex)
			{
				hc::SmartPtr<HttpserverJob>& ptr = m_spHttpserverArray[p.ThreadIndex];
				ptr->Header(p.GID, key, value);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool Httpserver::Reply(uint64 id, uint code, const char* buffer, uint len)
	{
		if (m_pRequestSourceArray->Have(id))
		{
			RequestPair p = m_pRequestSourceArray->Get(id);
			if (hc::nullid != p.GID && hc::nullhandle != p.ThreadIndex)
			{
				hc::SmartPtr<HttpserverJob>& ptr = m_spHttpserverArray[p.ThreadIndex];
				ptr->Reply(p.GID, code, buffer, len);
				m_pRequestSourceArray->Remove(id);
				return true;
			}
		}
		HADESERROR("%llu request is already replayed, handler code have multi-thread replay!!", id);
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* Httpserver::GetAddress(uint64 id)
	{
		if (m_pRequestSourceArray->Have(id))
		{
			RequestPair p = m_pRequestSourceArray->Get(id);
			if (hc::nullid != p.GID && hc::nullhandle != p.ThreadIndex)
			{
				hc::SmartPtr<HttpserverJob>& ptr = m_spHttpserverArray[p.ThreadIndex];
				return ptr->GetAddress(p.GID);
			}
		}
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* Httpserver::GetRequsetUrl(uint64 id)
	{
		if (m_pRequestSourceArray->Have(id))
		{
			RequestPair p = m_pRequestSourceArray->Get(id);
			if (hc::nullid != p.GID && hc::nullhandle != p.ThreadIndex)
			{
				hc::SmartPtr<HttpserverJob>& ptr = m_spHttpserverArray[p.ThreadIndex];
				return ptr->GetRequsetUrl(p.GID);
			}
		}
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string Httpserver::GetValue(uint64 id, const char* name, uint len)
	{
		if (m_pRequestSourceArray->Have(id))
		{
			RequestPair p = m_pRequestSourceArray->Get(id);
			if (hc::nullid != p.GID && hc::nullhandle != p.ThreadIndex)
			{
				hc::SmartPtr<HttpserverJob>& ptr = m_spHttpserverArray[p.ThreadIndex];
				return ptr->GetValue(p.GID, name, len);
			}
		}
		return eastl::string();
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string Httpserver::GetType(uint64 id, const char* name, uint len)
	{
		if (m_pRequestSourceArray->Have(id))
		{
			RequestPair p = m_pRequestSourceArray->Get(id);
			if (hc::nullid != p.GID && hc::nullhandle != p.ThreadIndex)
			{
				hc::SmartPtr<HttpserverJob>& ptr = m_spHttpserverArray[p.ThreadIndex];
				return ptr->GetType(p.GID, name, len);
			}
		}
		return eastl::string();
	}
	//---------------------------------------------------------------------------------------------------------
	HttpserverJob::RequestCommand Httpserver::GetCommand(uint64 id)
	{
		if (m_pRequestSourceArray->Have(id))
		{
			RequestPair p = m_pRequestSourceArray->Get(id);
			if (hc::nullid != p.GID && hc::nullhandle != p.ThreadIndex)
			{
				hc::SmartPtr<HttpserverJob>& ptr = m_spHttpserverArray[p.ThreadIndex];
				return ptr->GetCommand(p.GID);
			}
		}
		return HttpserverJob::RC_REQ_GET;
	}
}
