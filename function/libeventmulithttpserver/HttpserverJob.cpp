#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "script/IArguments.h"
#include "file/IFileSystem.h"
#include "core/Configure.h"
#include "log/ILogSystem.h"
#include "HttpserverJob.h"
#include "Exception.h"
#include "Parser.h"
#include "Field.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <winsock2.h>
//#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif
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
#ifdef _WIN32
#ifndef stat
#define stat _stat
#endif
#ifndef fstat
#define fstat _fstat
#endif
#ifndef open
#define open _open
#endif
#ifndef close
#define close _close
#endif
#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif
#endif


//MPFD::Parser没有清空
typedef std::map<std::string, MPFD::Field *> FieldMap;
using namespace hc;
using namespace eastl;
namespace hlhsp
{
	static const struct table_entry {
		const char *extension;
		const char *content_type;
	} content_type_table[] = {
		{ "txt", "text/plain" },
		{ "c", "text/plain" },
		{ "h", "text/plain" },
		{ "html", "text/html" },
		{ "htm", "text/htm" },
		{ "css", "text/css" },
		{ "gif", "image/gif" },
		{ "jpg", "image/jpeg" },
		{ "jpeg", "image/jpeg" },
		{ "png", "image/png" },
		{ "pdf", "application/pdf" },
		{ "ps", "application/postscript" },
		{ NULL, NULL },
	};

	/* Try to guess a good content-type for 'path' */
	static const char * guess_content_type(const char *path)
	{
		const char *last_period, *extension;
		const struct table_entry *ent;
		last_period = strrchr(path, '.');
		if (!last_period || strchr(last_period, '/'))
			goto not_found; /* no exension */
		extension = last_period + 1;
		for (ent = &content_type_table[0]; ent->extension; ++ent) {
			if (!evutil_ascii_strcasecmp(ent->extension, extension))
				return ent->content_type;
		}
	not_found:
		return "application/misc";
	}
	static void server_setup_certs(SSL_CTX *ctx,
		const char *certificate_chain,
		const char *private_key)
	{
		HADESLOG("Loading certificate chain from '%s'", certificate_chain);
		HADESLOG("Loading private key from '%s'", private_key);

		if (1 != SSL_CTX_use_certificate_chain_file(ctx, certificate_chain))
			HADESERROR("SSL_CTX_use_certificate_chain_file");

		if (1 != SSL_CTX_use_PrivateKey_file(ctx, private_key, SSL_FILETYPE_PEM))
			HADESERROR("SSL_CTX_use_PrivateKey_file");

		if (1 != SSL_CTX_check_private_key(ctx))
			HADESERROR("SSL_CTX_check_private_key");
	}
	//---------------------------------------------------------------------------------------------------------
	static struct bufferevent* bevcb(struct event_base *base, void *arg)
	{
		struct bufferevent* r;
		SSL_CTX *ctx = (SSL_CTX *)arg;

		r = bufferevent_openssl_socket_new(base,
			-1,
			SSL_new(ctx),
			BUFFEREVENT_SSL_ACCEPTING,
			BEV_OPT_CLOSE_ON_FREE);
		return r;
	}
	//---------------------------------------------------------------------------------------------------------
	static void generic_call_back(struct evhttp_request *req, void *arg)
	{
		HttpserverJob* httpserver = static_cast<HttpserverJob*>(arg);
		evhttp_cmd_type cmd_type = evhttp_request_get_command(req);
		switch (cmd_type)
		{
			//case EVHTTP_REQ_POST:httpserver->OnRequest(req); break;
			//case EVHTTP_REQ_GET:
			//{
			//					   if (!httpserver->DefualtGet(req))
			//					   {
			//						   httpserver->OnRequest(req);
			//					   }
			//					   break;
			//}
		case EVHTTP_REQ_GET:
		case EVHTTP_REQ_POST:httpserver->OnRequest(req); break;
		default:httpserver->OnError(req, 403, "Forbidden"); break;
		}
		httpserver->ExecuteReplyQueue();
	}
	static void autoread_call_back(struct evhttp_request *req, void *arg)
	{
		HttpserverJob* httpserver = static_cast<HttpserverJob*>(arg);
		evhttp_cmd_type cmd_type = evhttp_request_get_command(req);
		switch (cmd_type)
		{
		case EVHTTP_REQ_POST:httpserver->OnRequest(req); break;
		case EVHTTP_REQ_GET:
		{
							   if (!httpserver->DefualtGet(req))
							   {
								   httpserver->OnRequest(req);
							   }
							   break;
		}
		default:httpserver->OnError(req, 403, "Forbidden"); break;
		}
		httpserver->ExecuteReplyQueue();
	}
	//---------------------------------------------------------------------------------------------------------
	static void sendqueue_func(evutil_socket_t fd, short what, void *arg)
	{
		HttpserverJob* httpserver = static_cast<HttpserverJob*>(arg);
		httpserver->ExecuteReplyQueue();
	}


