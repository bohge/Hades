#include "log/ILogSystem.h"

extern "C" void ScriptOutPutString( const char* str, int l )
{	
	HADESLOG( "%s", str );
}
extern "C" void ScriptOutPutLine()
{
	HADESLOG("\n");
}
extern "C" void ScriptOutPutError( const char* str1, const char* str2 )
{
	HADESLOG( str1, str2 );
}