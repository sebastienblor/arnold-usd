
#include "common/MTBlockingQueue.h"

#include "render/RenderSession.h"
#include "render/OutputDriver.h"

#include <ai_critsec.h>
#include <ai_drivers.h>
#include <ai_filters.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>

#include <maya/MComputation.h>
#include <maya/MRenderView.h>
#include <maya/MGlobal.h>
#include <maya/MImage.h>

#include <time.h>
time_t s_start_time;

/// \defgroup render_view Render View Output Driver
/// These are the methods that make up the arnold side of the Render View update code.
/// @{


#define _gamma  (params[0].FLT)  /**< accessor for driver's gamma parameter */

AI_DRIVER_NODE_EXPORT_METHODS(mtoa_driver_mtd);


struct COutputDriverData
{
   AtUInt    imageWidth, imageHeight;
   float     gamma;
   AtBoolean rendering;
};

enum EDisplayUpdateMessageType
{
   MSG_BUCKET_PREPARE,
   MSG_BUCKET_UPDATE,
   MSG_IMAGE_COMPLETE,
   MSG_RENDER_DONE
};

struct CDisplayUpdateMessage
{
   EDisplayUpdateMessageType msgType;
   AtBBox2                   bucketRect;
   RV_PIXEL*                 pixels;      ///< These will be in the range of 0-255, not 0-1.
   CDisplayUpdateMessage(EDisplayUpdateMessageType msg = MSG_BUCKET_PREPARE,
                           AtInt minx = 0, AtInt miny = 0, AtInt maxx = 0, AtInt maxy = 0,
                           RV_PIXEL* px = NULL)
   : msgType(msg), pixels(px)
   {
      bucketRect.minx = minx;
      bucketRect.miny = miny;
      bucketRect.maxx = maxx;
      bucketRect.maxy = maxy;
   }
};

static CMTBlockingQueue<CDisplayUpdateMessage> s_displayUpdateQueue;
static COutputDriverData                       s_outputDriverData;
static bool                                    s_finishedRendering;


/// \name Arnold Output Driver.
/// \{
node_parameters
{
   AiParameterFLT ("gamma", 1.0f);
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

node_initialize
{
   AiDriverInitialize(node, FALSE, NULL);
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
         return TRUE;
      default:
         return FALSE;
   }
}

driver_extension
{
   return NULL;
}

driver_open
{
   AtParamValue *params = node->params;

   if (!s_outputDriverData.rendering)
   {
      s_outputDriverData.imageWidth  = display_window.maxx - display_window.minx + 1;
      s_outputDriverData.imageHeight = display_window.maxy - display_window.miny + 1;
      s_outputDriverData.gamma       = _gamma;
      s_outputDriverData.rendering   = TRUE;
   }
}

driver_prepare_bucket
{
   CDisplayUpdateMessage   msg(MSG_BUCKET_PREPARE,
                               bucket_xo, bucket_yo,
                               bucket_xo + bucket_size_x - 1, bucket_yo + bucket_size_y - 1,
                               NULL) ;

   // msg.msgType = MSG_BUCKET_PREPARE;
   // msg.bucketRect.minx = bucket_xo;
   // msg.bucketRect.miny = bucket_yo;
   // msg.bucketRect.maxx = bucket_xo + bucket_size_x - 1;
   // msg.bucketRect.maxy = bucket_yo + bucket_size_y - 1;
   // msg.pixels          = NULL;

   s_displayUpdateQueue.push(msg);
}

