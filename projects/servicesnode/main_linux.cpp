#include "ServicesnodeApplication.h"


#include <EASTL/string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_PATH          260


using namespace hsna;
using namespace eastl;

void get_path(char* path)
{
	char arg1[20];
	sprintf(arg1, "/proc/%d/exe", getpid());
	readlink(arg1, path, MAX_PATH);
}


int main(int argc, char **argv)
{
	string args;
	for (int i = 1; i < argc; ++i)
	{
		if (1 != i)args.append(" ");
		args.append(argv[i]);
	}
	eastl::string exename = argv[0];
	uint b = exename.find_last_of('/') + 1;
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
	char strFilePath[MAX_PATH + 1] = { 0 };
	get_path(strFilePath);
	(strrchr(strFilePath, ('/')))[1] = 0;
	int res = chdir(strFilePath);
	string path(strFilePath);
	app.Initialize(path, args);
	app.Dowork();
	app.Exit();
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::DumpUnfreed();
#endif
	printf("Press any key to exit.\r\n");
	return 0;
};
