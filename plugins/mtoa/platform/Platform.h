#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define DLLEXPORT __declspec(dllexport)
#include "platform/win32/Event.h"
#endif // _WIN32

#ifdef _LINUX
#define DLLEXPORT __attribute__ ((visibility("default")))
#include "platform/linux/Event.h"
#endif // _LINUX

#ifdef _DARWIN
#define DLLEXPORT __attribute__ ((visibility("default")))
#include "platform/darwin/Event.h"
#endif // _DARWIN

#endif  // PLATFORM_H
