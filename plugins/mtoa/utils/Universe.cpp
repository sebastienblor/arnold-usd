#include "Universe.h"
#include "extension/Extension.h"
#include "session/SessionManager.h"
#include "utils/MtoAAdpPayloads.h"

#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_render.h>
#include <ai_nodes.h>
#include <ai_node_entry.h>
#include <ai_plugins.h>
#include <ai_license.h>

#include <maya/MGlobal.h>
#include "../utils/BuildID.h"
#include "../utils/ConstantStrings.h"

extern const AtNodeMethods* mtoa_driver_mtd;

MString g_metafile = "";

void InstallNodes()
{
   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
      if (AiNodeEntryLookUp(str::renderview_display) == nullptr)
         AiNodeEntryInstall(AI_NODE_DRIVER,
                         AI_TYPE_NONE,
                         "renderview_display",
                         "mtoa",
                         (AtNodeMethods*) mtoa_driver_mtd,
                         AI_VERSION);
  
   }
}


bool ArnoldBegin(int logFlags)
{
   if (!AiArnoldIsActive())
   {
      // Report MtoA product usage to ADP
      MtoAADPPayloads::ADPPostProductMetadata();
      if (!logFlags)
	AiMsgSetConsoleFlags(NULL, AI_LOG_NONE);

      AiBegin(AI_SESSION_INTERACTIVE);
      MtoaSetupLogging(logFlags);

       MString mayaVersion = MGlobal::mayaVersion();     
       MString appString = MString("MtoA ") + MTOA_VERSION + " " + BUILD_ID + " Maya " + mayaVersion;
       AiSetAppString(appString.asChar());

      return true;
   }
   return false;
}

void ArnoldEnd()
{
   CSessionManager::End();
}

MString ArnoldGetEntryFile(const AtNodeEntry *entry)
{
   if (entry == NULL)
      return "";

   const char *filename = AiNodeEntryGetFilename(entry);
   if (filename == NULL)
      return "";

   std::string fileStr(filename);
   std::replace(fileStr.begin(), fileStr.end(), '\\', '/');

   return MString(fileStr.c_str());
}
