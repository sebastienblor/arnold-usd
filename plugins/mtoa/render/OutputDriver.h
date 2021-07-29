#pragma once

#include <ai_drivers.h>
#include <ai_render.h>
#include <ai_msg.h>
#include <ai_universe.h>

#include <vector>
#include <maya/MSelectionList.h>
#include <maya/MImage.h>
#include <maya/MRenderView.h>
#include <maya/MItDag.h>

#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MTimerMessage.h>
#include "common/MTBlockingQueue.h"

// This value has a huge impact on the render view performance.
// A figure too low on linux causes the render view to receive updates too slowly.
// A figure too high on windows causes the GUI to become less responsive.
#ifdef _WIN32
#define DISPLAY_QUEUE_WAIT 5
#else
#define DISPLAY_QUEUE_WAIT 10
#endif




enum EDisplayUpdateMessageType
{
   MSG_RENDER_BEGIN,
   MSG_BUCKET_PREPARE,
   MSG_BUCKET_UPDATE,
   MSG_IMAGE_BEGIN,
   MSG_IMAGE_COMPLETE,
   MSG_RENDER_END
};

// Do not use copy constructor and assignment operator outside
// of a critical section
// (basically do not use them, CMTBlockingQueue uses them)
struct CDisplayUpdateMessage
{

   EDisplayUpdateMessageType msgType;
   AtBBox2                   bucketRect;
   RV_PIXEL*                 pixels;
   unsigned int              imageWidth;
   unsigned int              imageHeight;

   CDisplayUpdateMessage(EDisplayUpdateMessageType msg = MSG_BUCKET_PREPARE,
                           int minx = 0, int miny = 0, int maxx = 0, int maxy = 0,
                           RV_PIXEL* px = NULL,
                           unsigned int width = 0, unsigned int height = 0)
   {
      msgType         = msg;
      bucketRect.minx = minx;
      bucketRect.miny = miny;
      bucketRect.maxx = maxx;
      bucketRect.maxy = maxy;
      pixels          = px;
      imageWidth      = width;
      imageHeight     = height;
   }
};

struct COutputDriverData
{
   COutputDriverData() 
   {
      FPS = 6.0;
      firstOpen = false;
      newRender = false;
      finishedRendering = false;
      AA_Samples = GI_diffuse_samples = GI_specular_samples = GI_transmission_samples = GI_sss_samples = GI_volume_samples = 1;
      start_time = 0;
      swatchPixels = nullptr;
      gamma = 1.f;
      imageWidth = imageHeight = swatchImageWidth = renderedPixels = totalPixels = 1;
      isRegion = isProgressive = rendering = clearBeforeRender = false;
   }


   CMTBlockingQueue<CDisplayUpdateMessage> displayUpdateQueue;
   bool                                    finishedRendering;
   MString                                 camera_name;
   MString                                 layer_name;
   MString                                 panel_name;
   
   int AA_Samples;
   int GI_diffuse_samples;
   int GI_specular_samples;
   int GI_transmission_samples;
   int GI_sss_samples;
   int GI_volume_samples;
   bool firstOpen;
   bool newRender;
   AtMutex driverLock;
   double FPS;
   time_t start_time;   

   std::vector<RV_PIXEL>   oldPixels;
   AtBBox2                 refresh_bbox;
   float*                  swatchPixels;
   float                   gamma;
   unsigned int            imageWidth;
   unsigned int            imageHeight;
   unsigned int            swatchImageWidth;
   unsigned int            renderedPixels;
   unsigned int            totalPixels;
   bool                    isRegion;
   bool                    isProgressive;  
   bool                    rendering;
   bool                    clearBeforeRender;
};

/// Initialize the display queue for a new render.
void InitializeDisplayUpdateQueue(const MString& camera, const MString& layer, const MString& panel, COutputDriverData *data);

/// Process a message on the queue from Arnold.
/// \param refresh the render view is slow to refresh, so pass false if possible.
bool ProcessUpdateMessage(AtNode *);
void UpdateBucket(RV_PIXEL* pixels, int minx, int miny, int maxx, int maxy, const bool refresh);
void RefreshRenderViewBBox(AtNode *);
void CopyBucketToBuffer(COutputDriverData *data,
                        CDisplayUpdateMessage & msg);

/// Clear the queue.
void ClearDisplayUpdateQueue(COutputDriverData *data);
void TransferTilesToRenderView(AtNode *);

