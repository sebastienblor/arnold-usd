#ifndef MTOALOG_H
#define MTOALOG_H

#include "platform/Platform.h"

#include <stdio.h>
#include <stdarg.h>
#include <cstring>

#include <ai_msg.h>


DLLEXPORT AtInt GetFlagsFromVerbosityLevel(AtUInt level);

DLLEXPORT AtVoid MtoaLogCallback(AtInt logmask, AtInt severity, const char *msg_string, AtInt tabs);

DLLEXPORT void MtoaSetupLogging();

void MtoaSetupSwatchLogging();

#endif // MTOALOGCALLBACK_H
