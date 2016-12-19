#include <ai.h>

#include <sstream>
#include <string>
#include <cstring> // for memset

#ifdef _LINUX
#include <climits>
#endif

#include "session/ArnoldSession.h"
#include "scene/MayaScene.h"

#include <maya/MRenderUtil.h>
#include <maya/MString.h>
#include <maya/MAtomic.h>
#include <maya/MMutexLock.h>

AI_DRIVER_NODE_EXPORT_METHODS(batch_progress_driver_mtd);

enum{
    FRAME_START = -1111, // magic number for starting frame 
    FRAME_END   = 100
};

static std::string filename;
static int lastpercent = -1;
void sendProgress(int percent){
    if(lastpercent != percent){
        MRenderUtil::sendRenderProgressInfo(MString(filename.c_str()), percent);
    }
}

node_parameters
{
   AiParameterInt("port", 4700);
}

node_initialize
{
   AiDriverInitialize(node, false, NULL);
}

node_update
{
}

node_finish
{
   AiDriverDestroy(node);
}

driver_supports_pixel_type
{
   return true;
}

driver_extension
{
   return NULL;
}

static int g_totalPixels;
static int g_calculatedPixels;

driver_open
{
   AtNode* options = AiUniverseGetOptions();

   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();

   MStringArray imageFilenames = arnoldSession->GetActiveImageFilenames();
   filename.clear();
   if (imageFilenames.length() > 0)
   {
      // Maya's batch progress can only output a single filename
      // so just send the first filename
      filename = imageFilenames[0].asChar();
   }

   lastpercent = -1;
   sendProgress(FRAME_START);

   const int regionMinX = AiNodeGetInt(options, "region_min_x");
   if (regionMinX == INT_MIN)
   {
      const int xRes = AiNodeGetInt(options, "xres");
      const int yRes = AiNodeGetInt(options, "yres");
      
      g_totalPixels = xRes * yRes;
   }
   else
   {
      const int regionMinY = AiNodeGetInt(options, "region_min_y");
      const int regionMaxX = AiNodeGetInt(options, "region_max_x");
      const int regionMaxY = AiNodeGetInt(options, "region_max_y");
      
      g_totalPixels = (regionMaxX - regionMinX + 1) * (regionMaxY - regionMinY + 1);
   }
   
   g_calculatedPixels = 0;
}

driver_needs_bucket
{
   return true;
}

driver_process_bucket
{
   MAtomic::increment(&g_calculatedPixels, bucket_size_x * bucket_size_y);
   bool need_update = true; // should we use a timer ?

   static MMutexLock lock;
   if (need_update && lock.tryLock())
   {
      sendProgress((int)(100.f * ((float)g_calculatedPixels / (float)g_totalPixels)));
      lock.unlock();
   } 
}

driver_prepare_bucket
{

}

driver_write_bucket
{  
   
}

driver_close
{
   sendProgress(FRAME_END);
}

