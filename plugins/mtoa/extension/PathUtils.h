#ifndef PATHUTILS_H
#define PATHUTILS_H

#ifdef _WIN32
#include <platform/win32/dirent.h>
#define PATHSEP ';'
#define DIRSEP "/"
#define LIBEXT MString(".dll")
#else
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#define PATHSEP ':'
#define DIRSEP "/"
#ifdef _LINUX
#define LIBEXT MString(".so")
#endif
#ifdef _DARWIN
#define LIBEXT MString(".dylib")
#endif
#endif

#endif // PATHUTILS_H
