#include "ClusternodeApplication.h"


#include <EASTL/string.h>
#include <Windows.h>
#include <conio.h>


using namespace hsna;
using namespace eastl;
void main( void )
{
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::Initialize();
#endif
	ClusternodeApplication app;
	char strFilePath[MAX_PATH + 1]={0};
	GetModuleFileNameA(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectoryA( strFilePath );
	string path(strFilePath);
	app.Initialize(path);
	app.Dowork();
	app.Exit();
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::DumpUnfreed();
#endif	
	printf( "Press any key to exit.\r\n" );
	getche();
};