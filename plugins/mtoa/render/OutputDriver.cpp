#include "platform/Platform.h"
#include "common/MTBlockingQueue.h"

#include "render/RenderSession.h"
#include "render/OutputDriver.h"
#include "scene/MayaScene.h"

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
#include <maya/MAnimControl.h>

#include <tbb/tick_count.h>

#include <time.h>
time_t s_start_time;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// \defgroup render_view Render View Output Driver
/// These are the methods that make up the arnold side of the Render View update code.
/// @{

AI_DRIVER_NODE_EXPORT_METHODS(mtoa_driver_mtd);

enum MayaDisplayDriverParams
{
   p_gamma,
   p_progressive,
   p_swatch
};

static CMTBlockingQueue<CDisplayUpdateMessage> s_displayUpdateQueue;
static COutputDriverData                       s_outputDriverData;
static bool                                    s_finishedRendering;
static MString                                 s_camera_name;
static MString                                 s_panel_name;

static int s_AA_Samples;
static int s_GI_diffuse_samples;
static int s_GI_glossy_samples;
static int s_GI_refraction_samples;
static int s_sss_bssrdf_samples;
static int s_volume_indirect_samples;

static bool s_firstOpen = false;
static bool s_newRender = false;

static CCritSec s_driverLock;

static double FPS = 6.0;

/// \name Arnold Output Driver.
/// \{
node_parameters
{
   AiParameterFLT ("gamma", 1.0f);
   AiMetaDataSetBool(mds, "gamma", "maya.hide", true);
   AiParameterBOOL("progressive", false);
   AiMetaDataSetBool(mds, "progressive", "maya.hide", true);
   AiParameterPTR("swatch", NULL);
   AiMetaDataSetBool(mds, "swatch", "maya.hide", true);
   AiMetaDataSetStr(mds, NULL, "maya.translator", "maya");
   AiMetaDataSetStr(mds, NULL, "maya.attr_prefix", "");
   AiMetaDataSetBool(mds, NULL, "single_layer_driver", true);
   AiMetaDataSetBool(mds, NULL, "display_driver", true);
}

node_initialize
{
   s_outputDriverData.swatchPixels = (float*)params[p_swatch].PTR;
   MString cameraName = "";
   AtNode* cameraNode = AiUniverseGetCamera();
   if (cameraNode != 0)
      cameraName = AiNodeGetName(cameraNode);
   InitializeDisplayUpdateQueue(cameraName, "renderView");

   AiDriverInitialize(node, false, NULL);

   s_firstOpen = true;
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
      case AI_TYPE_POINT:
      case AI_TYPE_VECTOR:
      case AI_TYPE_POINT2:
      case AI_TYPE_FLOAT:
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
   s_outputDriverData.gamma = params[p_gamma].FLT;
   s_outputDriverData.clearBeforeRender = CMayaScene::GetRenderSession()->RenderOptions()->clearBeforeRender();

   if (params[p_swatch].PTR == NULL)
   {
      unsigned int imageWidth  = display_window.maxx - display_window.minx + 1;
      unsigned int imageHeight = display_window.maxy - display_window.miny + 1;

      s_outputDriverData.isProgressive = params[p_progressive].BOOL;

      if (  (data_window.maxx == display_window.maxx) &&
            (data_window.maxy == display_window.maxy) &&
            (data_window.minx == display_window.minx) &&
            (data_window.miny == display_window.miny) )
      {
         s_outputDriverData.isRegion = false;
      }
      else
      {
         s_outputDriverData.isRegion = true;
      }

      if (s_firstOpen)
      {
         CDisplayUpdateMessage msg1;
         msg1.msgType = MSG_RENDER_BEGIN;
         msg1.imageWidth = imageWidth;
         msg1.imageHeight = imageHeight;
         s_displayUpdateQueue.push(msg1);
      }

      CDisplayUpdateMessage msg2;
      msg2.msgType = MSG_IMAGE_BEGIN;
      s_displayUpdateQueue.push(msg2);

      CCritSec::CScopedLock sc(s_driverLock);

      if (s_firstOpen)
      {
         if (CRenderSession::GetCallback() == 0)
            CRenderSession::SetCallback(TransferTilesToRenderView);
      }

      s_firstOpen = false;
      s_newRender = true;
   }
   else
   {
      s_outputDriverData.swatchImageWidth = display_window.maxx - display_window.minx + 1;
   }
}

