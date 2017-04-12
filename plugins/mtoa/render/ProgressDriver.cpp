#include <ai.h>

#include <sstream>
#include <string>
#include <cstring> // for memset
#include <maya/MComputation.h>
#include <maya/MAtomic.h>
#include <maya/MRenderView.h>
#include <maya/MGlobal.h>
#include <maya/MImage.h>
#include <maya/MAnimControl.h>

#ifdef _LINUX
#include <climits>
#endif

AI_DRIVER_NODE_EXPORT_METHODS(progress_driver_mtd);

node_parameters
{
}


node_initialize
{
   AiDriverInitialize(node, false);
   
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
static int g_lastReportedPercent;

driver_open
{

   AtNode* options = AiUniverseGetOptions();
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
   g_lastReportedPercent = -1;

   MString cmd;
   cmd += "global string $gMainProgressBar;";
   cmd += "progressBar -edit -beginProgress -status \"Arnold Render ...\" -maxValue 100 -progress 0 $gMainProgressBar;";
   
   MGlobal::executeCommand(cmd, false);
   
}

driver_needs_bucket
{
   return true;
}

driver_process_bucket
{

}

driver_prepare_bucket
{
   
}

driver_write_bucket
{   

   g_calculatedPixels += bucket_size_x * bucket_size_y;      
   const int currentPercent = (int)(100.f * ((float)g_calculatedPixels / (float)g_totalPixels)) / 5;
   
   if (currentPercent != g_lastReportedPercent)
   {
      std::stringstream ss;
      g_lastReportedPercent = currentPercent;
      MString cmd;
      cmd += "global string $gMainProgressBar;";
      cmd += "progressBar -edit -progress ";
      cmd += currentPercent * 5;
      cmd += " $gMainProgressBar;";

      MGlobal::executeCommand(cmd, false);
   }      
}

driver_close
{
   MGlobal::executeCommand("global string $gMainProgressBar; progressBar -edit -endProgress $gMainProgressBar;", false);
}

node_finish
{
   
}
