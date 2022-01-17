#include "SessionManager.h"
#include "render/MaterialView.h"
#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>
#include <maya/MEventMessage.h>

MCallbackId CSessionManager::s_QuitApplicationCallbackId = 0;
MCallbackId CSessionManager::s_FileOpenCallbackId = 0;
AtMutex CSessionManager::s_lock;

unordered_map<std::string, CArnoldSession *> CSessionManager::s_activeSessions;


void FileOpenCallback(void *)
{
   CSessionManager::ClearActiveSessions();

   // something we might want to do when a new file is opened
   // We want to clear the caches, since the scene is changing (#3277)
   AiUniverseCacheFlush(nullptr, AI_CACHE_ALL);
   // tell MtoaLog that mtoa_translation_info might change
   UpdateMtoaTranslationInfo();
}


void QuitApplicationCallback(void *)
{
   // something we might want to do when closing maya
   CSessionManager::End();

}


bool CSessionManager::AddActiveSession(const std::string &name, CArnoldSession *session)
{
   std::lock_guard<AtMutex> guard(s_lock);

   if (s_activeSessions.find(name) != s_activeSessions.end())
   {
      AiMsgError("[mtoa] Session %s already exists", name.c_str());
      return false;
   }
   
   s_activeSessions[name] = session;
   
   session->SetSessionName(name.c_str());
   MStatus cbStatus;
   if (s_QuitApplicationCallbackId == 0)
   {
      MCallbackId id = MEventMessage::addEventCallback("quitApplication", QuitApplicationCallback, NULL, &cbStatus);
      if (cbStatus == MS::kSuccess) s_QuitApplicationCallbackId = id;
   }
   
   if (s_FileOpenCallbackId == 0)
   {
      MCallbackId id = MEventMessage::addEventCallback("PreFileNewOrOpened", FileOpenCallback, NULL, &cbStatus);
      if (cbStatus == MS::kSuccess) s_FileOpenCallbackId = id;
   }

   return true;
}

bool CSessionManager::DeleteActiveSession(const std::string &name)
{
   std::lock_guard<AtMutex> guard(s_lock);
   auto it = s_activeSessions.find(name);
   if (it == s_activeSessions.end())
      return false;

   delete it->second; // delete the session
   s_activeSessions.erase(it); // remove it from the list
   return true;
}

CArnoldSession *CSessionManager::FindActiveSession(const std::string &name)
{
   std::lock_guard<AtMutex> guard(s_lock);

   unordered_map<std::string, CArnoldSession *>::iterator iter = s_activeSessions.find(name);
   if (iter == s_activeSessions.end())
      return nullptr;
   return iter->second;

}

void CSessionManager::End()
{
   std::lock_guard<AtMutex> guard(s_lock);

   if (s_QuitApplicationCallbackId)
   {
      MMessage::removeCallback(s_QuitApplicationCallbackId);
      s_QuitApplicationCallbackId = 0;
   }

   if (s_FileOpenCallbackId)
   {
      MMessage::removeCallback(s_FileOpenCallbackId);
      s_FileOpenCallbackId = 0;
   }
   unordered_map<std::string, CArnoldSession *>::iterator iter = s_activeSessions.begin();
   for (; iter != s_activeSessions.end(); ++iter)
      delete iter->second;
   
   s_activeSessions.clear();
   // tell MtoaLog that mtoa_translation_info might change
   UpdateMtoaTranslationInfo();

   // Tell the material view to end its session
   CMaterialView::End();

   if (AiArnoldIsActive())
      AiEnd();

}

void CSessionManager::ClearActiveSessions()
{
   unordered_map<std::string, CArnoldSession *>::iterator iter = s_activeSessions.begin();
   for (; iter != s_activeSessions.end(); ++iter)
   {
      iter->second->Clear();
      iter->second->GetOptions().SetArnoldRenderOptions(MObject());
   }
}

unordered_map<std::string, CArnoldSession *> CSessionManager::GetActiveSessions()
{
   return s_activeSessions;
}