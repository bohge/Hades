#include "MonkeyApplication.h"


#include <EASTL/string.h>
#include <Windows.h>
#include <conio.h>



using namespace hmka;
using namespace eastl;
void main( void )
{
	MonkeyApplication app;
	char strFilePath[MAX_PATH + 1]={0};
	GetModuleFileNameA(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectoryA( strFilePath );
	string path(strFilePath);
	app.Initialize(path);
	app.Dowork();
	app.Exit();
	printf( "Press any key to exit.\r\n" );
	getche();
};