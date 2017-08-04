#include "ClusternodeApplication.h"


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


int main(int argc, char *argv[])
{
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::Initialize();
#endif
	ClusternodeApplication app;
	char strFilePath[MAX_PATH + 1] = { 0 };
	get_path(strFilePath);
	(strrchr(strFilePath, ('/')))[1] = 0;
	int res = chdir(strFilePath);
	string path(strFilePath);
	app.Initialize(path);
	app.Dowork();
	app.Exit();
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::DumpUnfreed();
#endif
	printf("Press any key to exit.\r\n");
	return 0;
};