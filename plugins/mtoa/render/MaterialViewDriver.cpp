#include <vector>
#include "platform/Platform.h"
#include "render/OutputDriver.h"
#include "render/MaterialView.h"
#include "scene/MayaScene.h"

#include <ai_critsec.h>
#include <ai_drivers.h>
#include <ai_filters.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AI_DRIVER_NODE_EXPORT_METHODS(materialview_driver_mtd);

namespace 
{

enum MaterialViewDriverParams
{
   p_view,
};

static CMaterialView* s_view;

};

node_parameters
{
   // Use a pointer parameter to set the view pointer
   AiParameterPTR("view", NULL);
   AiMetaDataSetBool(mds, "view", "maya.hide", true);

   AiMetaDataSetStr(mds, NULL, "maya.translator", "maya");
   AiMetaDataSetStr(mds, NULL, "maya.attr_prefix", "");
   AiMetaDataSetBool(mds, NULL, "single_layer_driver", true);
   AiMetaDataSetBool(mds, NULL, "display_driver", true);
}

node_initialize
{
   s_view = (CMaterialView*)params[p_view].PTR;
   AiDriverInitialize(node, false, NULL);
}

node_update
{
}

driver_supports_pixel_type
{
   return pixel_type == AI_TYPE_RGBA;
}

driver_extension
{
   return NULL;
}

driver_open
{
}

driver_needs_bucket
{
   return true;
}

driver_process_bucket
{
   if (s_view == NULL)
      return;

   int         pixel_type;
   const void* bucket_data;

   // Get the first AOV layer
   if (!AiOutputIteratorGetNext(iterator, NULL, &pixel_type, &bucket_data) || pixel_type != AI_TYPE_RGBA)
      return;

   // TODO: Don't allocate memory for each new tile
   RV_PIXEL* pixels = new RV_PIXEL[bucket_size_x * bucket_size_y];
   int minx = bucket_xo;
   int miny = bucket_yo;
   int maxx = bucket_xo + bucket_size_x - 1;
   int maxy = bucket_yo + bucket_size_y - 1;

   for (int j = miny; (j <= maxy); ++j)
   {
      for (int i = minx; (i <= maxx); ++i)
      {
         const unsigned int in_idx = (j - bucket_yo) * bucket_size_x + (i-bucket_xo);
         const AtRGBA& rgba = ((AtRGBA*)bucket_data)[in_idx]; 

         // Flip vertically
         const int targetX = i - minx;
         const int targetY = bucket_size_y - (j - miny) - 1;

         const unsigned int out_idx = targetY * bucket_size_x + targetX;
         RV_PIXEL* pixel = &pixels[out_idx];

         pixel->r = rgba.r;
         pixel->g = rgba.g;
         pixel->b = rgba.b;
         pixel->a = rgba.a;
      }
   }

   s_view->SendBucketToView(minx, maxx, miny, maxy, (void*)pixels);

   delete [] pixels;
}

driver_prepare_bucket
{
}

driver_write_bucket
{
}

driver_close
{
}

node_finish
{
   s_view = NULL;

   // release the driver
   AiDriverDestroy(node);
}
