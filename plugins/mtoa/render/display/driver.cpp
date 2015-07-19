
#include "renderview.h"
#include "render_gl_widget.h"
#include "display_gl.h"
#include "render_loop.h"

#include <ai.h>

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <limits>
#include <iostream>


#pragma warning (disable : 4244)

/*
 * Displays a rectangle color based on thread ID
 */
static void BucketSetThreadColor(CRenderView* rv, int xo, int yo, int xsize, int ysize, int tid)
{
   AtRGBA rgba;

   // pick a color based on thread ID
   switch (tid % 6)
   {

      case 0: rgba.r = 1.f; rgba.g = 0.f; rgba.b = 0.f; break;
      case 1: rgba.r = 0.f; rgba.g = 1.f; rgba.b = 0.f; break;
      case 2: rgba.r = 0.f; rgba.g = 0.f; rgba.b = 1.f; break;
      case 3: rgba.r = 1.f; rgba.g = 1.f; rgba.b = 0.f; break;
      case 4: rgba.r = 1.f; rgba.g = 0.f; rgba.b = 1.f; break;
      case 5: rgba.r = 0.f; rgba.g = 1.f; rgba.b = 1.f; break;

      default: rgba.r = rgba.b = rgba.b = 0.f; break; // ?
   }
   rgba.a = 1.f;

   int min_x = xo;
   int min_y = yo;
   int max_x = xo + xsize;
   int max_y = yo + ysize;


   // paint in checkerboard (8-pixels wide)
   for (int j = min_y, by = 0; j < max_y; j++, by++)
   {
      for (int i = min_x, bx = 0; i < max_x; i++, bx++)
      {
         if (((bx == 0 || bx == xsize - 1) && (5 * by < ysize || 5 * (ysize - by - 1) < ysize)) ||
             ((by == 0 || by == ysize - 1) && (5 * bx < xsize || 5 * (xsize - bx - 1) < xsize)))
         {
            // overwrite the color in the corners
            rv->setPixelColor(i, j, rgba);
         }
      }
   }
}



/*
 * Driver Node
 */

AI_DRIVER_NODE_EXPORT_METHODS(kick_driver_mtd);

#define _userdata  (params[0].PTR )

node_parameters
{
   AiParameterPTR ( "userdata", NULL );
}

node_initialize
{
   AiDriverInitialize(node, true, NULL);
}

node_update
{
}

