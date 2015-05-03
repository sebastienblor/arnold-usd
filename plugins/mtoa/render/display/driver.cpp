
#include "renderview.h"
#include "render_gl_widget.h"
#include "display_gl.h"

#include <ai.h>

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <limits>
#include <iostream>


#pragma warning (disable : 4244)
/*
 * Utility Functions
 */
/*
 * Displays a rectangle color based on thread ID
 */
static void BucketSetThreadColor(CRenderView* rv, int xo, int yo, int xsize, int ysize, int tid)
{
   unsigned char R,G,B;

   // pick a color based on thread ID
   switch (tid % 6)
   {
      case 0: R = 0xFF; G = 0x00; B = 0x00; break;
      case 1: R = 0x00; G = 0xFF; B = 0x00; break;
      case 2: R = 0x00; G = 0x00; B = 0xFF; break;
      case 3: R = 0xFF; G = 0xFF; B = 0x00; break;
      case 4: R = 0xFF; G = 0x00; B = 0xFF; break;
      case 5: R = 0x00; G = 0xFF; B = 0xFF; break;
      default: R = G = B = 0; break; // ?
   }

   int min_x = xo;
   int min_y = yo;
   int max_x = xo + xsize;
   int max_y = yo + ysize;

   AtRGBA8 *rgb_buffer =  rv->getBuffer();
   int buffer_width = rv->getBufferWidth();

   // paint in checkerboard (8-pixels wide)
   for (int j = min_y, by = 0; j < max_y; j++, by++)
   {
      for (int i = min_x, bx = 0; i < max_x; i++, bx++)
      {
         if (((bx == 0 || bx == xsize - 1) && (5 * by < ysize || 5 * (ysize - by - 1) < ysize)) ||
             ((by == 0 || by == ysize - 1) && (5 * bx < xsize || 5 * (xsize - bx - 1) < xsize)))
         {
            // overwrite the color in the corners
            int out_idx = (j * buffer_width + i);
            rgb_buffer[out_idx].r = R;
            rgb_buffer[out_idx].g = G;
            rgb_buffer[out_idx].b = B;
            rgb_buffer[out_idx].a = 255;
         }
      }
   }
}



/*
 * Driver Node
 */

AI_DRIVER_NODE_EXPORT_METHODS(kick_driver_mtd);

#define _userdata  (params[0].PTR )
#define _dither    (params[1].BOOL)
#define _gamma     (params[2].FLT )

node_parameters
{
   AiParameterPTR ( "userdata", NULL );
   AiParameterBOOL( "dither"  , true );
   AiParameterFLT ( "gamma"   , 1.0f );
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
   switch (pixel_type)
   {
      case AI_TYPE_RGB:
      case AI_TYPE_RGBA:
         return true;
      default:
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
   return;

   AtParamValue *params = AiNodeGetParams(node);
   CRenderView *rv = (CRenderView *) _userdata;

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

   if (!AiOutputIteratorGetNext(iterator, NULL, &pixel_type, &bucket_data))
      return;

   // translate to local display coordinates (cropped over overscan)
   int min_x = bucket_xo - rv->min_x;
   int min_y = bucket_yo - rv->min_y;
   int max_x = MIN(min_x + bucket_size_x, rv->reg_x);
   int max_y = MIN(min_y + bucket_size_y, rv->reg_y);

   AtRGBA8 *rgb_buffer = rv->getBuffer();
   int buffer_width = rv->getBufferWidth();
   float gamma = rv->gamma;
   bool dither = rv->dither;

   // for AA_samples < 0, arnold scales up the image, save time by doing gamma
   // correction only for every n-th pixel as they have the same value
   int AA_samples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
   int AA_spacing = 1 << (-AA_samples);
   int spacing = 1;

   if (AA_samples < 0 && (min_x & (AA_spacing-1)) == 0 && (min_y & (AA_spacing-1)) == 0)
      spacing = AA_spacing;

   for (int j = min_y; j < max_y; j+=spacing)
   {
      int in_j = j - min_y;

      for (int i = min_x; i < max_x; i+=spacing)
      {
         AtRGBA rgba;

         // retrieve data from bucket
         int in_idx = in_j * bucket_size_x + i - min_x;

         if (pixel_type == AI_TYPE_RGB)
         {
            rgba.rgb() = ((const AtRGB *)bucket_data)[in_idx];
            rgba.a = 1.0f;
         }
         else
         {
            rgba = ((const AtRGBA *)bucket_data)[in_idx];
         }

         // apply gamma
         if (gamma != 1.0f)
         {
            AtRGB& rgb = rgba.rgb();

            AiColorClamp(rgb, rgb, 0, 1);
            AiColorGamma(&rgb, gamma);
         }

         // quantize
         AtRGBA8 rgba8;

         rgba8.r = AiQuantize8bit(i, j, 0, rgba.r, dither);
         rgba8.g = AiQuantize8bit(i, j, 1, rgba.g, dither);
         rgba8.b = AiQuantize8bit(i, j, 2, rgba.b, dither);
         rgba8.a = AiQuantize8bit(i, j, 3, rgba.a, dither);
        // write to buffer
         for (int sj = j; sj < MIN(j + spacing, max_y); sj++)
         {
            for (int si = i; si < MIN(i + spacing, max_x); si++)
            {
               int out_idx = sj * buffer_width + si;
               rgb_buffer[out_idx] = rgba8;
            }
         }
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

