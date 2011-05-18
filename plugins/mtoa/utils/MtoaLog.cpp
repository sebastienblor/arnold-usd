#include "MtoaLog.h"


DLLEXPORT AtVoid MtoaLogCallback(AtInt logmask, AtInt severity, const char *msg_string, AtInt tabs)
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
         // black color
         // if (logmask & AI_LOG_COLOR) clog << "\033[22;30m";
         clog << "\033[22;30m";
      break;
   case AI_SEVERITY_WARNING:
      if (logmask & AI_LOG_WARNINGS)
         MGlobal::displayWarning(msg_string);
         // yellow color
         // if (logmask & AI_LOG_COLOR) clog << "\033[22;33m";
         clog << "\033[22;33m";
      break;
   case AI_SEVERITY_ERROR:
      if (logmask & AI_LOG_ERRORS)
         MGlobal::displayError(msg_string);
         // red color
         // if (logmask & AI_LOG_COLOR) clog << "\033[22;31m";
         clog << "\033[22;31m";
      break;
   default:
      // black color
      // if (logmask & AI_LOG_COLOR) clog << "\033[22;30m";
      clog << "\033[22;30m";
      break;
   }

   // Standard log output
   clog << msg_string << "\033[22;30m" << endl;
   // How do we know to write both in cerr and clog, or write in logFile even ?
   // cerr << msg_string << endl;
}

// Setup a default logging level to use when not rendering.
// Logging parameters are stored on the render options node and are only put in place when a render
// is triggered.
DLLEXPORT void MtoaSetupLogging()
{
   // TODO: read initial values from an environment variable or option variable?
   AiMsgSetLogFileName(MString("$MTOA_LOG_PATH/arnold.log").expandEnvironmentVariablesAndTilde().asChar());
#ifdef _DEBUG
   AtInt defaultLogFlags = AI_LOG_ALL;
#else
   AtInt defaultLogFlags = (AI_LOG_ALL & ~AI_LOG_DEBUG);
#endif
   AiMsgSetConsoleFlags(defaultLogFlags | AI_LOG_COLOR);
   AiMsgSetLogFileFlags(defaultLogFlags);
   AiMsgSetCallback(MtoaLogCallback);
}
