#include <ai.h>

#include <maya/MGlobal.h>

AI_DRIVER_NODE_EXPORT_METHODS(batch_progress_driver_mtd);

node_parameters
{
   
}

node_initialize
{
   AiDriverInitialize(node, false, NULL);
}

node_update
{
   
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
   const int regionMinX = AiNodeGetInt(options, "region_min_x");
   if (regionMinX == -1)
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

driver_prepare_bucket
{
   
}

#include <fstream>

driver_write_bucket
{
   g_calculatedPixels += bucket_size_x * bucket_size_y;
   MString str;
   str += "Render progress : ";
   str += (int)(100.f * ((float)g_calculatedPixels / (float)g_totalPixels));
   MGlobal::displayInfo(str);
   std::fstream fs("/work/test.tx", std::ios::app);
   fs << 100.f * ((float)g_calculatedPixels / (float)g_totalPixels) << std::endl;
}

driver_close
{
   
}

node_finish
{
   
}