driver_needs_bucket
{
   return true;
}


/// Convert the data to Maya format.
/// driver_write_bucket takes the data from Arnold and converts it to
/// 0-255 instead of 0-1. It also flips it around height.
driver_process_bucket
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
      case AI_TYPE_FLOAT:
      {
         for (int j = miny; (j <= maxy); ++j)
         {
            for (int i = minx; (i <= maxx); ++i)
            {
               unsigned int in_idx = (j - bucket_yo) * bucket_size_x + (i-bucket_xo);
               float flt = ((float*)bucket_data)[in_idx]; 

               // Flip vertically
               int targetX = i - minx;
               int targetY = bucket_size_y - (j - miny) - 1;

               unsigned int out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               pixel->r = flt;
               pixel->g = flt;
               pixel->b = flt;
               pixel->a = 0.f;
            }
         }
         break;
      }
      case AI_TYPE_VECTOR:
      case AI_TYPE_POINT:
      {
         for (int j = miny; (j <= maxy); ++j)
         {
            for (int i = minx; (i <= maxx); ++i)
            {
               unsigned int in_idx = (j - bucket_yo) * bucket_size_x + (i-bucket_xo);
               AtVector vec = ((AtVector*)bucket_data)[in_idx]; 

               // Flip vertically
               int targetX = i - minx;
               int targetY = bucket_size_y - (j - miny) - 1;

               unsigned int out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               pixel->r = vec.x;
               pixel->g = vec.y;
               pixel->b = vec.z;
               pixel->a = 0.f;
            }
         }
         break;
      }
      case AI_TYPE_POINT2:
      {
         for (int j = miny; (j <= maxy); ++j)
         {
            for (int i = minx; (i <= maxx); ++i)
            {
               unsigned int in_idx = (j - bucket_yo) * bucket_size_x + (i-bucket_xo);
               AtPoint2 vec = ((AtPoint2*)bucket_data)[in_idx]; 

               // Flip vertically
               int targetX = i - minx;
               int targetY = bucket_size_y - (j - miny) - 1;

               unsigned int out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               pixel->r = vec.x;
               pixel->g = vec.y;
               pixel->b = 0.f;
               pixel->a = 0.f;
            }
         }
         break;
      }
      case AI_TYPE_RGB:
      {
         for (int j = miny; (j <= maxy); ++j)
         {
            for (int i = minx; (i <= maxx); ++i)
            {
               unsigned int in_idx = (j - bucket_yo) * bucket_size_x + (i-bucket_xo);
               AtRGB  rgb = ((AtRGB*)bucket_data)[in_idx]; 

               // Flip vertically
               int targetX = i - minx;
               int targetY = bucket_size_y - (j - miny) - 1;

               unsigned int out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               AiColorGamma(&rgb, s_outputDriverData.gamma);

               pixel->r = rgb.r;
               pixel->g = rgb.g;
               pixel->b = rgb.b;
               pixel->a = 0.f;
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
               unsigned int in_idx = (j - bucket_yo) * bucket_size_x + (i-bucket_xo);
               AtRGBA  rgba = ((AtRGBA*)bucket_data)[in_idx]; 

               // Flip vertically
               int targetX = i - minx;
               int targetY = bucket_size_y - (j - miny) - 1;

               unsigned int out_idx = targetY * bucket_size_x + targetX;
               RV_PIXEL* pixel = &pixels[out_idx];

               AiRGBAGamma(&rgba, s_outputDriverData.gamma);

               pixel->r = rgba.r;
               pixel->g = rgba.g;
               pixel->b = rgba.b;
               pixel->a = rgba.a;
            }
         }
         break;
      }
   }

   CDisplayUpdateMessage msg(MSG_BUCKET_UPDATE, minx, miny, maxx, maxy, pixels);
   if (s_outputDriverData.swatchPixels != NULL)
      // swatches render on the same thread and provide their own buffer to write to
      CopyBucketToBuffer(s_outputDriverData.swatchPixels, msg);
   else
      s_displayUpdateQueue.push(msg);
}

driver_prepare_bucket
{
}

driver_write_bucket
{
}


