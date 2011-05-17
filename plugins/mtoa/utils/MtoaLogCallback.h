#ifndef MTOALOGCALLBACK_H
#define MTOALOGCALLBACK_H

#include "platform/Platform.h"

#include <cstring>

#include <ai_msg.h>

#include <maya/MGlobal.h>

DLLEXPORT AtVoid MtoaLogCallback(AtInt logmask, AtInt severity, const char *msg_string, AtInt tabs);

DLLEXPORT void SetupMtoaLogging();

#endif // MTOALOGCALLBACK_H
