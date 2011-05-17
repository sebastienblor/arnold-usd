#include "MtoaLogCallback.h"


DLLEXPORT AtVoid MtoaLogCallback(AtInt logmask, AtInt severity, const char *msg_string, AtInt tabs)
{
   const char *header = "[mtoa] %s";
   char *buf;
   buf = new (std::nothrow) char[strlen(header)+strlen(msg_string)];

   if (NULL == buf)
   {
      // Fallback so normal logging still works in case of an allocation failure
      clog << msg_string << endl;
   }
   else
   {
      sprintf(buf, header, msg_string);
      // Standard log output
      clog << buf << endl;
      // Some feedback will go in Maya script window
      switch (severity)
      {
      case AI_SEVERITY_INFO:
         if (logmask & AI_LOG_INFO)
            MGlobal::displayInfo(buf);
         break;
      case AI_SEVERITY_WARNING:
         if (logmask & AI_LOG_WARNINGS)
            // if (clog != cerr) cerr << buf << endl;
            MGlobal::displayWarning(buf);
         break;
      case AI_SEVERITY_ERROR:
         if (logmask & AI_LOG_ERRORS)
            // if (clog != cerr) cerr << buf << endl;
            MGlobal::displayError(buf);
         break;
      default:
         break;
      }

      delete[] buf;
   }
}

// Setup a default logging level to use when not rendering.
// Logging parameters are stored on the render options node and are only put in place when a render
// is triggered.
DLLEXPORT void SetupMtoaLogging()
{
   // TODO: read this initial value from an environment variable or option variable
#ifdef _DEBUG
   AtInt defaultLogFlags = AI_LOG_ALL;
#else
   AtInt defaultLogFlags = AI_LOG_INFO | AI_LOG_WARNINGS | AI_LOG_ERRORS | AI_LOG_TIMESTAMP | AI_LOG_BACKTRACE | AI_LOG_MEMORY | AI_LOG_COLOR;
#endif
   AiMsgSetConsoleFlags(defaultLogFlags);
   AiMsgSetCallback(MtoaLogCallback);
}
