#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define DLLEXPORT __declspec(dllexport)
#include "platform/win32/Event.h"
#include <platform/win32/dirent.h>
#define PATHSEP ';'
#define DIRSEP "/"
#define LIBEXT ".dll"
#ifdef _MSC_VER
#pragma warning(disable:4251)
#pragma warning(disable:4267)
#endif // _MSC_VER
#else
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>

#define PATHSEP ':'
#define DIRSEP "/"

#ifdef _LINUX
#define DLLEXPORT __attribute__ ((visibility("default")))
#include "platform/linux/Event.h"
#define LIBEXT ".so"
#endif // _LINUX

#ifdef _DARWIN
#define DLLEXPORT __attribute__ ((visibility("default")))
#include "platform/darwin/Event.h"
#define LIBEXT ".dylib"
#endif // _DARWIN
#endif // _WIN32

#endif  // PLATFORM_H
