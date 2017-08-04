#pragma once
#include "core/Recycleablevector.hpp"
#include "core/Delegate.hpp"
#include "thread/IJob.h"
#include "Parser.h"



struct event;
struct evhttp;
struct evbuffer;
struct event_base;
struct ssl_ctx_st;
struct evhttp_request;
namespace hlhsp
{
	typedef hc::Unidelegate2< void, uint, uint64 > RequestCallback;
	class HttpserverJob : public hc::IJob
	{
	public:
		enum ServerType
		{
			ST_HTTP = 0,
			ST_HTTPS = 1,
		};
	private:
		enum Constant
		{
			HC_PERPARE_EVBUFFER = 1000,
		};
	public:
		enum RequestCommand
		{
			RC_REQ_GET		= 1 << 0,
			RC_REQ_POST		= 1 << 1,
			RC_REQ_HEAD		= 1 << 2,
			RC_REQ_PUT		= 1 << 3,
			RC_REQ_DELETE	= 1 << 4,
			RC_REQ_OPTIONS	= 1 << 5,
			RC_REQ_TRACE	= 1 << 6,
			RC_REQ_CONNECT	= 1 << 7,
			RC_REQ_PATCH	= 1 << 8
		};
	private:
		struct Httprequest
		{
			struct evhttp_request* req;
			struct evbuffer* evb;
			MPFD::Parser* parser;
			Httprequest();
			Httprequest(int);
		};
	private:
		//从代码角度看多线程操作不同的evbuffer是安全的，所以我们用这个作为接受数据的buffer减少copy次数
		typedef moodycamel::ConcurrentQueue< struct evbuffer* > EvbufferPool;
		typedef hc::Recycleablevector< Httprequest > RequestPool;
		typedef moodycamel::ConcurrentQueue< Httprequest > ReplyPool;
		typedef moodycamel::ConcurrentQueue< MPFD::Parser* > ParserPool;
	private:
		RequestCallback				m_RequestCallback;
		RequestPool					m_RequestPool;
		EvbufferPool				m_EvbufferPool;
		ReplyPool					m_ReplyPool;
		ParserPool					m_ParserPool;
		const eastl::string			m_RootDir;
		const eastl::string			m_TempDir;
		struct evhttp*				m_pHttp;
		struct event_base*			m_pBase;
		struct event*				m_pInfinityEvent;
		ssl_ctx_st*					m_pCTX;
		const uint					m_PostSize;
		const uint					m_ThreadIndex;
	public:
		HttpserverJob(uint index, uint post, const eastl::string& root, const eastl::string& temp);
		virtual ~HttpserverJob();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	private:
		struct evbuffer* _GetEventbuffer();
		uint64 _InsertRequest(struct evhttp_request* req);
		MPFD::Parser* _GetParser();
		void _FreeParser(MPFD::Parser* p);
		bool _ParsePost(Httprequest& request);
	public:
		bool Initialize(uint fd, ServerType st, bool isauto, const eastl::string& crt, const eastl::string&key);
		bool DefualtGet(struct evhttp_request* req);
		void OnRequest(struct evhttp_request* req);
		void OnError(struct evhttp_request* req, uint code, const char* msg);
		bool Reply(uint64 id, uint code, const char* buffer, uint len);
		void Header(uint64 id, const char* key, const char* value);
		void ExecuteReplyQueue();
		const char* GetRequsetUrl(uint64 id);
		const char* GetAddress(uint64 id);
		eastl::string GetValue(uint64 id, const char* name, uint len);
		eastl::string GetType(uint64 id, const char* name, uint len);
		RequestCommand GetCommand(uint64 id);
	public:
		HADES_FORCEINLINE void SetRequestCallback(RequestCallback::Callback c);
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void HttpserverJob::SetRequestCallback(RequestCallback::Callback c)
	{
		m_RequestCallback.Add(c);
	}
}
