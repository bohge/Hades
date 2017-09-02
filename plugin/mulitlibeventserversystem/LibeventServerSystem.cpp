#include "configure/IConfigSystem.h"
#include "memory/IMemorySystem.h"
#include "LibeventServerSystem.h"
#include "configure/IConfigure.h"
#include "LibeventConnection.h"
#include "file/IFileSystem.h"
#include "LibeventServer.h"
#include "log/ILogSystem.h"
#include "core/Configure.h"
#include "Connectionpool.h"
#include "ConnectionJob.h"


#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <openssl/ssl.h>
#include <signal.h>

using namespace hc;
using namespace eastl;

namespace hles
{
	SSL_CTX* init_ssl_ctx(const string& certfile, const string& keyfile, const string& cafile)
	{
		/* Load encryption & hashing algorithms for the SSL program */
		SSL_library_init();

		/* Load the error strings for SSL & CRYPTO APIs */
		SSL_load_error_strings();

		/* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
		const SSL_METHOD *meth = SSLv23_server_method();

		/* Create an SSL_CTX structure */
		SSL_CTX *ctx = SSL_CTX_new(meth);
		if (NULL == ctx)
		{
			HADESERROR("Could not new SSL_CTX\n");
			return NULL;
		}

		/* Load the CA cert file*/
		if (SSL_CTX_load_verify_locations(ctx, cafile.c_str(), NULL) <= 0)
		{
			HADESERROR("Could not load ca cert file\n");
		}

		/* Load the server certificate into the SSL_CTX structure */
		if (SSL_CTX_use_certificate_file(ctx, certfile.c_str(), SSL_FILETYPE_PEM) <= 0)
		{
			HADESERROR("Could not use certificate file\n");
		}

		/* Load the private-key corresponding to the client certificate */
		if (SSL_CTX_use_PrivateKey_file(ctx, keyfile.c_str(), SSL_FILETYPE_PEM) <= 0)
		{
			HADESERROR("Could not use private key file\n");
		}

		/* Check if the client certificate and private-key matches */
		if (!SSL_CTX_check_private_key(ctx))
		{
			HADESERROR("Private key does not match certfile\n");
		}

		return ctx;
	}
	void event_log_cb(int severity, const char *msg)
	{
		if (msg)
		{
			switch (severity) {
			case _EVENT_LOG_DEBUG: HADESLOG("DEBUG: %s", msg); break;
			case _EVENT_LOG_MSG:   HADESLOG("MSG: %s", msg);   break;
			case _EVENT_LOG_WARN:  HADESWARNING("%s", msg);  break;
			case _EVENT_LOG_ERR:   HADESERROR("%s", msg); break;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventServerSystem::LibeventServerSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventServerSystem::~LibeventServerSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IServer* LibeventServerSystem::_DoServerFactory(IServerSystem::ServerType st) const
	{
		IServer* res = NULL;
		switch (st)
		{
		case hc::IServerSystem::ST_COMMON_TL: res = NEW LibeventServer(st); break;
		case hc::IServerSystem::ST_OPENSSL_TLS: res = NEW LibeventServer(st); break;
		default:ASSERT(false); break;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServerSystem::_DoRecycleBin(hc::IServer* is) const
	{
		SAFE_DELETE(is);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServerSystem::Initialize()
	{
		Connectionpool::Create();
		IServerSystem::Initialize();
		//event_enable_debug_mode();
		//event_set_log_callback(event_log_cb);
		evthread_use_depend_threads();
		event_set_mem_functions(
			&IMemorySystem::AllocMemory,
			&IMemorySystem::ReallocMemory,
			&IMemorySystem::ReleaseMemory
			);
#ifdef WIN32
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);
#else
		signal(SIGPIPE, SIG_IGN);
#endif
		IConfigure* conf = IConfigSystem::Instance()->ConfigureFactory(Configure::TLS_CONFING_PATH);
		string crt = conf->GetString("OPENSSL_CRT");
		string key = conf->GetString("OPENSSL_KEY");
		string ca = conf->GetString("OPENSSL_CA");
		crt = IFileSystem::Instance()->PathAssembly(crt);
		key = IFileSystem::Instance()->PathAssembly(key);
		ca = IFileSystem::Instance()->PathAssembly(ca);
		IConfigSystem::Instance()->RecycleBin(conf);
		m_Openssl = init_ssl_ctx(crt, key, ca);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServerSystem::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServerSystem::Exit()
	{
		IServerSystem::Exit();
		Connectionpool::Destroy();
	}
}