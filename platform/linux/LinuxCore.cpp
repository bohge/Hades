#include "LinuxPluginLoader.h"
#include "LinuxCore.h"


#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(1);
}

using namespace hc;
namespace hcl
{
	//---------------------------------------------------------------------------------------------------------
	LinuxCore::LinuxCore(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	LinuxCore::~LinuxCore(void)
	{
	}

	//---------------------------------------------------------------------------------------------------------
	IPluginLoader* LinuxCore::_CreateLoader()
	{
		signal(SIGSEGV, handler);
		return NEW LinuxPluginLoader;
	}

	//---------------------------------------------------------------------------------------------------------
	void LinuxCore::_ReleaseLoader( IPluginLoader* loder )
	{
		SAFE_DELETE( loder );
	}
	//---------------------------------------------------------------------------------------------------------
	uint LinuxCore::GetPID()
	{
		return getpid();
	}
}