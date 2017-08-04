#pragma once



#define MAX_FILE_LENGTH 98
namespace hc
{
	namespace Configure
	{
		static const char* LOAD_LIBRARY_ARRAY("Librarys");
		static const char* COMMON_PATH("common/");
		static const char* FUNCTION_PATH("function/");
		static const char* DEPENDENCY_PATH("dependency/");

		//½Å±¾Ïà¹Ø
		static const char* NATIVE_LIBRARY_SYSTEM("nativesystem");


		#define FILE_PATH_FOURCC_ROOT		'root'
		#define FILE_PATH_FOURCC_DOCUMENT	'docs'
		#define FILE_PATH_FOURCC_SCRIPT		'scrs'
		#define FILE_PATH_FOURCC_COMMON		'comm'
		#define FILE_PATH_FOURCC_FUNCTION	'func'
		#define FILE_PATH_FOURCC_DEPENDENCY	'depd'

		static const char* FILE_PATH_ROOT("root:");
		static const char* FILE_PATH_DOCUMENT("docs:");
		static const char* FILE_PATH_SCRIPT("scrs:");
		static const char* FILE_PATH_COMMON_SCRIPT("comm:script/");
		static const char* FILE_PATH_FUNCTION("func:");
		static const char* FILE_PATH_COMMON("comm:");
		static const char* FILE_PATH_DEPENDENCY("depd:");

		//ÍøÂç
		#define MIN_BUFFER_SIZE 1024
		#define MAX_BUFFER_SIZE	1024 * 4
		static const char* TLS_CONFING_PATH("root:TLS.json");
		static const char* CONNECT_FUNCTION("OnConnect");
		static const char* MESSAGE_FUNCTION("OnReciveRequest");
		static const char* DISCONNECT_FUNCTION("OnDisconnect");
	}
}