#include "MtoaLog.h"

#include <maya/MGlobal.h>

int GetFlagsFromVerbosityLevel(AtUInt level)
{
   int flags = 0;

   switch(level)
   {
   case 6:  flags = AI_LOG_ALL; break;
   case 5:  flags = AI_LOG_ALL & ~AI_LOG_DEBUG; break;
   case 4:  flags |= AI_LOG_PROGRESS | AI_LOG_PLUGINS | AI_LOG_STATS;
   case 3:  flags |= AI_LOG_INFO;
   case 2:  flags |= AI_LOG_WARNINGS;
   case 1:  flags |= AI_LOG_ERRORS | AI_LOG_TIMESTAMP | AI_LOG_BACKTRACE | AI_LOG_MEMORY; break;
   case 0:  flags = 0; break;
   }

   return flags;
}

DLLEXPORT AtVoid MtoaLogCallback(int logmask, int severity, const char *msg_string, int tabs)
{
   /*
   const char *header = "[mtoa] %s";
   char *buf;
   buf = new (std::nothrow) char[strlen(header)+strlen(msg_string)];
   if (NULL != buf)
   {
      sprintf(buf, header, msg_string);
      clog << buf << endl;
      delete[] buf;
   }
   */


   // Some feedback will go in Maya script window
   // Need to handle regular log / output since
   // Seems green should be used for progress message, but no idea
   // how to detect we're currently displaying one
   // green is "\033[22;32m"
   // logmask & AI_LOG_COLOR is never true, even when AI_LOG_COLOR is set
   // in Console flags?
   switch (severity)
   {
   case AI_SEVERITY_INFO:
      if (logmask & AI_LOG_INFO)
         // MGlobal::displayInfo(msg_string);
         // default color
         // if (logmask & AI_LOG_COLOR) clog << "\033[0m";
#ifndef _WIN32
         clog << "\033[0m";
#endif
      break;
   case AI_SEVERITY_WARNING:
      if (logmask & AI_LOG_WARNINGS)
         MGlobal::displayWarning(msg_string);
         // yellow color
         // if (logmask & AI_LOG_COLOR) clog << "\033[22;33m";
#ifndef _WIN32
         clog << "\033[22;33m";
#endif
      break;
   case AI_SEVERITY_ERROR:
      if (logmask & AI_LOG_ERRORS)
         MGlobal::displayError(msg_string);
         // red color
         // if (logmask & AI_LOG_COLOR) clog << "\033[22;31m";
#ifndef _WIN32
         clog << "\033[22;31m";
#endif
      break;
   default:
      // default color
      // if (logmask & AI_LOG_COLOR) clog << "\033[0m";
#ifndef _WIN32
      clog << "\033[0m";
#endif
      break;
   }

   // Standard log output
#ifndef _WIN32
   clog << msg_string << "\033[0m" << endl;
#else
   clog << msg_string << endl;
#endif
   // How do we know to write both in cerr and clog, or write in logFile even ?
   // cerr << msg_string << endl;
}

// Setup a default logging level to use when not rendering.
// Logging parameters are stored on the render options node and are only put in place when a render
// is triggered.
DLLEXPORT void MtoaSetupLogging()
{
   AiMsgSetLogFileName(MString("$MTOA_LOG_PATH/arnold.log").expandEnvironmentVariablesAndTilde().asChar());
#ifdef NDEBUG
   int defaultLogFlags = (AI_LOG_ALL & ~AI_LOG_DEBUG);
#else
   int defaultLogFlags = AI_LOG_ALL;
#endif

   // MString loglevelStr = MString("$MTOA_LOG_VERBOSITY").expandEnvironmentVariablesAndTilde();
   // if (loglevelStr.isShort())
   //    defaultLogFlags = GetFlagsFromVerbosityLevel(loglevelStr.asShort());

   AiMsgSetConsoleFlags(defaultLogFlags | AI_LOG_COLOR);
   AiMsgSetLogFileFlags(defaultLogFlags);
   // Not working correctly until we can add to callback rather than replace it,
   // or have access to original callback code
   // AiMsgSetCallback(MtoaLogCallback);
}

void MtoaSetupSwatchLogging()
{
   AiMsgSetLogFileName(MString("$MTOA_LOG_PATH/arnold.log").expandEnvironmentVariablesAndTilde().asChar());
#ifdef NDEBUG
   int defaultLogFlags = AI_LOG_WARNINGS | AI_LOG_ERRORS | AI_LOG_TIMESTAMP | AI_LOG_BACKTRACE;
#else
   int defaultLogFlags = AI_LOG_ALL;
#endif

   // MString loglevelStr = MString("$MTOA_SWATCH_LOG_VERBOSITY").expandEnvironmentVariablesAndTilde();
   // if (loglevelStr.isShort())
   //    defaultLogFlags = GetFlagsFromVerbosityLevel(loglevelStr.asShort());


   // TODO: Should we be using render options for logging, or is it better not to clutter
   // the log with swatch output?
   // If we use global render options, m_renderSession->Init() already did
   // m_renderSession->m_renderOptions.SetupLog();
   // TODO: read these from an environment variable

   AiMsgSetConsoleFlags(defaultLogFlags | AI_LOG_COLOR);
   AiMsgSetLogFileFlags(defaultLogFlags);
   // Not working correctly until we can add to callback rather than replace it,
   // or have access to original callback code
   // AiMsgSetCallback(MtoaLogCallback);
}
