#include "platform/Platform.h"
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
#include <maya/MAtomic.h>
#include <maya/MRenderView.h>
#include <maya/MGlobal.h>
#include <maya/MImage.h>

#include <time.h>
time_t s_start_time;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// \defgroup render_view Render View Output Driver
/// These are the methods that make up the arnold side of the Render View update code.
/// @{


#define _gamma  (params[0].FLT)  /**< accessor for driver's gamma parameter */

AI_DRIVER_NODE_EXPORT_METHODS(mtoa_driver_mtd);


struct COutputDriverData
{
   AtBBox2   refresh_bbox;
   float     gamma;
   unsigned int    imageWidth, imageHeight;
   bool rendering;
};

enum EDisplayUpdateMessageType
{
   MSG_BUCKET_PREPARE,
   MSG_BUCKET_UPDATE,
   MSG_IMAGE_COMPLETE,
   MSG_RENDER_DONE
};

// Do not use copy constructor and assignment operator outside
// of a critical section
// (basically do not use them, CMTBlockingQueue uses them)
struct CDisplayUpdateMessage
{

   EDisplayUpdateMessageType msgType;
   AtBBox2                   bucketRect;
   RV_PIXEL*                 pixels;

   CDisplayUpdateMessage(EDisplayUpdateMessageType msg = MSG_BUCKET_PREPARE,
                           int minx = 0, int miny = 0, int maxx = 0, int maxy = 0,
                           RV_PIXEL* px = NULL)
   {
      msgType         = msg;
      bucketRect.minx = minx;
      bucketRect.miny = miny;
      bucketRect.maxx = maxx;
      bucketRect.maxy = maxy;
      pixels          = px;
   }
};

static CMTBlockingQueue<CDisplayUpdateMessage> s_displayUpdateQueue;
static COutputDriverData                       s_outputDriverData;
static bool                                    s_finishedRendering;
static MString                                 s_camera_name;
static MString                                 s_panel_name;

/// \name Arnold Output Driver.
/// \{
node_parameters
{
   AiParameterFLT ("gamma", 1.0f);
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   AtParamValue *params = AiNodeGetParams(node);

   if (!s_outputDriverData.rendering)
   {
      s_outputDriverData.imageWidth  = display_window.maxx - display_window.minx + 1;
      s_outputDriverData.imageHeight = display_window.maxy - display_window.miny + 1;
      s_outputDriverData.gamma       = _gamma;
      s_outputDriverData.rendering   = true;
   }
}

driver_prepare_bucket
{
   CDisplayUpdateMessage   msg(MSG_BUCKET_PREPARE,
                               bucket_xo, bucket_yo,
                               bucket_xo + bucket_size_x - 1, bucket_yo + bucket_size_y - 1,
                               NULL) ;

   s_displayUpdateQueue.push(msg);
}

