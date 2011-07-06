#include "Universe.h"

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
      return MStatus::kSuccess;
   }
   return MStatus::kFailure;
}

MStatus InitArnoldUniverse()
{
   if (!AiUniverseIsActive())
   {
      AiBegin();
      MtoaSetupLogging();
      return ReadMetafile();
   }
   return MStatus::kFailure;
}
