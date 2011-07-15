#include "Universe.h"

#include <ai_universe.h>
#include <ai_metadata.h>
#include <ai_render.h>

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

bool ArnoldUniverseBegin()
{
   if (!AiUniverseIsActive())
   {
      AiBegin();
      MtoaSetupLogging();
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
