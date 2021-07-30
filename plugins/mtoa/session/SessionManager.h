#pragma once

#include "session/ArnoldSession.h"
#include <vector>
#include <set>
#include <map>
#include <string>


class DLLEXPORT CSessionManager
{

public:

   static bool AddActiveSession(const std::string &name, CArnoldSession *session);
   static bool DeleteActiveSession(const std::string &name);
   static CArnoldSession *FindActiveSession(const std::string &name);
   static void ClearActiveSessions();
   static void End();
     
private:
   CSessionManager() {}

   static unordered_map<std::string, CArnoldSession *> s_activeSessions;
   static MCallbackId s_QuitApplicationCallbackId;
   static MCallbackId s_FileOpenCallbackId;
   static AtMutex s_lock;
};