/// Convert the data to Maya format.
/// driver_write_bucket takes the data from Arnold and converts it to
/// 0-255 instead of 0-1. It also flips it around height.
driver_write_bucket
{
   AtInt         pixel_type;
   const AtVoid* bucket_data;

   // get the first AOV layer
   if (!AiOutputIteratorGetNext(iterator, NULL, &pixel_type, &bucket_data))
      return;

   RV_PIXEL* pixels = new RV_PIXEL[bucket_size_x * bucket_size_y];
   AtInt minx = bucket_xo;
   AtInt miny = bucket_yo;
   AtInt maxx = bucket_xo + bucket_size_x - 1;
   AtInt maxy = bucket_yo + bucket_size_y - 1;

   switch(pixel_type)
   {
      case AI_TYPE_RGB:
      {
         for (AtInt j = miny; (j <= maxy); ++j)
         {
            for (AtInt i = minx; (i <= maxx); ++i)
            {
               AtUInt in_idx = (j-bucket_yo)*bucket_size_x + (i-bucket_xo);
               AtRGB  rgb = ((AtRGB*)bucket_data)[in_idx]; 

               // Flip vertically
               AtInt targetX = i - minx;
               AtInt targetY = bucket_size_y - (j - miny) - 1;

               AtUInt out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               // JS: Commented this out, makes no sense.
               //AiColorClamp(rgb, rgb, 0, 1);
               AiColorGamma(&rgb, s_outputDriverData.gamma);

               pixel->r = rgb.r * 255;
               pixel->g = rgb.g * 255;
               pixel->b = rgb.b * 255;
               pixel->a = 0;
            }
         }
         break;
      }

      case AI_TYPE_RGBA:
      {
         for (AtInt j = miny; (j <= maxy); ++j)
         {
            for (AtInt i = minx; (i <= maxx); ++i)
            {
               AtUInt in_idx = (j-bucket_yo)*bucket_size_x + (i-bucket_xo);
               AtRGBA  rgba = ((AtRGBA*)bucket_data)[in_idx]; 

               // Flip vertically
               AtInt targetX = i - minx;
               AtInt targetY = bucket_size_y - (j - miny) - 1;

               AtUInt out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               // JS: Commented this out, makes no sense.
               //AiRGBAClamp(rgba, rgba, 0, 1);
               AiRGBAGamma(&rgba, s_outputDriverData.gamma);

               pixel->r = rgba.r * 255;
               pixel->g = rgba.g * 255;
               pixel->b = rgba.b * 255;
               pixel->a = rgba.a * 255;
            }
         }
         break;
      }
   }

   CDisplayUpdateMessage msg(MSG_BUCKET_UPDATE, minx, miny, maxx, maxy, pixels);

   s_displayUpdateQueue.push(msg);
}


driver_close
{
   CDisplayUpdateMessage msg;
   msg.msgType = MSG_IMAGE_COMPLETE;
   s_displayUpdateQueue.push(msg);

   s_outputDriverData.rendering = FALSE;
}

node_finish
{
   // release the driver
   AiDriverDestroy(node);
}

/// \}


/// \name Render View and Queue processing.
/// \{

void UpdateBucket(const AtBBox2& bucketRect, RV_PIXEL* pixels, const bool refresh)
{
   // Flip vertically
   AtInt   miny = s_outputDriverData.imageHeight - bucketRect.maxy - 1;
   AtInt   maxy = s_outputDriverData.imageHeight - bucketRect.miny - 1;

   MRenderView::updatePixels(bucketRect.minx, bucketRect.maxx, miny, maxy, pixels);
   if (refresh) MRenderView::refresh(bucketRect.minx, bucketRect.maxx, miny, maxy);

   delete[] pixels;
}

// Please note: this function flips the Y as the resulting
// image is for use with MImage.
void CopyBucketToBuffer(float * to_pixels,
                         const CDisplayUpdateMessage & bucket)
{
   const AtInt bucket_size_x = bucket.bucketRect.maxx - bucket.bucketRect.minx + 1;
   const AtInt bucket_size_y = bucket.bucketRect.maxy - bucket.bucketRect.miny + 1;

   RV_PIXEL * from = bucket.pixels;
   const char num_channels(4);
   for(int y(0); y < bucket_size_y; ++y)
   {
      // Invert Y.
      const int oy = (bucket_size_y - y) + bucket.bucketRect.miny - 1;
      for(int x(0); x < bucket_size_x; ++x)
      {
         // Offset into the buffer.
         const int ox = (x + bucket.bucketRect.minx);
         const int to_idx = (oy * s_outputDriverData.imageWidth + ox) * num_channels;
         to_pixels[to_idx+0]= from->r;
         to_pixels[to_idx+1]= from->g;
         to_pixels[to_idx+2]= from->b;
         to_pixels[to_idx+3]= from->a;
         ++from;
      }
   }

   delete [] bucket.pixels;
}

