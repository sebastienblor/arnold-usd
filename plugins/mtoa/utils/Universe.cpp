#include "Universe.h"

MString g_metafile = "";

void SetMetafile(MString metafile)
{
   g_metafile = metafile;
}

bool ReadMetafile()
{
   AtBoolean readMetaSuccess = AiMetaDataLoadFile(g_metafile.asChar());
   if (!readMetaSuccess)
   {
      AiMsgError("[mtoa] Could not read mtoa built-in metadata file mtoa.mtd");
      return false;
   }
   return true;
}

bool InitArnoldUniverse()
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