	HttpserverJob::Httprequest::Httprequest() :req(NULL), evb(NULL), parser(NULL){}
	HttpserverJob::Httprequest::Httprequest(int) : req(NULL), evb(NULL), parser(NULL){}
	//---------------------------------------------------------------------------------------------------------
	HttpserverJob::HttpserverJob(uint index, uint post, const eastl::string& root, const eastl::string& temp)
		: m_pHttp(NULL)
		, m_pBase(NULL)
		, m_RootDir(root)
		, m_TempDir(temp)
		, m_PostSize(post)
		, m_ThreadIndex(index)
		, m_pInfinityEvent(NEW event)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	HttpserverJob::~HttpserverJob()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void HttpserverJob::_DoYieldJob()
	{
		event_base_loopexit(m_pBase, NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void HttpserverJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		event_base_dispatch(m_pBase);
		event_base_free(m_pBase);
		evhttp_free(m_pHttp);
		struct evbuffer* evb;
		while (m_EvbufferPool.try_dequeue(evb))
		{
			evbuffer_free(evb);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool HttpserverJob::DefualtGet(struct evhttp_request* req)
	{
		char whole_path[MAX_BUFFER_SIZE];
		const char *uri = evhttp_request_get_uri(req);
		struct evhttp_uri *decoded = evhttp_uri_parse(uri);
		const char* path = evhttp_uri_get_path(decoded);
		if (!path) path = "/";
		const char* decoded_path = evhttp_uridecode(path, 0, NULL);
		if (decoded_path != NULL)
		{
			if (!strstr(decoded_path, ".."))
			{
				struct stat st;
				sprintf(whole_path, "%s/%s", m_RootDir.c_str(), decoded_path);
				if (stat(whole_path, &st) < 0)
				{
					//不存在转向默认的接收文件
					evhttp_uri_free(decoded);
					return false;
				}
				else
				{
					int fd = -1;
					if ((fd = open(whole_path, O_RDONLY)) > 0)
					{
						const char *type = guess_content_type(decoded_path);
						evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", type);
						evbuffer* evb = _GetEventbuffer();
						evbuffer_add_file(evb, fd, 0, st.st_size);
						evhttp_send_reply(req, 200, "OK", evb);
						m_EvbufferPool.enqueue(evb);
						evhttp_uri_free(decoded);
						return true;
					}
				}
			}
		}
		OnError(req, 403, "Forbidden");
		evhttp_uri_free(decoded);
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	struct evbuffer* HttpserverJob::_GetEventbuffer()
	{
		struct evbuffer* evb;
		if (!m_EvbufferPool.try_dequeue(evb))
		{
			evb = evbuffer_new();
		}
		return evb;
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 HttpserverJob::_InsertRequest(struct evhttp_request* req)
	{
		Httprequest reqpair;
		reqpair.req = req;
		reqpair.evb = _GetEventbuffer();
		return m_RequestPool.Push(reqpair);
	}
	//---------------------------------------------------------------------------------------------------------
	MPFD::Parser* HttpserverJob::_GetParser()
	{
		MPFD::Parser* parser;
		if (!m_ParserPool.try_dequeue(parser))
		{
			parser = NEW MPFD::Parser;
			parser->SetTempDirForFileUpload(m_TempDir.c_str());
			parser->SetMaxCollectedDataLength(m_PostSize);
		}
		return parser;
	}
	//---------------------------------------------------------------------------------------------------------
	void HttpserverJob::_FreeParser(MPFD::Parser* p)
	{
		m_ParserPool.enqueue(p);
		//SAFE_DELETE(p);
	}
	//---------------------------------------------------------------------------------------------------------
	void HttpserverJob::OnError(struct evhttp_request* req, uint code, const char* msg)
	{
		evhttp_send_error(req, code, msg);
	}
	//---------------------------------------------------------------------------------------------------------
	void HttpserverJob::OnRequest(struct evhttp_request* req)
	{
		uint64 uid = _InsertRequest(req);
		m_RequestCallback.Unicast(m_ThreadIndex, uid);
	}
	//---------------------------------------------------------------------------------------------------------
	void HttpserverJob::Header(uint64 id, const char* key, const char* value)
	{
		if (m_RequestPool.Have(id))
		{
			Httprequest reqpair = m_RequestPool.Get(id);
			if (reqpair.req && reqpair.evb)
			{
				evhttp_add_header(evhttp_request_get_output_headers(reqpair.req), key, value);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool HttpserverJob::Reply(uint64 id, uint code, const char* buffer, uint len)
	{
		if (m_RequestPool.Have(id))
		{
			Httprequest reqpair = m_RequestPool.Get(id);
			m_RequestPool.Remove(id);
			if (reqpair.req && reqpair.evb)
			{
				reqpair.req->response_code = code;
				evbuffer_add(reqpair.evb, buffer, len);
				m_ReplyPool.enqueue(reqpair);

				//处理Parser
				if (reqpair.parser)
				{
					FieldMap fieldmap = reqpair.parser->GetFieldsMap();
					FOR_EACH(FieldMap, field, fieldmap)
					{
						if (MPFD::Field::FileType == field->second->GetType())
						{
							remove(field->second->GetTempFilePath().c_str());
						}
					}
					_FreeParser(reqpair.parser);
				}
				return true;
			}
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void HttpserverJob::ExecuteReplyQueue()
	{
		Httprequest reqpair;
		while (m_ReplyPool.try_dequeue(reqpair))
		{
			evhttp_send_reply(reqpair.req, reqpair.req->response_code, "OK", reqpair.evb);
			m_EvbufferPool.enqueue(reqpair.evb);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool hlhsp::HttpserverJob::_ParsePost(Httprequest& request)
	{
		if (NULL == request.parser)
		{
			MPFD::Parser* parser = _GetParser();
			struct evkeyvalq * h = evhttp_request_get_input_headers(request.req);
			const char* content_type = evhttp_find_header(h, "Content-Type");
			char *input = (char *)evbuffer_pullup(request.req->input_buffer, -1);
			int buffer_data_len = evbuffer_get_length(request.req->input_buffer);

			if (NULL == content_type
				|| NULL == input
				|| 0 == buffer_data_len)
			{
				m_ReplyPool.enqueue(request);
				return false;
			}
			else
			{
				try {
					parser->SetContentType(content_type);
					parser->AcceptSomeData(input, buffer_data_len);
					request.parser = parser;
				}
				catch (MPFD::Exception e)
				{
					HADESERROR("Parse post fail, error message:%s", e.GetError().c_str());
					_FreeParser(parser);
					if (request.req && request.evb)
					{
						request.req->response_code = 500;
						evbuffer_add(request.evb, "Server error...", strlen("Server error..."));
						m_ReplyPool.enqueue(request);
					}
					return false;
				}
			}
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string HttpserverJob::GetValue(uint64 id, const char* name, uint len)
	{
		eastl::string res;
		if (m_RequestPool.Have(id))
		{
			Httprequest& reqpair = m_RequestPool.Get(id);
			if (_ParsePost(reqpair))
			{
				MPFD::Field* field = reqpair.parser->GetField(std::string(name, len));
				if (field)
				{
					switch (field->GetType())
					{
					case MPFD::Field::TextType: res = field->GetTextTypeContent().c_str(); break;
					case MPFD::Field::FileType: res = field->GetTempFilePath().c_str(); break;
					}
				}
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string HttpserverJob::GetType(uint64 id, const char* name, uint len)
	{
		eastl::string res;
		if (m_RequestPool.Have(id))
		{
			Httprequest& reqpair = m_RequestPool.Get(id);
			if (_ParsePost(reqpair))
			{
				MPFD::Field* field = reqpair.parser->GetField(std::string(name, len));
				if (field)
				{
					switch (field->GetType())
					{
					case MPFD::Field::TextType: res = "text"; break;
					case MPFD::Field::FileType: res = field->GetFileMimeType().c_str(); break;
					}
				}
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool HttpserverJob::Initialize(uint fd, HttpserverJob::ServerType st, bool isauto, const eastl::string& crt, const eastl::string&key)
	{
		//evthread_use_depend_threads();
		m_RequestPool.Initialize(HC_PERPARE_EVBUFFER);

		for (size_t i = 0; i < HC_PERPARE_EVBUFFER; i++)
		{
			m_EvbufferPool.enqueue(evbuffer_new());
		}

		m_pBase = event_base_new();
		if (!m_pBase) {
			HADESERROR("Couldn't create an event_base: exiting\n");
		}

		/* Create a new evhttp object to handle requests. */
		m_pHttp = evhttp_new(m_pBase);
		if (!m_pHttp) {
			HADESERROR("couldn't create evhttp. Exiting.\n");
		}

		if (ST_HTTPS == st)
		{
			m_pCTX = SSL_CTX_new(SSLv23_server_method());
			SSL_CTX_set_options(m_pCTX,
				SSL_OP_SINGLE_DH_USE |
				SSL_OP_SINGLE_ECDH_USE |
				SSL_OP_NO_SSLv2);

			/* Cheesily pick an elliptic curve to use with elliptic curve ciphersuites.
			* We just hardcode a single curve which is reasonably decent.
			* See http://www.mail-archive.com/openssl-dev@openssl.org/msg30957.html */
			EC_KEY *ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
			if (!ecdh)
				HADESERROR("EC_KEY_new_by_curve_name");
			if (1 != SSL_CTX_set_tmp_ecdh(m_pCTX, ecdh))
				HADESERROR("SSL_CTX_set_tmp_ecdh");

			/* Find and set up our server certificate. */
			server_setup_certs(m_pCTX, crt.c_str(), key.c_str());

			/* This is the magic that lets evhttp use SSL. */
			evhttp_set_bevcb(m_pHttp, bevcb, m_pCTX);
		}

		//evhttp_accept_socket_with_handle(m_pHttp, fd);
		if (-1 == evhttp_accept_socket(m_pHttp, fd))
		{
			HADESERROR("fail to accept socket");
			return false;
		}

		evhttp_set_max_body_size(m_pHttp, m_PostSize);
		//evhttp_set_max_headers_size(m_pHttp, 1024 * 1024);

		/* We want to accept arbitrary requests, so we need to set a "generic"
		* cb.  We can also add callbacks for specific paths. */
		if (isauto)
		{
			evhttp_set_gencb(m_pHttp, autoread_call_back, this);
		}
		else
		{
			evhttp_set_gencb(m_pHttp, generic_call_back, this);
		}

		struct timeval tv;
		evutil_timerclear(&tv);
		tv.tv_sec = 0;
		tv.tv_usec = 100000;//每100毫秒check一次堆栈防止有没有发出去的数据
		event_assign(m_pInfinityEvent, m_pBase, -1, EV_PERSIST, sendqueue_func, this);
		event_add(m_pInfinityEvent, &tv);
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* HttpserverJob::GetRequsetUrl(uint64 id)
	{
		const char* res = NULL;
		if (m_RequestPool.Have(id))
		{
			Httprequest& reqpair = m_RequestPool.Get(id);
			if (reqpair.req && reqpair.evb)
			{
				res = evhttp_request_get_uri(reqpair.req);
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* HttpserverJob::GetAddress(uint64 id)
	{
		char* res = NULL;
		if (m_RequestPool.Have(id))
		{
			Httprequest& reqpair = m_RequestPool.Get(id);
			if (reqpair.req && reqpair.evb)
			{
				ushort client_port;
				evhttp_connection_get_peer(evhttp_request_get_connection(reqpair.req), &res, &client_port);
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	HttpserverJob::RequestCommand HttpserverJob::GetCommand(uint64 id)
	{
		RequestCommand res(RC_REQ_GET);
		if (m_RequestPool.Have(id))
		{
			Httprequest& reqpair = m_RequestPool.Get(id);
			if (reqpair.req && reqpair.evb)
			{
				res = (RequestCommand)evhttp_request_get_command(reqpair.req);
			}
		}
		return res;
	}
}