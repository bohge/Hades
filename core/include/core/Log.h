#pragma once

//#ifdef _DEBUG
#define  PRINTLOG
//#endif

#ifdef PRINTLOG
	#ifdef WIN32
		#include <stdio.h>
		#define DEBUGLOG(x,...) printf(x, ##__VA_ARGS__)
		#define TODO(x) message(x);
	#else
		#include <stdio.h>
		#define DEBUGLOG(x,...) printf(x, ##__VA_ARGS__)
		#define TODO(x)
	#endif
#else
	#define DEBUGLOG(x,...)
	#define TODO(x)
#endif // PRINTLOG