#include "ServicesnodeApplication.h"


#include <EASTL/string.h>
#include <Windows.h>
#include <conio.h>


using namespace hsna;
using namespace eastl;
void main(int argc, char *argv[])
{
	string args;
	for (int i = 1; i < argc; ++i)
	{
		if (1 != i)args.append(" ");
		args.append(argv[i]);
	}
	string exename = argv[0];
	uint b = exename.find_last_of('/\\') + 1;
	uint e = exename.find_first_of('.', b);
	exename = exename.substr(b, e - b);
#ifdef _DEBUG
	Utility::RemoveTargetString(exename, "_d");
#endif
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::Initialize();
#endif
	ServicesnodeApplication app;
	app.SetExecutableName(exename);
	char strFilePath[MAX_PATH + 1]={0};
	GetModuleFileNameA(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectoryA( strFilePath );
	string path(strFilePath);
	app.Initialize(path, args);
	app.Dowork();
	app.Exit();
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::DumpUnfreed();
#endif	
	printf( "Press any key to exit.\r\n" );
	getche();
};