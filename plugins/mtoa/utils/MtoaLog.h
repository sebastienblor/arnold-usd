#ifndef MTOALOG_H
#define MTOALOG_H

#include "platform/Platform.h"

#include <stdio.h>
#include <stdarg.h>
#include <cstring>

#include <ai_msg.h>


DLLEXPORT int GetFlagsFromVerbosityLevel(unsigned int level);

DLLEXPORT void MtoaLogCallback(int logmask, int severity, const char *msg_string, int tabs);

DLLEXPORT void MtoaSetupLogging();

void MtoaSetupSwatchLogging();

#endif // MTOALOGCALLBACK_H
