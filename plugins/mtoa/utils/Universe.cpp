#include "Universe.h"
#include "extension/Extension.h"

#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_render.h>

#include <maya/MGlobal.h>

extern AtNodeMethods* mtoa_driver_mtd;

MString g_metafile = "";

void SetMetafile(MString metafile)
{
   g_metafile = metafile;
}

MStatus ReadMetafile()
{
   AtBoolean readMetaSuccess = AiMetaDataLoadFile(g_metafile.asChar());
   if (!readMetaSuccess)
   {
      AiMsgError("[mtoa] Could not read mtoa built-in metadata file mtoa.mtd");
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
                         NULL,
                         (AtNodeMethods*) mtoa_driver_mtd,
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
      AiLoadPlugin(pluginFile.asChar());
   }
}

bool ArnoldUniverseBegin()
{
   if (!AiUniverseIsActive())
   {
      AiBegin();
      MtoaSetupLogging();
      LoadPlugins();
      ReadMetafile();
      return true;
   }
   return false;
}

void ArnoldUniverseEnd()
{
   if (AiUniverseIsActive())
   {
      AiRenderAbort();
      AiEnd();
      // MtoaSetupLogging();
      // AiMsgResetCallback();
   }
}