/// Convert the data to Maya format.
/// driver_write_bucket takes the data from Arnold and converts it to
/// 0-255 instead of 0-1. It also flips it around height.
driver_write_bucket
{
   int         pixel_type;
   const void* bucket_data;

   // get the first AOV layer
   if (!AiOutputIteratorGetNext(iterator, NULL, &pixel_type, &bucket_data))
      return;

   RV_PIXEL* pixels = new RV_PIXEL[bucket_size_x * bucket_size_y];
   int minx = bucket_xo;
   int miny = bucket_yo;
   int maxx = bucket_xo + bucket_size_x - 1;
   int maxy = bucket_yo + bucket_size_y - 1;

   switch(pixel_type)
   {
      case AI_TYPE_RGB:
      {
         for (int j = miny; (j <= maxy); ++j)
         {
            for (int i = minx; (i <= maxx); ++i)
            {
               unsigned int in_idx = (j-bucket_yo)*bucket_size_x + (i-bucket_xo);
               AtRGB  rgb = ((AtRGB*)bucket_data)[in_idx]; 

               // Flip vertically
               int targetX = i - minx;
               int targetY = bucket_size_y - (j - miny) - 1;

               unsigned int out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

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
         for (int j = miny; (j <= maxy); ++j)
         {
            for (int i = minx; (i <= maxx); ++i)
            {
               unsigned int in_idx = (j-bucket_yo)*bucket_size_x + (i-bucket_xo);
               AtRGBA  rgba = ((AtRGBA*)bucket_data)[in_idx]; 

               // Flip vertically
               int targetX = i - minx;
               int targetY = bucket_size_y - (j - miny) - 1;

               unsigned int out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

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

   s_outputDriverData.rendering = false;
}

node_finish
{
   // release the driver
   AiDriverDestroy(node);
}

/// \}


/// \name Render View and Queue processing.
/// \{

void UpdateBucket(CDisplayUpdateMessage & msg, const bool refresh)
{
   // Flip vertically
   const int miny = s_outputDriverData.imageHeight - msg.bucketRect.maxy - 1;
   const int maxy = s_outputDriverData.imageHeight - msg.bucketRect.miny - 1;

   MRenderView::updatePixels(msg.bucketRect.minx, msg.bucketRect.maxx, miny, maxy, msg.pixels);
   if (refresh)
   {
      MRenderView::refresh(msg.bucketRect.minx, msg.bucketRect.maxx, miny, maxy);
   }
   else
   {
      // Expand the bounding box for the render view refresh in RefreshRenderViewBBox().
      if (msg.bucketRect.minx < s_outputDriverData.refresh_bbox.minx)
         s_outputDriverData.refresh_bbox.minx = msg.bucketRect.minx;
      
      if (msg.bucketRect.maxx > s_outputDriverData.refresh_bbox.maxx)
         s_outputDriverData.refresh_bbox.maxx = msg.bucketRect.maxx;
      
      if (miny < s_outputDriverData.refresh_bbox.miny)
         s_outputDriverData.refresh_bbox.miny = miny;
      
      if (maxy > s_outputDriverData.refresh_bbox.maxy)
         s_outputDriverData.refresh_bbox.maxy = maxy;
   }

   if (msg.pixels != NULL)
   {
      delete[] msg.pixels;
      msg.pixels = NULL;
   }
}

void RefreshRenderViewBBox()
{
   MRenderView::refresh(s_outputDriverData.refresh_bbox.minx,
                        s_outputDriverData.refresh_bbox.maxx,
                        s_outputDriverData.refresh_bbox.miny,
                        s_outputDriverData.refresh_bbox.maxy);
}

// Please note: this function flips the Y as the resulting
// image is for use with MImage.
void CopyBucketToBuffer(float * to_pixels,
                        CDisplayUpdateMessage & msg)
{
   const int bucket_size_x = msg.bucketRect.maxx - msg.bucketRect.minx + 1;
   const int bucket_size_y = msg.bucketRect.maxy - msg.bucketRect.miny + 1;

   RV_PIXEL * from = msg.pixels;
   const char num_channels(4);
   for(int y(0); y < bucket_size_y; ++y)
   {
      // Invert Y.
      const int oy = (bucket_size_y - y) + msg.bucketRect.miny - 1;
      for(int x(0); x < bucket_size_x; ++x)
      {
         // Offset into the buffer.
         const int ox = (x + msg.bucketRect.minx);
         const int to_idx = (oy * s_outputDriverData.imageWidth + ox) * num_channels;
         to_pixels[to_idx+0]= from->r;
         to_pixels[to_idx+1]= from->g;
         to_pixels[to_idx+2]= from->b;
         to_pixels[to_idx+3]= from->a;
         ++from;
      }
   }
   if (msg.pixels != NULL)
   {
      delete[] msg.pixels;
      msg.pixels = NULL;
   }
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
            ClearDisplayUpdateQueue();
            return true;
         case MSG_RENDER_DONE:
            ClearDisplayUpdateQueue();
            return true;
         }
      }
   }
   // If we get here, then we've not got a whole image.
   return false;
}

void InitializeDisplayUpdateQueue(const MString camera, const MString panel)
{
   // Clears the display update queue, in case we had aborted a previous render.
   ClearDisplayUpdateQueue();
   s_start_time = time(NULL);
   s_finishedRendering = false;
   s_camera_name = camera;
   s_panel_name = panel;
}

void FinishedWithDisplayUpdateQueue()
{
   // Get some data from Arnold before it gets deleted with the universe.
   const int AA_Samples(AiNodeGetInt(AiUniverseGetOptions(), "AA_samples"));
   const int GI_diffuse_samples(AiNodeGetInt(AiUniverseGetOptions(), "GI_diffuse_samples"));
   const int GI_glossy_samples(AiNodeGetInt(AiUniverseGetOptions(), "GI_glossy_samples"));
   const int sss_sample_factor(AiNodeGetInt(AiUniverseGetOptions(), "sss_sample_factor"));

   // Calculate the time taken.
   const time_t elapsed = time(NULL) - s_start_time;
   // And ram used
   const AtUInt64 mem_used = AiMsgUtilGetUsedMemory() / 1024 / 1024;

   // Format a bit of info for the renderview.
   if (s_panel_name != "")
   {
      MString rvInfo("renderWindowEditor -edit -pcaption (\"    (Arnold Renderer)\\n");
      rvInfo += "Memory: ";
      rvInfo += (unsigned int)mem_used;
      rvInfo += "Mb";

      rvInfo += "    Sampling: ";
      rvInfo += "[";
      rvInfo += AA_Samples;
      rvInfo += "/";
      rvInfo += GI_diffuse_samples;
      rvInfo += "/";
      rvInfo += GI_glossy_samples;
      rvInfo += "/";
      rvInfo += sss_sample_factor;
      rvInfo += "]";

      rvInfo += "    Render Time: ";
      rvInfo += int(elapsed / 60);
      rvInfo += ":";
      rvInfo += int(elapsed % 60);

      if (s_camera_name != "")
      {
         rvInfo += "    Camera: ";
         rvInfo += s_camera_name;
      }

      rvInfo += "\") " + s_panel_name;
      MGlobal::executeCommandOnIdle(rvInfo, false);
   }

   ClearDisplayUpdateQueue();
}

void ClearDisplayUpdateQueue()
{
   s_displayUpdateQueue.clear();
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
            UpdateBucket(msg, refresh);
            break;
         case MSG_IMAGE_COMPLETE:
            // Received "end-of-image" message.
            // AiMsgDebug("[mtoa] Got end image");
            break;
         case MSG_RENDER_DONE:
            // Recieved "end-of-rendering" message.
            // AiMsgDebug("[mtoa] Got end render message");
            FinishedWithDisplayUpdateQueue();           
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

void ProcessDisplayUpdateQueueWithInterupt(MComputation & comp)
{
   // Break out the loop when we've displayed the last complete image.
   // ProcessUpdateMessage returns false on end of render message.
   // s_finishedRendering = false while rendering, but while rendering
   // is going on, we want to refresh the render view, hence it's negated.
   while(ProcessUpdateMessage(!s_finishedRendering))
   {
      // Break if the user wants out.
      if (comp.isInterruptRequested())
      {
         FinishedWithDisplayUpdateQueue();
         break;
      }
   }
}
/// \}
/// @}

