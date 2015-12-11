
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

      default: rgba.r = rgba.g = rgba.b = 0.f; break; // ?
   }
   rgba.a = 1.f;

   int minX = xo;
   int minY = yo;
   int maxX = xo + xsize;
   int maxY = yo + ysize;

   const int sz = 4;
   // paint in checkerboard (8-pixels wide)
   for (int j = minY, by = 0; j < maxY; j++, by++)
   {
      for (int i = minX, bx = 0; i < maxX; i++, bx++)
      {
         if (((bx == 0 || bx == xsize - 1) && (by < sz || by > (ysize - sz-1))) ||
             ((by == 0 || by == ysize - 1) && (bx < sz || bx > (xsize - sz-1))))
         {
            // overwrite the color in the corners
            rv->SetPixelColor(i, j, rgba);
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
      case AI_TYPE_INT:
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
   if (!rv->GetShowRenderingTiles()) return;

   // translate to local display coordinates (cropped over overscan)
   int minX = bucket_xo - rv->m_minX;
   int minY = bucket_yo - rv->m_minY;
   int maxX = MIN(minX + bucket_size_x, rv->m_regX);
   int maxY = MIN(minY + bucket_size_y, rv->m_regY);

   // submit a 'prepare-bucket' event into our queue
   BucketSetThreadColor(rv,
                        minX, minY,
                        maxX - minX, maxY - minY,
                        tid);

   rv->Draw(minX, minY, maxX - minX, maxY - minY);
}

driver_process_bucket
{
   AtParamValue *params = AiNodeGetParams(node);
   CRenderView *rv = (CRenderView *) _userdata;

   // get the first AOV layer
   int pixelType;
   const void* bucketData;


   // translate to local display coordinates (cropped over overscan)
   int minX = bucket_xo - rv->m_minX;
   int minY = bucket_yo - rv->m_minY;
   int maxX = MIN(minX + bucket_size_x, rv->m_regX);
   int maxY = MIN(minY + bucket_size_y, rv->m_regY);


   int AASamples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
   int AASpacing = 1 << (-AASamples);
   int spacing = 1;
   bool hasSpacing = false;

   if (AASamples < 0 && (minX & (AASpacing-1)) == 0 && (minY & (AASpacing-1)) == 0)
   {
      spacing = AASpacing;
      hasSpacing = true;
   }


   //const char *aov_name;
   if (!AiOutputIteratorGetNext(iterator, NULL /* &aov_name */, &pixelType, &bucketData)) return;

   for (int j = minY; j < maxY; j+=spacing)
   {
      int inOffsetJ = (j - minY) * bucket_size_x;

      for (int i = minX; i < maxX; i+=spacing)
      {
         
         int inIdx = inOffsetJ + i - minX;
        // write to buffer
         if (hasSpacing)
         {
            rv->SetPixelsBlock(i, j, MIN(i + spacing, maxX), MIN(j + spacing, maxY), ((const AtRGBA *)bucketData)[inIdx]);
         } else
         {
            // simpler / faster case
            rv->SetPixelColor(i, j, ((const AtRGBA *)bucketData)[inIdx]);
         }
      }
   }
   if (K_enable_aovs)
   {
      int AOVIndex = 0;
      while (AiOutputIteratorGetNext(iterator, NULL /*&aov_name*/, &pixelType, &bucketData))
      {
         for (int j = minY; j < maxY; j+=spacing)
         {
            int inOffsetJ = (j - minY) * bucket_size_x;

            for (int i = minX; i < maxX; i+=spacing)
            {  
               int inIdx = inOffsetJ + i - minX;
              // write to buffer
               if (hasSpacing)
               {
                  switch(pixelType)
                  {
                     case AI_TYPE_RGBA:
                        rv->SetPixelsBlock(i, j,  MIN(i + spacing, maxX), MIN(j + spacing, maxY), ((const AtRGBA *)bucketData)[inIdx], AOVIndex);
                        break;

                     {
                     case AI_TYPE_RGB:
                     case AI_TYPE_VECTOR:
                     case AI_TYPE_POINT:
                        const AtRGB &rgb = ((const AtRGB *)bucketData)[inIdx];
                        AtRGBA rgba;
                        rgba.r = rgb.r;
                        rgba.g = rgb.g;
                        rgba.b = rgb.b;
                        rgba.a = 1.f;
                        rv->SetPixelsBlock(i, j,  MIN(i + spacing, maxX), MIN(j + spacing, maxY), rgba, AOVIndex);
                        break;
                     }
                     {
                     case AI_TYPE_FLOAT:
                        const float &flt = ((const float *)bucketData)[inIdx];
                        AtRGBA rgba;
                        rgba.r = rgba.g = rgba.b = flt;
                        rgba.a = 1.f;
                        rv->SetPixelsBlock(i, j,  MIN(i + spacing, maxX), MIN(j + spacing, maxY), rgba, AOVIndex);
                        break;
                     }
                     {
                        case AI_TYPE_INT:
                        const int &intVal = ((const int *)bucketData)[inIdx];
                        AtRGBA rgba;
                        // not a simple cast, because we'll want to get the original 
                        // int value later (when picking)

                        //*((int*)&rgba.r) = intVal;
                        rgba.r = reinterpret_type<int, float>(intVal);
                        rgba.g = rgba.b = rgba.r;
                        rgba.a = 1.f;
                        rv->SetPixelsBlock(i, j,  MIN(i + spacing, maxX), MIN(j + spacing, maxY), rgba, AOVIndex);
                        break;
                     }
                     default:
                        break;

                  }
               } else
               {
                  switch(pixelType)
                  {
                     case AI_TYPE_RGBA:
                        rv->SetAOVPixelColor(AOVIndex, i, j, ((const AtRGBA *)bucketData)[inIdx]);
                        break;
                     {
                     case AI_TYPE_RGB:
                     case AI_TYPE_VECTOR:
                     case AI_TYPE_POINT:
                        const AtRGB &rgb = ((const AtRGB *)bucketData)[inIdx];
                        AtRGBA rgba;
                        rgba.r = rgb.r;
                        rgba.g = rgb.g;
                        rgba.b = rgb.b;
                        rgba.a = 1.f;
                        rv->SetAOVPixelColor(AOVIndex, i, j, rgba);
                        break;
                     }

                     {
                     case AI_TYPE_FLOAT:
                        const float &flt = ((const float *)bucketData)[inIdx];
                        AtRGBA rgba;
                        rgba.r = rgba.g = rgba.b = flt;
                        rgba.a = 1.f;
                        rv->SetAOVPixelColor(AOVIndex, i, j, rgba);
                        break;
                     }
                     {
                     case AI_TYPE_INT:
                        const int &intVal = ((const int *)bucketData)[inIdx];
                        AtRGBA rgba;
                        // not a simple cast, because we'll want to get the original 
                        // int value later (when picking)
                        //*((int*)&rgba.r) = intVal;
                        rgba.r = reinterpret_type<int, float>(intVal);
                        rgba.g = rgba.b = rgba.r;
                        rgba.a = 1.f;
                        rv->SetAOVPixelColor(AOVIndex, i, j, rgba);
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

   rv->Draw(minX, minY, maxX - minX, maxY - minY);
}

driver_write_bucket
{
}

driver_close
{
   AtParamValue *params = AiNodeGetParams(node);
   CRenderView *rv = (CRenderView *)_userdata;

   // signal that a full frame has finished rendering
   rv->Draw();
}

node_finish
{
   AiDriverDestroy(node);
}

