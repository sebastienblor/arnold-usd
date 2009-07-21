
#include "common/MTBlockingQueue.h"

#include <ai_drivers.h>
#include <ai_critsec.h>
#include <ai_filters.h>
#include <ai_msg.h>
#include <ai_universe.h>

#include <maya/MRenderView.h>

AI_DRIVER_NODE_EXPORT_METHODS(mtoa_driver_mtd);

struct COutputDriverData
{
   AtUInt    imageWidth, imageHeight;
   AtBoolean rendering;
};  // struct COutputDriverData

enum EDisplayUpdateMessageType
{
   MSG_BUCKET_PREPARE,
   MSG_BUCKET_UPDATE,
   MSG_IMAGE_UPDATE
};

// This struct holds the data for a display update message
struct CDisplayUpdateMessage
{
   EDisplayUpdateMessageType msgType;
   AtBBox2                   bucketRect;
   RV_PIXEL*                 pixels;
   bool                      finished;
};  // struct CDisplayUpdateMessage

static CMTBlockingQueue<CDisplayUpdateMessage> s_displayUpdateQueue;
static CEvent                                  s_displayUpdateFinishedEvent;
static COutputDriverData                       s_outputDriverData;

node_parameters
{
}  // node_parameters()


node_initialize
{
   AiDriverInitialize(node, FALSE, NULL);
}  // node_initialize()


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
   if (!s_outputDriverData.rendering)
   {
      s_outputDriverData.imageWidth  = display_window.maxx - display_window.minx + 1;
      s_outputDriverData.imageHeight = display_window.maxy - display_window.miny + 1;
      s_outputDriverData.rendering   = TRUE;
   }
}  // driver_open()


driver_prepare_bucket
{

   CDisplayUpdateMessage   msg;

   msg.msgType = MSG_BUCKET_PREPARE;
   msg.bucketRect.minx = bucket_xo;
   msg.bucketRect.miny = bucket_yo;
   msg.bucketRect.maxx = bucket_xo + bucket_size_x - 1;
   msg.bucketRect.maxy = bucket_yo + bucket_size_y - 1;
   msg.pixels          = NULL;
   msg.finished        = false;

   s_displayUpdateQueue.push(msg);

}  // driver_prepare_bucket()


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
               AtRGB  rgb = ((AtRGB* )bucket_data)[in_idx]; 

               // Flip vertically
               AtInt targetX = i - minx;
               AtInt targetY = bucket_size_y - (j - miny) - 1;

               AtUInt out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               pixel->r = rgb.r * 255;
               pixel->g = rgb.g * 255;
               pixel->b = rgb.b * 255;
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
               AtRGBA  rgba = ((AtRGBA* )bucket_data)[in_idx]; 

               // Flip vertically
               AtInt targetX = i - minx;
               AtInt targetY = bucket_size_y - (j - miny) - 1;

               AtUInt out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               pixel->r = rgba.r * 255;
               pixel->g = rgba.g * 255;
               pixel->b = rgba.b * 255;
               pixel->a = rgba.a * 255;
            }
         }
         break;
      }
   }

   CDisplayUpdateMessage msg;

   msg.msgType         = MSG_BUCKET_UPDATE;
   msg.bucketRect.minx = minx;
   msg.bucketRect.miny = miny;
   msg.bucketRect.maxx = maxx;
   msg.bucketRect.maxy = maxy;
   msg.pixels          = pixels;
   msg.finished        = false;

   s_displayUpdateQueue.push( msg );

}  // driver_write_bucket()

driver_close
{
   CDisplayUpdateMessage msg;

   msg.finished = true;

   s_displayUpdateQueue.push(msg);

   // AJJ: This is a little hack to avoid returning control here, until the display update has finished
   s_displayUpdateFinishedEvent.wait();

   s_outputDriverData.rendering = FALSE;
}  // driver_close()

node_finish
{
   // release the driver
   AiDriverDestroy(node);
}  // node_finish()


void UpdateBucket(const AtBBox2& bucketRect, RV_PIXEL* pixels)
{
   // Flip vertically
   AtInt   miny = s_outputDriverData.imageHeight - bucketRect.maxy - 1;
   AtInt   maxy = s_outputDriverData.imageHeight - bucketRect.miny - 1;

   MRenderView::updatePixels(bucketRect.minx, bucketRect.maxx, miny, maxy, pixels);
   MRenderView::refresh(bucketRect.minx, bucketRect.maxx, miny, maxy);

   delete[] pixels;
} // UpdateBucket()


void InitializeDisplayUpdateQueue()
{

   // This event is used to hold the render thread from releasing buffers after sending the last message.
   s_displayUpdateFinishedEvent.unset();

   // Clears the display update queue, in case we had aborted a previous render.
   s_displayUpdateQueue.reset();

}  // InitializeDisplayUpdateQueue()


void ProcessDisplayUpdateQueue()
{

   while (true)
   {
      if (s_displayUpdateQueue.waitForNotEmpty(10))
      {
         CDisplayUpdateMessage   msg;

         if (s_displayUpdateQueue.pop(msg))
         {
            if (!msg.finished)
            {
               switch (msg.msgType)
               {
               case MSG_BUCKET_PREPARE:
                  // TODO: Implement this...
                  break;
               case MSG_BUCKET_UPDATE:
                  UpdateBucket(msg.bucketRect, msg.pixels);
                  break;
               case MSG_IMAGE_UPDATE:
                  break;
               }
            }
            else
            {
               // Received "end-of-render" message, so get out of the loop
               break;
            }
         }
      }
   }

   // Notify the render thread that we are done with the renderview update
   s_displayUpdateFinishedEvent.set();

}  // ProcessDisplayUpdateQueue()