driver_supports_pixel_type
{
   switch (pixel_type)
   {
      case AI_TYPE_RGB:
      case AI_TYPE_RGBA:
      case AI_TYPE_FLOAT:
      case AI_TYPE_VECTOR:
      case AI_TYPE_POINT:
         return true;
      default:
         std::string msg = "[RenderView] Driver Type not supported ";
         msg += AiParamGetTypeName(pixel_type);
         AiMsgWarning(msg.c_str());
         return false;
   }
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

driver_prepare_bucket
{

   AtParamValue *params = AiNodeGetParams(node);
   CRenderView *rv = (CRenderView *) _userdata;
   if (!rv->getShowRenderingTiles()) return;

   // translate to local display coordinates (cropped over overscan)
   int min_x = bucket_xo - rv->min_x;
   int min_y = bucket_yo - rv->min_y;
   int max_x = MIN(min_x + bucket_size_x, rv->reg_x);
   int max_y = MIN(min_y + bucket_size_y, rv->reg_y);

   // submit a 'prepare-bucket' event into our queue
   BucketSetThreadColor(rv,
                        min_x, min_y,
                        max_x - min_x, max_y - min_y,
                        tid);

   rv->draw(min_x, min_y, max_x - min_x, max_y - min_y);
}

driver_process_bucket
{
   AtParamValue *params = AiNodeGetParams(node);
   CRenderView *rv = (CRenderView *) _userdata;

   // get the first AOV layer
   int pixel_type;
   const void* bucket_data;


   // translate to local display coordinates (cropped over overscan)
   int min_x = bucket_xo - rv->min_x;
   int min_y = bucket_yo - rv->min_y;
   int max_x = MIN(min_x + bucket_size_x, rv->reg_x);
   int max_y = MIN(min_y + bucket_size_y, rv->reg_y);


   int AA_samples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
   int AA_spacing = 1 << (-AA_samples);
   int spacing = 1;
   bool has_spacing = false;

   if (AA_samples < 0 && (min_x & (AA_spacing-1)) == 0 && (min_y & (AA_spacing-1)) == 0)
   {
      spacing = AA_spacing;
      has_spacing = true;
   }


   //const char *aov_name;
   if (!AiOutputIteratorGetNext(iterator, NULL /* &aov_name */, &pixel_type, &bucket_data)) return;

   for (int j = min_y; j < max_y; j+=spacing)
   {
      int in_j_offset = (j - min_y) * bucket_size_x;

      for (int i = min_x; i < max_x; i+=spacing)
      {
         
         int in_idx = in_j_offset + i - min_x;
        // write to buffer
         if (has_spacing)
         {
            for (int sj = j; sj < MIN(j + spacing, max_y); sj++)
            {
               for (int si = i; si < MIN(i + spacing, max_x); si++)
               {
                  rv->setPixelColor(si, sj, ((const AtRGBA *)bucket_data)[in_idx]);
               }
            }
         } else
         {
            // simpler / faster case
            rv->setPixelColor(i, j, ((const AtRGBA *)bucket_data)[in_idx]);
          
         }
      }
   }
   if (K_enable_aovs)
   {
      int AOVIndex = 0;
      while (AiOutputIteratorGetNext(iterator, NULL /*&aov_name*/, &pixel_type, &bucket_data))
      {
         for (int j = min_y; j < max_y; j+=spacing)
         {
            int in_j_offset = (j - min_y) * bucket_size_x;

            for (int i = min_x; i < max_x; i+=spacing)
            {  
               int in_idx = in_j_offset + i - min_x;
              // write to buffer
               if (has_spacing)
               {
                  for (int sj = j; sj < MIN(j + spacing, max_y); sj++)
                  {
                     for (int si = i; si < MIN(i + spacing, max_x); si++)
                     {
                        switch(pixel_type)
                        {
                           case AI_TYPE_RGBA:
                              rv->setAOVPixelColor(AOVIndex, si, sj, ((const AtRGBA *)bucket_data)[in_idx]);
                              break;
                           {
                           case AI_TYPE_RGB:
                           case AI_TYPE_VECTOR:
                           case AI_TYPE_POINT:
                              const AtRGB &rgb = ((const AtRGB *)bucket_data)[in_idx];
                              AtRGBA rgba;
                              rgba.r = rgb.r;
                              rgba.g = rgb.g;
                              rgba.b = rgb.b;
                              rgba.a = 1.f;
                              rv->setAOVPixelColor(AOVIndex, si, sj, rgba);
                              break;
                           }

                           {
                           case AI_TYPE_FLOAT:
                              const float &flt = ((const float *)bucket_data)[in_idx];
                              AtRGBA rgba;
                              rgba.r = rgba.g = rgba.b = flt;
                              rgba.a = 1.f;

                              rv->setAOVPixelColor(AOVIndex, si, sj, rgba);
                              break;
                           }
                           default:
                              break;

                        }
                     }
                  }
               } else
               {
                  switch(pixel_type)
                  {
                     case AI_TYPE_RGBA:
                        rv->setAOVPixelColor(AOVIndex, i, j, ((const AtRGBA *)bucket_data)[in_idx]);
                        break;
                     {
                     case AI_TYPE_RGB:
                     case AI_TYPE_VECTOR:
                     case AI_TYPE_POINT:
                        const AtRGB &rgb = ((const AtRGB *)bucket_data)[in_idx];
                        AtRGBA rgba;
                        rgba.r = rgb.r;
                        rgba.g = rgb.g;
                        rgba.b = rgb.b;
                        rgba.a = 1.f;
                        rv->setAOVPixelColor(AOVIndex, i, j, rgba);
                        break;
                     }

                     {
                     case AI_TYPE_FLOAT:
                        const float &flt = ((const float *)bucket_data)[in_idx];
                        AtRGBA rgba;
                        rgba.r = rgba.g = rgba.b = flt;
                        rgba.a = 1.f;

                        rv->setAOVPixelColor(AOVIndex, i, j, rgba);
                        break;
                     }
                     default:
                        break;
                  }
               }
            }
         }
         AOVIndex++;
      }
   }

   rv->draw(min_x, min_y, max_x - min_x, max_y - min_y);
}

driver_write_bucket
{
}

driver_close
{
   AtParamValue *params = AiNodeGetParams(node);
   CRenderView *rv = (CRenderView *)_userdata;

   // signal that a full frame has finished rendering
   rv->draw();
}

node_finish
{
   AiDriverDestroy(node);
}

