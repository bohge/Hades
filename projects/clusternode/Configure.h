#pragma once


namespace hsna
{
	namespace Configure
	{
		#define RESUME_BOOL 0
		#define RESUME_ACESSOR 1
		//配置表相关
#ifdef _PLATFORM_WINDOWS
		static const char* CONFIG_FILE_PATH( "root:Clusternode_win.json" );
#else
		static const char* CONFIG_FILE_PATH("root:Clusternode_linux.json");
#endif
		static const char* LOCAL_PORT( "Localport" );
		static const char* CONNECT_THREAD_COUNT("Connectthreadcount");
		static const char* WORK_THREAD_COUNT( "Workthreadcount" );
		static const char* DOCUMENT_PATH("Documentspath");
		static const char* SCRIPT_PATH("Scriptpath");
		static const char* USEING_TLS("TLSFlag");
		static const char* RPC_OPTIMIZATION("RPCOptimization");
		static const char* DEBUG_FLAG("Debug");
		static const char* SCRIPT_MAIN("Scriptmain");
		static const char* SERVICES_CACHE_SIZE("Servicecachesize");
		static const char* LOG_FILE_NAME("Logfile");

		//脚本相关
		static const char* GLOBAL_APPLICATION("g_Application");
		static const char* GLOBAL_SERVER("g_Server");
		static const char* GLOBAL_MACHINE("g_Machine");
		static const char* GLOBAL_CLIENTID("g_ClientID");
		static const char* GLOBAL_REQUEST("g_Request");
		static const char* GLOBAL_MESSAGE("g_Buffer");
		static const char* INITALIZE_FUNCTION("Initialize");
		static const char* CONNECT_FUNCTION("OnConnect");
		static const char* MESSAGE_FUNCTION("OnReciveRequest");
		static const char* DISCONNECT_FUNCTION("OnDisconnect");
	};
}