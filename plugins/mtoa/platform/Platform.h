#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "platform/win32/Event.h"
#endif // _WIN32

#ifdef _LINUX
#include "platform/linux/Event.h"
#endif // _LINUX

#endif  // PLATFORM_H