driver_close
{
   CDisplayUpdateMessage msg;
   msg.msgType = MSG_IMAGE_COMPLETE;
   s_displayUpdateQueue.push(msg);
}

node_finish
{
   CCritSec::CScopedLock sc(s_driverLock);
   s_newRender = false;
   CDisplayUpdateMessage msg;
   msg.msgType = MSG_RENDER_END;
   s_displayUpdateQueue.push(msg);

   // release the driver
   AiDriverDestroy(node);
}

/// \}


/// \name Render View and Queue processing.
/// \{

void UpdateBucket(CDisplayUpdateMessage & msg)
{
   // Flip vertically
   const int miny = s_outputDriverData.imageHeight - msg.bucketRect.maxy - 1;
   const int maxy = s_outputDriverData.imageHeight - msg.bucketRect.miny - 1;

   // last argument tells the RenderView that these are float pixels
   MRenderView::updatePixels(msg.bucketRect.minx, msg.bucketRect.maxx, miny, maxy,
                             msg.pixels, true);
   
   const unsigned int num_pixels = (unsigned int)((msg.bucketRect.maxx - msg.bucketRect.minx + 1) * (msg.bucketRect.maxy - msg.bucketRect.miny + 1));
   s_outputDriverData.renderedPixels += num_pixels;
   
   if (!s_outputDriverData.clearBeforeRender)
   {
      unsigned int i = 0;
      for (int y = miny; y <= maxy; ++y)
      {
         const unsigned int yw = y * s_outputDriverData.imageWidth;
         for (int x = msg.bucketRect.minx; x <= msg.bucketRect.maxx; ++x)
            s_outputDriverData.oldPixels[x + yw] = msg.pixels[i++];
      }
   }
   // Expand the bounding box for the render view refresh in RefreshRenderViewBBox().
   if (msg.bucketRect.minx < s_outputDriverData.refresh_bbox.minx)
      s_outputDriverData.refresh_bbox.minx = msg.bucketRect.minx;
   
   if (msg.bucketRect.maxx > s_outputDriverData.refresh_bbox.maxx)
      s_outputDriverData.refresh_bbox.maxx = msg.bucketRect.maxx;
   
   if (miny < s_outputDriverData.refresh_bbox.miny)
      s_outputDriverData.refresh_bbox.miny = miny;
   
   if (maxy > s_outputDriverData.refresh_bbox.maxy)
      s_outputDriverData.refresh_bbox.maxy = maxy;

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
   int progress = MIN((int)(100.f * ((float)s_outputDriverData.renderedPixels / (float)s_outputDriverData.totalPixels)), 100);
   MString cmd;
   cmd += "global string $gMainProgressBar;";
   cmd += "progressBar -edit -progress ";
   cmd += progress;
   cmd += " $gMainProgressBar;";
   MGlobal::executeCommand(cmd, false);
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
         const int to_idx = (oy * s_outputDriverData.swatchImageWidth + ox) * num_channels;
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

void InitializeDisplayUpdateQueue(const MString camera, const MString panel)
{
   // Clears the display update queue, in case we had aborted a previous render.
   ClearDisplayUpdateQueue();
   s_start_time = time(NULL);
   s_finishedRendering = false;
   s_camera_name = camera;
   s_panel_name = panel;
}

void RenderBegin(CDisplayUpdateMessage & msg)
{
   MString commandRet;
   MGlobal::executeCommand("getPanel -scriptType \"renderWindowPanel\"", commandRet);
   if (commandRet == "")
      s_panel_name = "renderView";
   else
      s_panel_name = commandRet;
   // TODO: Implement this...      MStatus status;
   // This is not the most reliable way to get the camera, since it relies on the camera names matching
   // but theoretically, if the camera was exported by mtoa they should match.
   s_outputDriverData.imageWidth = msg.imageWidth;
   s_outputDriverData.imageHeight = msg.imageHeight;
   
   const unsigned int pixelCount = s_outputDriverData.imageWidth * s_outputDriverData.imageHeight;
   const static RV_PIXEL blackRVPixel = {0.f, 0.f, 50.f / 255.f, 0.f};
   if (pixelCount != (unsigned int)s_outputDriverData.oldPixels.size())
   {
      s_outputDriverData.oldPixels.clear();
      s_outputDriverData.oldPixels.resize(pixelCount, blackRVPixel);
   }
   else if (s_outputDriverData.clearBeforeRender)
   {
      const size_t numOldPixels = s_outputDriverData.oldPixels.size();      
      for (size_t i = 0; i < numOldPixels; ++i)
         s_outputDriverData.oldPixels[i] = blackRVPixel;
   }
   
   MStatus status;
   const char* camName = AiNodeGetName(AiUniverseGetCamera());
   MDagPath camera;
   MSelectionList list;
   list.add(camName);
   if (list.length() > 0)
      list.getDagPath(0, camera);
   else if (camName)
      AiMsgError("[mtoa] display driver could not find render camera \"%s\"", camName);
   // An alternate solution:
   //       MDagPath camera = CMayaScene::GetRenderSession()->GetCamera();
   status = MRenderView::setCurrentCamera(camera);
   // last arg is immediateFeedback

   if (s_outputDriverData.isRegion)
   {
      unsigned int left(-1);
      unsigned int right(-1);
      unsigned int bottom(-1);
      unsigned int top(-1);

      status = MRenderView::getRenderRegion(left, right, bottom, top);
      status = MRenderView::startRegionRender(  s_outputDriverData.imageWidth,
                                                s_outputDriverData.imageHeight,
                                                left,
                                                right,
                                                bottom,
                                                top,
                                                // keep current image (true) or clear (false):
                                                s_outputDriverData.isProgressive,
                                                true);
      const unsigned int regionSize = (right - left + 1) * (top - bottom + 1);
      std::vector<RV_PIXEL> regionData;
      regionData.resize(regionSize);
      unsigned int i = 0;
      for (unsigned int y = bottom; y <= top; ++y)
      {
         const unsigned int yw = y * s_outputDriverData.imageWidth;
         for (unsigned int x = left; x <= right; ++x)
            regionData[i++] = s_outputDriverData.oldPixels[x + yw];        
      }
      MRenderView::updatePixels(left, right, bottom, top, 
                                &regionData[0], true);
      MRenderView::refresh(left, right, bottom, top);
   }
   else
   {
      status = MRenderView::startRender(s_outputDriverData.imageWidth,
                                        s_outputDriverData.imageHeight,
                                        // keep current image (true) or clear (false):
                                        s_outputDriverData.isProgressive,
                                        true);
      MRenderView::updatePixels(0, s_outputDriverData.imageWidth - 1, 0, s_outputDriverData.imageHeight - 1, 
                                &s_outputDriverData.oldPixels[0], true);
      MRenderView::refresh(0, s_outputDriverData.imageWidth - 1, 0, s_outputDriverData.imageHeight - 1);
   } 

   CHECK_MSTATUS(status);

   s_outputDriverData.rendering  = true;
}

enum RenderViewOptionVars{
   RV_SHOW_FRAME_NUMBER = 0,
   RV_SHOW_RENDER_TIME,
   RV_SHOW_CAMERA_NAME,
   RV_SHOW_LAYER_NAME,
   RV_SHOW_RENDER_TARGET_NAME,
   RV_OPTION_VAR_COUNT
};

static void ReadRenderViewOptionVars(int* optionVars)
{
   for (int i = 0; i < RV_OPTION_VAR_COUNT; ++i) optionVars[i] = 0;
   int optionVarValue = 0;
   bool exists = false;
   optionVarValue = MGlobal::optionVarIntValue("renderViewShowFrameNumber", &exists);
   if (exists)
      optionVars[RV_SHOW_FRAME_NUMBER] = optionVarValue;
   optionVarValue = MGlobal::optionVarIntValue("renderViewShowRenderTime", &exists);
   if (exists)
      optionVars[RV_SHOW_RENDER_TIME] = optionVarValue;
   optionVarValue = MGlobal::optionVarIntValue("renderViewShowCameraName", &exists);
   if (exists)
      optionVars[RV_SHOW_CAMERA_NAME] = optionVarValue;
   optionVarValue = MGlobal::optionVarIntValue("renderViewShowLayerName", &exists);
   if (exists)
      optionVars[RV_SHOW_LAYER_NAME] = optionVarValue;
   optionVarValue = MGlobal::optionVarIntValue("renderViewShowRenderTargetName", &exists);
   if (exists)
      optionVars[RV_SHOW_RENDER_TARGET_NAME] = optionVarValue;
}

void RenderEnd()
{
   // Calculate the time taken.
   const time_t elapsed = time(NULL) - s_start_time;
   // And ram used
   const AtUInt64 mem_used = AiMsgUtilGetUsedMemory() / 1024 / 1024;
   int renderViewOptionVars[RV_OPTION_VAR_COUNT];
   ReadRenderViewOptionVars(renderViewOptionVars);

   // Format a bit of info for the renderview.
   if (s_panel_name != "")
   {
      MString rvInfo("renderWindowEditor -edit -pcaption (\"    (Arnold Renderer)\\n");
      
      const double frame = MAnimControl::currentTime().as(MTime::uiUnit());
      if (renderViewOptionVars[RV_SHOW_FRAME_NUMBER])
      {
         rvInfo += "Frame: ";
         rvInfo += int(frame);
         rvInfo += "    ";
      }      
      
      rvInfo += "Memory: ";
      rvInfo += (unsigned int)mem_used;
      rvInfo += "Mb";
      rvInfo += "    ";

      rvInfo += "Sampling: ";
      rvInfo += "[";
      rvInfo += s_AA_Samples;
      rvInfo += "/";
      rvInfo += s_GI_diffuse_samples;
      rvInfo += "/";
      rvInfo += s_GI_glossy_samples;
      rvInfo += "/";
      rvInfo += s_GI_refraction_samples;
      rvInfo += "/";
      rvInfo += s_sss_bssrdf_samples;
      rvInfo += "/";
      rvInfo += s_volume_indirect_samples;
      rvInfo += "]";
      rvInfo += "    ";

      if (renderViewOptionVars[RV_SHOW_RENDER_TIME])
      {
         rvInfo += "Render Time: ";
         rvInfo += int(elapsed / 60);
         rvInfo += ":";
         const int secondsPart = int(elapsed % 60);
         if (secondsPart < 10)
            rvInfo += "0";
         rvInfo += secondsPart;
         rvInfo += "    ";
      }
      
      if (renderViewOptionVars[RV_SHOW_CAMERA_NAME])
      {
         if (s_camera_name != "")
         {
            rvInfo += "Camera: ";
            rvInfo += s_camera_name;
         }
      }

      rvInfo += "\") " + s_panel_name;
      MGlobal::executeCommandOnIdle(rvInfo, false);
   }

   // clear callbacks
   s_driverLock.lock();
   if ((s_newRender == false) && (CRenderSession::GetCallback() != 0))
   {
      CRenderSession::ClearCallback();
      ClearDisplayUpdateQueue();
   }
   s_driverLock.unlock();

   s_outputDriverData.rendering = false;
   MRenderView::endRender();
}

void ClearDisplayUpdateQueue()
{
   s_displayUpdateQueue.clear();
   s_finishedRendering = false;
}

void BeginImage()
{
   MStatus status;

   AtNode* options = AiUniverseGetOptions();
   s_AA_Samples               = AiNodeGetInt(options, "AA_samples");
   s_GI_diffuse_samples       = AiNodeGetInt(options, "GI_diffuse_samples");
   s_GI_glossy_samples        = AiNodeGetInt(options, "GI_glossy_samples");
   s_GI_refraction_samples    = AiNodeGetInt(options, "GI_refraction_samples");
   s_sss_bssrdf_samples         = AiNodeGetInt(options, "sss_bssrdf_samples");
   s_volume_indirect_samples  = AiNodeGetInt(options, "volume_indirect_samples");

   s_start_time = time(NULL);
   if (s_outputDriverData.isRegion)
   {
      const int region_min_x = AiNodeGetInt(options, "region_min_x");
      const int region_min_y = AiNodeGetInt(options, "region_min_y");
      const int region_max_x = AiNodeGetInt(options, "region_max_x");
      const int region_max_y = AiNodeGetInt(options, "region_max_y");
      s_outputDriverData.totalPixels = (region_max_x - region_min_x + 1) * (region_max_y - region_min_y + 1);
   }
   else
      s_outputDriverData.totalPixels = s_outputDriverData.imageWidth * s_outputDriverData.imageHeight;
   s_outputDriverData.renderedPixels = 0;
   
   MString cmd;
   cmd += "global string $gMainProgressBar;";
   cmd += "progressBar -edit -beginProgress -status \"Arnold Render ...\" -maxValue 100 -progress 0 $gMainProgressBar;";
   MGlobal::executeCommand(cmd, false);
}

void EndImage()
{
   // Calculate the time taken.
   const time_t elapsed = time(NULL) - s_start_time;
   // And ram used
   const AtUInt64 mem_used = AiMsgUtilGetUsedMemory() / 1024 / 1024;

   // Format a bit of info for the renderview.
   if (s_panel_name != "")
   {
      int renderViewOptionVars[RV_OPTION_VAR_COUNT];
      ReadRenderViewOptionVars(renderViewOptionVars);
      MString rvInfo("renderWindowEditor -edit -pcaption (\"    (Arnold Renderer)\\n");
      
      const double frame = MAnimControl::currentTime().as(MTime::uiUnit());
      if (renderViewOptionVars[RV_SHOW_FRAME_NUMBER])
      {
         rvInfo += "Frame: ";
         rvInfo += int(frame);
         rvInfo += "    ";
      }
      
      rvInfo += "Memory: ";
      rvInfo += (unsigned int)mem_used;
      rvInfo += "Mb";
      rvInfo += "    ";

      rvInfo += "Sampling: ";
      rvInfo += "[";
      rvInfo += s_AA_Samples;
      rvInfo += "/";
      rvInfo += s_GI_diffuse_samples;
      rvInfo += "/";
      rvInfo += s_GI_glossy_samples;
      rvInfo += "/";
      rvInfo += s_GI_refraction_samples;
      rvInfo += "/";
      rvInfo += s_sss_bssrdf_samples;
      rvInfo += "/";
      rvInfo += s_volume_indirect_samples;
      rvInfo += "]";
      rvInfo += "    ";

      if (renderViewOptionVars[RV_SHOW_RENDER_TIME])
      {
         rvInfo += "Render Time: ";
         rvInfo += int(elapsed / 60);
         rvInfo += ":";
         const int secondsPart = int(elapsed % 60);
         if (secondsPart < 10)
            rvInfo += "0";
         rvInfo += secondsPart;
         rvInfo += "    ";
      }
      
      if (renderViewOptionVars[RV_SHOW_CAMERA_NAME])
      {
         if (s_camera_name != "")
         {
            rvInfo += "Camera: ";
            rvInfo += s_camera_name;
         }
      }

      rvInfo += "\") " + s_panel_name;
      MGlobal::executeCommandOnIdle(rvInfo, false);
   }

   MGlobal::executeCommand("global string $gMainProgressBar; progressBar -edit -endProgress $gMainProgressBar;", false);
}

// return false if render is done
bool ProcessUpdateMessage()
{
   if (s_displayUpdateQueue.waitForNotEmpty(DISPLAY_QUEUE_WAIT))
   {
      CDisplayUpdateMessage msg;
      if (s_displayUpdateQueue.pop(msg))
      {
         switch (msg.msgType)
         {
         case MSG_RENDER_BEGIN:
            RenderBegin(msg);
            break;
         case MSG_BUCKET_PREPARE:
            // TODO: Implement this...
            break;
         case MSG_BUCKET_UPDATE:
            UpdateBucket(msg);
            break;
         case MSG_IMAGE_BEGIN:
            BeginImage();
            break;
         case MSG_IMAGE_COMPLETE:
            // Received "end-of-image" message.
            EndImage();
            break;
         case MSG_RENDER_END:
            // Recieved "end-of-rendering" message.
            RenderEnd();
            break;
         }
      }
      return true;
   }
   return false;
}

void TransferTilesToRenderView()
{
   static tbb::tick_count past = tbb::tick_count::now(); // this will probably enforce a refresh at the first time... but not a big problem
   unsigned int i = 0;
   while (true)
   {
      ++i;
      if (!ProcessUpdateMessage())
         break;
   }
   tbb::tick_count now = tbb::tick_count::now();
   const double seconds = (now - past).seconds();
   if ((seconds > 1.0 / FPS) || (seconds < 0.0)) // second check just to be sure
   {
      past = now;
      RefreshRenderViewBBox();
   }
}

/// \}
/// @}

