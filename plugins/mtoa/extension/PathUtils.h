#ifndef PATHUTILS_H
#define PATHUTILS_H

#define PLUGIN_SEARCH "$ARNOLD_PLUGIN_PATH"
#define EXTENSION_SEARCH "$MTOA_EXTENSIONS_PATH"

#ifdef _WIN32
#include <platform/win32/dirent.h>
#define PATHSEP ';'
#define DIRSEP "/"
#define LIBEXT ".dll"
#else
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#define PATHSEP ':'
#define DIRSEP "/"
#ifdef _LINUX
#define LIBEXT ".so"
#endif
#ifdef _DARWIN
#define LIBEXT ".dylib"
#endif
#endif

#endif // PATHUTILS_H
