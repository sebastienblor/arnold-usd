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

// void SetMetafile(MString metafile)
// {
//    g_metafile = metafile;
// }

// MStatus ReadMetafile()
// {
//    bool readMetaSuccess = AiMetaDataLoadFile(g_metafile.asChar());
//    if (!readMetaSuccess)
//    {
//       AiMsgError("[mtoa] Could not read mtoa built-in metadata file \"%s\"", g_metafile.asChar());
//       return MStatus::kFailure;
//    }
//    return MStatus::kSuccess;
// }

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

      AiBegin(AI_SESSION_INTERACTIVE);
      MtoaSetupLogging(logFlags);
      // ReadMetafile();

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
