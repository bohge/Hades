#pragma once


namespace hmka
{
	namespace Configure
	{
		//配置表相关
		static const char* CONFIG_FILE_PATH( "root:Monkey_win.json" );
		static const char* CONNECT_THREAD_COUNT("Connectthreadcount");
		static const char* WORK_THREAD_COUNT("Workthreadcount");
		static const char* MONKEY_COUNT("Monkeycount");
		static const char* DOCUMENT_PATH("Documentspath");
		static const char* SCRIPT_PATH("Scriptpath");
		static const char* USEING_TLS("TLSFlag");
		static const char* LOOP_FREQUNCY("Loopfrequncy");
		static const char* LOG_FILE_NAME("Logfile");
		static const char* LOG_FILE_SIZE("Logsize");

		//脚本相关
		static const char* GLOBAL_APPLICATION("g_Application");
		static const char* GLOBAL_MACHINE("g_Machine");
		static const char* MONKEY_HANDLE("Monkey");
		static const char* GLOBAL_SOCKET("g_Socket");
		static const char* GLOBAL_MONKEY_HANDLE("monkeyhandle");
		static const char* INITIALIZE_FUNCITON("Initialize");
		static const char* LOOP_FUNCITON("loop");
	};
}