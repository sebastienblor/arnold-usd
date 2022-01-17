#include "MtoaLog.h"
#include "../session/SessionOptions.h"
#include "../session/ArnoldSession.h"

#include <maya/MGlobal.h>
#include <iostream>

int GetFlagsFromVerbosityLevel(unsigned int level)
{
   int flags = 0;
   switch(level)
   {
      case MTOA_LOG_DEBUG:  flags = AI_LOG_ALL; break;
      case MTOA_LOG_INFO:  flags |= AI_LOG_INFO | AI_LOG_STATS | AI_LOG_PLUGINS | AI_LOG_PROGRESS | AI_LOG_TIMESTAMP | AI_LOG_MEMORY ;
      default:
      case MTOA_LOG_WARNINGS:  flags |= AI_LOG_WARNINGS | AI_LOG_TIMESTAMP | AI_LOG_MEMORY;
      case MTOA_LOG_ERRORS:  flags |= AI_LOG_ERRORS | AI_LOG_TIMESTAMP | AI_LOG_MEMORY | AI_LOG_BACKTRACE ; break; 
   }
   return flags;
}

DLLEXPORT void MtoaLogCallback(int logmask, int severity, const char *msg_string, AtParamValueMap* metadata, void* user_ptr)
{
   /*
   const char *header = "[mtoa] %s";
   char *buf;
   buf = new (std::nothrow) char[strlen(header)+strlen(msg_string)];
   if (NULL != buf)
   {
      sprintf(buf, header, msg_string);
      std::clog << buf << std::endl;
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
         // if (logmask & AI_LOG_COLOR) std::clog << "\033[0m";
#ifndef _WIN32
         std::clog << "\033[0m";
#endif
      break;
   case AI_SEVERITY_WARNING:
      if (logmask & AI_LOG_WARNINGS)
         MGlobal::displayWarning(msg_string);
         // yellow color
         // if (logmask & AI_LOG_COLOR) std::clog << "\033[22;33m";
#ifndef _WIN32
         std::clog << "\033[22;33m";
#endif
      break;
   case AI_SEVERITY_ERROR:
      if (logmask & AI_LOG_ERRORS)
         MGlobal::displayError(msg_string);
         // red color
         // if (logmask & AI_LOG_COLOR) std::clog << "\033[22;31m";
#ifndef _WIN32
         std::clog << "\033[22;31m";
#endif
      break;
   default:
      // default color
      // if (logmask & AI_LOG_COLOR) std::clog << "\033[0m";
#ifndef _WIN32
      std::clog << "\033[0m";
#endif
      break;
   }

   // Standard log output
#ifndef _WIN32
   std::clog << msg_string << "\033[0m" << std::endl;
#else
   std::clog << msg_string << std::endl;
#endif
   // How do we know to write both in cerr and std::clog, or write in logFile even ?
   // cerr << msg_string << std::endl;
}

// Setup a default logging level to use when not rendering.
// Logging parameters are stored on the render options node and are only put in place when a render
// is triggered.
DLLEXPORT void MtoaSetupLogging(int logFlags)
{
   if (getenv("MTOA_LOG_PATH") != 0)
      AiMsgSetLogFileName(MString("$MTOA_LOG_PATH/arnold.log").expandEnvironmentVariablesAndTilde().asChar());

   AiMsgSetConsoleFlags(nullptr, logFlags | AI_LOG_COLOR);
   AiMsgSetLogFileFlags(nullptr, logFlags);
   // Not working correctly until we can add to callback rather than replace it,
   // or have access to original callback code
   // AiMsgSetCallback(MtoaLogCallback);
}

static int s_mtoaTranslationInfo = -1;
DLLEXPORT bool MtoaTranslationInfo()
{
   if (s_mtoaTranslationInfo < 0)
   {
      MObject options;
      MSelectionList list;
      list.add("defaultArnoldRenderOptions");
      if (list.length() == 0)
         return false;

      list.getDependNode(0, options);
      MStatus status;
      MFnDependencyNode optNode(options, &status);

      bool info = (status == MS::kSuccess) ? optNode.findPlug("mtoa_translation_info", true).asBool() : false;
      s_mtoaTranslationInfo = (info) ? 1 : 0;
   }

   return s_mtoaTranslationInfo > 0 ? true : false;
}
// the options node has changed, we can't keep using the cached value of mtoa_translation_info
DLLEXPORT void UpdateMtoaTranslationInfo()
{
   s_mtoaTranslationInfo = -1;
}

DLLEXPORT void MtoaDebugLog(MString log)
{
   // Do we want to dump this as a Maya log or as an arnold log ??
   MGlobal::displayInfo(log);
   std::clog<<log.asChar()<<std::endl;
}
