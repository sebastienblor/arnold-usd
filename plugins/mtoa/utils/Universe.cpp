#include "Universe.h"
#include "extension/Extension.h"
#include "render/RenderSession.h"
#include "scene/MayaScene.h"

#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_render.h>
#include <ai_nodes.h>
#include <ai_node_entry.h>
#include <ai_plugins.h>
#include <ai_license.h>

#include <maya/MGlobal.h>

extern const AtNodeMethods* mtoa_driver_mtd;
extern const AtNodeMethods* progress_driver_mtd;

MString g_metafile = "";

void SetMetafile(MString metafile)
{
   g_metafile = metafile;
}

MStatus ReadMetafile()
{
   bool readMetaSuccess = AiMetaDataLoadFile(g_metafile.asChar());
   if (!readMetaSuccess)
   {
      AiMsgError("[mtoa] Could not read mtoa built-in metadata file \"%s\"", g_metafile.asChar());
      return MStatus::kFailure;
   }
   return MStatus::kSuccess;
}

void InstallNodes()
{
   if (MGlobal::mayaState() == MGlobal::kInteractive)
   {
      AiNodeEntryInstall(AI_NODE_DRIVER,
                         AI_TYPE_NONE,
                         "renderview_display",
                         "mtoa",
                         (AtNodeMethods*) mtoa_driver_mtd,
                         AI_VERSION);


      AiNodeEntryInstall(AI_NODE_DRIVER,
                         AI_TYPE_NONE,
                         "progress_driver",
                         "mtoa",
                         (AtNodeMethods*) progress_driver_mtd,
                         AI_VERSION);
        
   }
}

// Reload the Arnold plugins that were registered by the extensions manager for this session
void LoadPlugins()
{
   MStringArray plugins = CExtension::GetAllLoadedArnoldPlugins();
   for (unsigned int i=0; i<plugins.length(); ++i)
   {
      const MString pluginFile = plugins[i];
      AiLoadPlugins(pluginFile.asChar());
   }
}

bool ArnoldUniverseBegin(int logFlags)
{
   if (!AiUniverseIsActive())
   {
      CRenderSession *renderSession = CMayaScene::GetRenderSession();
      bool isInteractiveSession = (renderSession == NULL || renderSession->IsInteractiveSession());
      AtSessionMode mode = (isInteractiveSession) ? AI_SESSION_INTERACTIVE : AI_SESSION_BATCH;

      AiBegin(mode);
      MtoaSetupLogging(logFlags);
      LoadPlugins();
      ReadMetafile();
      return true;
   }
   return false;
}

bool ArnoldUniverseOnlyBegin()
{
   if (!AiUniverseIsActive())
   {
      CRenderSession *renderSession = CMayaScene::GetRenderSession();
      bool isInteractiveSession = (renderSession == NULL || renderSession->IsInteractiveSession());
      AtSessionMode mode = (isInteractiveSession) ? AI_SESSION_INTERACTIVE : AI_SESSION_BATCH;

      AiBegin(mode);
      return true;
   }
   return false;
}

void ArnoldUniverseLoadPluginsAndMetadata()
{
   LoadPlugins();
   ReadMetafile();
}

void ArnoldUniverseEnd()
{
   if (AiUniverseIsActive())
   {
      if (AiRendering())
         AiRenderInterrupt();
      if (AiRendering())
         AiRenderAbort();
      AiEnd();
      // MtoaSetupLogging();
      // AiMsgResetCallback();
   }
}

void ArnoldUniverseEndAndFlush(int cache_flags)
{
   if (AiUniverseIsActive())
   {
      if (AiRendering())
         AiRenderInterrupt();
      if (AiRendering())
         AiRenderAbort();
      AiUniverseCacheFlush(cache_flags);
      AiEnd();
   }
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