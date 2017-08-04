/* -*- C -*- */

#include "lua.h"
#include "lauxlib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#if defined(_WIN32) || defined(LUA_WIN)
# ifdef paths_EXPORTS
#  define PATHS_API __declspec(dllexport)
# else
#  define PATHS_API __declspec(dllimport)
# endif
#else
# define PATHS_API /**/
#endif


#if defined(_WIN32) || defined(LUA_WIN)

# include <errno.h>
# include <windows.h>
# include <direct.h>
# include <io.h>
# include <time.h>
# include <process.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>

#else
#define HAVE_DIRENT_H 1
#define HAVE_FCNTL_H 1
#define HAVE_UNISTD_H 1
#define HAVE_TIME_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_NDIR_H 1
#define HAVE_SYS_DIR_H 1
#define HAVE_SYS_UTSNAME_H 1
#define HAVE_NDIR_H 1
#define HAVE_GETCWD 1
#define HAVE_DLOPEN 1
/* #undef HAVE_DIRENT_H */
/* #undef HAVE_FCNTL_H */
/* #undef HAVE_UNISTD_H */
/* #undef HAVE_TIME_H */
/* #undef HAVE_SYS_TIME_H */
/* #undef HAVE_SYS_NDIR_H */
/* #undef HAVE_SYS_DIR_H */
/* #undef HAVE_SYS_UTSNAME_H */
/* #undef HAVE_NDIR_H */
/* #undef HAVE_GETCWD */
/* #undef HAVE_DLOPEN */

# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# if HAVE_FCNTL_H
#  include <fcntl.h>
# endif
# if HAVE_UNISTD_H
#  include <unistd.h>
# endif
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# endif
# if HAVE_SYS_UTSNAME_H
#  include <sys/utsname.h>
# endif
# if HAVE_TIME_H
#  include <time.h>
# endif
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# endif
# ifdef HAVE_DIRENT_H
#  include <dirent.h>
#  define NAMLEN(dirent) strlen((dirent)->d_name)
# else
#  define dirent direct
#  define NAMLEN(dirent) (dirent)->d_namlen
#  if HAVE_SYS_NDIR_H
#   include <sys/ndir.h>
#  endif
#  if HAVE_SYS_DIR_H
#   include <sys/dir.h>
#  endif
#  if HAVE_NDIR_H
#   include <ndir.h>
#  endif
# endif

#endif



