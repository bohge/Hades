#include "ServicesnodeApplication.h"


#include <EASTL/string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#define MAX_PATH          260


using namespace hsna;
using namespace eastl;

void get_path(char* path)
{
	char arg1[20];
	sprintf(arg1, "/proc/%d/exe", getpid());
	readlink(arg1, path, MAX_PATH);
}

int runchiledprocess(bool cmdline, const eastl::string& exename, const eastl::string& args)
{
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
	app.Initialize(cmdline, path, args);
	app.Dowork();
	app.Exit();
#ifdef _TRACK_MEMORY
	hc::MemoryTrack::DumpUnfreed();
#endif
	return EXIT_SUCCESS;
}

void redirection()
{
	for (int i = 0; i < 3; ++i)
	{
		close(i);
		open("/dev/null", O_RDWR);
		dup(0);
		dup(0);
	}
}

int daemonmode(int childpreocess, const eastl::string& exename, const eastl::string& args)
{
	pid_t pid = fork();
	if (-1 == pid)
	{
		fprintf(stderr, "main fork error");
	}
	else if (0 == pid)
	{
		if (setsid() == -1)
		{
			fprintf(stderr, "setsid error");
		}
		else
		{
			if (1 == childpreocess)
			{
				redirection();
				return runchiledprocess(true, exename, args);
			}
			else
			{
				for (int i = 0; i < childpreocess; ++i)
				{
					if (0 == fork())
					{
						redirection();
						return runchiledprocess(true, exename, args);
					}
				}
				int status = 0;
				wait(&status);
			}
		}
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	int childpreocess = 0;
	eastl::string args;
	for (int i = 1; i < argc; ++i)
	{
		if (strstr(argv[i], "daemon"))
		{
			char* cmd = argv[i];
			cmd = cmd + strlen("daemon=");
			childpreocess = atoi(cmd);
		}
		else
		{
			if (1 != i)args.append(" ");
			args.append(argv[i]);
		}
	}
	eastl::string exename = argv[0];
	uint b = exename.find_last_of('/') + 1;
	uint e = exename.find_first_of('.', b);
	exename = exename.substr(b, e - b);
#ifdef _DEBUG
	Utility::RemoveTargetString(exename, "_d");
#endif
	if (0 == childpreocess)
	{
		printf("run as single mode\r\n");
		return runchiledprocess(false, exename, args);
	}
	else
	{
		printf("run as daemon mode with chile process count %d\r\n", childpreocess);
		return daemonmode(childpreocess, exename, args);
	}
	return EXIT_SUCCESS;
};