// Create an MImage from the buffer/queue rendered from Arnold.
// The resulting image will be flipped, just how Maya likes it.
bool DisplayUpdateQueueToMImage(MImage & image)
{
   image.create(s_outputDriverData.imageWidth,
                s_outputDriverData.imageHeight,
                4,                               // RGBA
                MImage::kFloat);                // Has to be for swatches it seems.

   CDisplayUpdateMessage msg;
   
   while(!s_displayUpdateQueue.isEmpty())
   {
      if (s_displayUpdateQueue.pop(msg))
      {
         switch (msg.msgType)
         {
         case MSG_BUCKET_PREPARE:
            continue;
         case MSG_BUCKET_UPDATE:
            CopyBucketToBuffer(image.floatPixels(), msg);
            break;
         case MSG_IMAGE_COMPLETE:
         case MSG_RENDER_DONE:
            return true;
         }
      }
   }
   // If we get here, then we've not got a whole image.
   return false;
}

void InitializeDisplayUpdateQueue()
{
   // Clears the display update queue, in case we had aborted a previous render.
   ClearDisplayUpdateQueue();
   s_start_time = time(NULL);
   s_finishedRendering = false;
}

void FinishedWithDisplayUpdateQueue()
{
   s_finishedRendering = false;

   // Get some data from Arnold before it gets deleted with the universe.
   const AtInt AA_Samples(AiNodeGetInt(AiUniverseGetOptions(), "AA_samples"));
   const AtInt GI_diffuse_samples(AiNodeGetInt(AiUniverseGetOptions(), "GI_diffuse_samples"));
   const AtInt GI_glossy_samples(AiNodeGetInt(AiUniverseGetOptions(), "GI_glossy_samples"));
   const AtInt sss_sample_factor(AiNodeGetInt(AiUniverseGetOptions(), "sss_sample_factor"));

   // Calculate the time taken.
   const time_t elapsed = time(NULL) - s_start_time;
   char command_str[256];
   sprintf(command_str,
            "arnoldIpr -mode finishedIPR -elapsedTime \"%ld:%02ld\" -samplingInfo \"[%d/%d/%d/%d]\" ;",
            elapsed / 60,
            elapsed % 60,
            AA_Samples,
            GI_diffuse_samples,
            GI_glossy_samples,
            sss_sample_factor);
   MGlobal::executeCommandOnIdle(command_str, false);
}

void ClearDisplayUpdateQueue()
{
   s_displayUpdateQueue.reset();
   s_finishedRendering = false;
}

void DisplayUpdateQueueRenderFinished()
{
   CDisplayUpdateMessage msg(MSG_RENDER_DONE);
   s_displayUpdateQueue.push(msg);
   s_finishedRendering = true;
}

bool ProcessUpdateMessage(const bool refresh)
{
   if (s_displayUpdateQueue.waitForNotEmpty(10))
   {
      CDisplayUpdateMessage msg;
      if (s_displayUpdateQueue.pop(msg))
      {
         switch (msg.msgType)
         {
         case MSG_BUCKET_PREPARE:
            // TODO: Implement this...
            break;
         case MSG_BUCKET_UPDATE:
            UpdateBucket(msg.bucketRect, msg.pixels, refresh);
            break;
         case MSG_IMAGE_COMPLETE:
            // Received "end-of-image" message.
            //AiMsgDebug("[mtoa] Got end image");
            break;
         case MSG_RENDER_DONE:
            // Recieved "end-of-rendering" message.
            FinishedWithDisplayUpdateQueue();
            //AiMsgDebug("[mtoa] Got end render message");
            return false;
         }
      }
   }
   
   return true;
}


void ProcessDisplayUpdateQueue()
{
   while(!s_displayUpdateQueue.isEmpty())
   {
      ProcessUpdateMessage(false);
   }
}

bool ProcessSomeOfDisplayUpdateQueue(const bool refresh)
{
   // If we're done, do the whole queue.
   if (s_finishedRendering)
   {
      ProcessDisplayUpdateQueue();
      return false;
   }
   // Otherwise we just to a bit of it.
   else
   {
      return ProcessUpdateMessage(refresh);
   }
}

void ProcessDisplayUpdateQueueWithInterupt(MComputation & comp)
{
   // Break out the loop when we've displayed the last complete image.
   // ProcessUpdateMessage returns false on end of render message.
   // s_finishedRendering = false while rendering, but while rendering
   // is going on, we want to refresh the render view, hence it's negated.
   while(ProcessUpdateMessage(!s_finishedRendering))
   {
      // Break if the user wants out.
      if (comp.isInterruptRequested()) break;
   }
}
/// \}
/// @}

