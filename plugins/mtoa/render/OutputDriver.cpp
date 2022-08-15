#include "platform/Platform.h"
#include "common/MTBlockingQueue.h"

#include "render/OutputDriver.h"
#include "session/SessionOptions.h"
#include "session/SessionManager.h"
#include "session/ArnoldRenderSession.h"
#include "utils/ConstantStrings.h"

#include <ai_critsec.h>
#include <ai_drivers.h>
#include <ai_filters.h>
#include <ai_msg.h>
#include <ai_render.h>
#include <ai_universe.h>
#include <ai_metadata.h>

#include <maya/MComputation.h>
#include <maya/MRenderView.h>
#include <maya/MGlobal.h>
#include <maya/MImage.h>
#include <maya/MAnimControl.h>
#include <maya/MFnRenderLayer.h>

#include <time.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static const double FPS = 6.0;
static MString s_last_caption_cmd;
/// \defgroup render_view Render View Output Driver
/// These are the methods that make up the arnold side of the Render View update code.
/// @{

AI_DRIVER_NODE_EXPORT_METHODS(mtoa_driver_mtd);


/// \name Arnold Output Driver.
/// \{
node_parameters
{
   AiParameterFlt ("gamma", 1.0f);
   AiMetaDataSetBool(nentry, "gamma", "maya.hide", true);
   AiParameterBool("progressive", false);
   AiMetaDataSetBool(nentry, "progressive", "maya.hide", true);
   AiParameterPtr("swatch", NULL);
   AiParameterStr("renderSession", "");
   AiMetaDataSetBool(nentry, "swatch", "maya.hide", true);
   AiMetaDataSetStr(nentry, NULL, "maya.translator", "maya");
   AiMetaDataSetStr(nentry, NULL, "maya.attr_prefix", "");
   AiMetaDataSetBool(nentry, NULL, "single_layer_driver", true);
   AiMetaDataSetBool(nentry, NULL, "display_driver", true);
}

node_initialize
{
   COutputDriverData *data = new COutputDriverData();
   AiNodeSetLocalData(node, data);
   data->swatchPixels = (float*)AiNodeGetPtr(node, str::swatch);
   MString cameraName = "";
   AtUniverse *universe = AiNodeGetUniverse(node);
   AtNode* cameraNode = AiUniverseGetCamera(universe);
   if (cameraNode != 0)
      cameraName = AiNodeGetName(cameraNode);

   MString layerName ("");
   MStatus status;

   MObject currentRenderLayerObj = MFnRenderLayer::currentLayer(&status);   
   if (status)
   {
      MFnRenderLayer currentRenderLayer(currentRenderLayerObj, &status);
      if (status)
      {
         layerName = currentRenderLayer.name();
      }
   }

   // MGlobal::executePythonCommand("mtoa.utils.getActiveRenderLayerName()", layerName);

   InitializeDisplayUpdateQueue(cameraName, layerName, "renderView", data);

   AiDriverInitialize(node, false);

   data->firstOpen = true;
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
      case AI_TYPE_VECTOR:
      case AI_TYPE_VECTOR2:
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
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
   data->gamma = AiNodeGetFlt(node, str::gamma);
   data->clearBeforeRender = true ;// FIXME !!! CMayaScene::GetRenderSession()->RenderOptions()->clearBeforeRender();

   void *swatchPtr = AiNodeGetPtr(node, str::swatch);
   if (swatchPtr)
   {
      data->swatchImageWidth = display_window.maxx - display_window.minx + 1;
   } else  
   {
      unsigned int imageWidth  = display_window.maxx - display_window.minx + 1;
      unsigned int imageHeight = display_window.maxy - display_window.miny + 1;

      data->isProgressive = AiNodeGetBool(node, str::progressive);

      if (  (data_window.maxx == display_window.maxx) &&
            (data_window.maxy == display_window.maxy) &&
            (data_window.minx == display_window.minx) &&
            (data_window.miny == display_window.miny) )
      {
         data->isRegion = false;
      }
      else
      {
         data->isRegion = true;
      }

      if (data->firstOpen)
      {
         CDisplayUpdateMessage msg1;
         msg1.msgType = MSG_RENDER_BEGIN;
         msg1.imageWidth = imageWidth;
         msg1.imageHeight = imageHeight;
         data->displayUpdateQueue.push(msg1);
      }

      CDisplayUpdateMessage msg2;
      msg2.msgType = MSG_IMAGE_BEGIN;
      data->displayUpdateQueue.push(msg2);

      //CCritSec::CScopedLock sc(s_driverLock);
      std::lock_guard<AtMutex> guard(data->driverLock);

      if (data->firstOpen)
      {
         std::string renderSessionName = AiNodeGetStr(node, str::renderSession).c_str();
         CArnoldRenderSession *session = (CArnoldRenderSession*)CSessionManager::FindActiveSession(renderSessionName);
         if (session)
         {
            session->SetRenderCallback(TransferTilesToRenderView, node);
         }
      }

      data->firstOpen = false;
      data->newRender = true;
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
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
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
      case AI_TYPE_VECTOR2:
      {
         for (int j = miny; (j <= maxy); ++j)
         {
            for (int i = minx; (i <= maxx); ++i)
            {
               unsigned int in_idx = (j - bucket_yo) * bucket_size_x + (i-bucket_xo);
               AtVector2 vec = ((AtVector2*)bucket_data)[in_idx]; 

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

               //AiRGBAGamma(&rgba, s_outputDriverData.gamma); commenting as it is deprecated now

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
   if (data->swatchPixels != NULL)
      // swatches render on the same thread and provide their own buffer to write to
      CopyBucketToBuffer(data, msg);
   else
      data->displayUpdateQueue.push(msg);
}

driver_prepare_bucket
{
}

driver_write_bucket
{
}


driver_close
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
   CDisplayUpdateMessage msg;
   msg.msgType = MSG_IMAGE_COMPLETE;
   data->displayUpdateQueue.push(msg);
}

node_finish
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);

   std::lock_guard<AtMutex> guard(data->driverLock);
   data->newRender = false;
   CDisplayUpdateMessage msg;
   msg.msgType = MSG_RENDER_END;
   data->displayUpdateQueue.push(msg);

}

/// \}


/// \name Render View and Queue processing.
/// \{

void UpdateBucket(AtNode *node, CDisplayUpdateMessage & msg)
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
   // Flip vertically
   const int miny = data->imageHeight - msg.bucketRect.maxy - 1;
   const int maxy = data->imageHeight - msg.bucketRect.miny - 1;

   // last argument tells the RenderView that these are float pixels
   MRenderView::updatePixels(msg.bucketRect.minx, msg.bucketRect.maxx, miny, maxy,
                             msg.pixels, true);
   
   const unsigned int num_pixels = (unsigned int)((msg.bucketRect.maxx - msg.bucketRect.minx + 1) * (msg.bucketRect.maxy - msg.bucketRect.miny + 1));
   data->renderedPixels += num_pixels;
   
   if (!data->clearBeforeRender)
   {
      unsigned int i = 0;
      for (int y = miny; y <= maxy; ++y)
      {
         const unsigned int yw = y * data->imageWidth;
         for (int x = msg.bucketRect.minx; x <= msg.bucketRect.maxx; ++x)
            data->oldPixels[x + yw] = msg.pixels[i++];
      }
   }
   // Expand the bounding box for the render view refresh in RefreshRenderViewBBox().
   if (msg.bucketRect.minx < data->refresh_bbox.minx)
      data->refresh_bbox.minx = msg.bucketRect.minx;
   
   if (msg.bucketRect.maxx > data->refresh_bbox.maxx)
      data->refresh_bbox.maxx = msg.bucketRect.maxx;
   
   if (miny < data->refresh_bbox.miny)
      data->refresh_bbox.miny = miny;
   
   if (maxy > data->refresh_bbox.maxy)
      data->refresh_bbox.maxy = maxy;

   if (msg.pixels != NULL)
   {
      delete[] msg.pixels;
      msg.pixels = NULL;
   }
}

void RefreshRenderViewBBox(AtNode *node)
{

   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
   MRenderView::refresh(data->refresh_bbox.minx,
                        data->refresh_bbox.maxx,
                        data->refresh_bbox.miny,
                        data->refresh_bbox.maxy);
   int progress = AiMin((int)(100.f * ((float)data->renderedPixels / (float)data->totalPixels)), 100);
   MString cmd;
   cmd += "global string $gMainProgressBar;";
   cmd += "progressBar -edit -progress ";
   cmd += progress;
   cmd += " $gMainProgressBar;";
   MGlobal::executeCommand(cmd, false);
}

// Please note: this function flips the Y as the resulting
// image is for use with MImage.
void CopyBucketToBuffer(COutputDriverData *data,
                        CDisplayUpdateMessage & msg)
{
   float *to_pixels = data->swatchPixels;
   
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
         const int to_idx = (oy * data->swatchImageWidth + ox) * num_channels;
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

void InitializeDisplayUpdateQueue(const MString& camera, const MString& layer, const MString& panel, COutputDriverData *data)
{
   
   // Clears the display update queue, in case we had aborted a previous render.
   ClearDisplayUpdateQueue(data);
   data->start_time = time(NULL);
   data->finishedRendering = false;
   data->camera_name = camera;
   data->layer_name = layer;
   data->panel_name = panel;
}

void RenderBegin(AtNode *node, CDisplayUpdateMessage & msg)
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
   MString commandRet;
   MGlobal::executeCommand("getPanel -scriptType \"renderWindowPanel\"", commandRet);
   if (commandRet == "")
      data->panel_name = "renderView";
   else
      data->panel_name = commandRet;
   // TODO: Implement this...      MStatus status;
   // This is not the most reliable way to get the camera, since it relies on the camera names matching
   // but theoretically, if the camera was exported by mtoa they should match.
   data->imageWidth = msg.imageWidth;
   data->imageHeight = msg.imageHeight;
   
   const unsigned int pixelCount = data->imageWidth * data->imageHeight;
   const static RV_PIXEL blackRVPixel = {0.f, 0.f, 50.f / 255.f, 0.f};
   if (pixelCount != (unsigned int)data->oldPixels.size())
   {
      data->oldPixels.clear();
      data->oldPixels.resize(pixelCount, blackRVPixel);
   }
   else if (data->clearBeforeRender)
   {
      const size_t numOldPixels = data->oldPixels.size();      
      for (size_t i = 0; i < numOldPixels; ++i)
         data->oldPixels[i] = blackRVPixel;
   }
   
   MStatus status;
   MDagPath camera;
   MSelectionList list;
      // FIXME want to get the camera dag path
   AtUniverse * universe = (node) ? AiNodeGetUniverse(node) : nullptr;
   AtNode *cam = AiUniverseGetCamera(universe);
   if (cam)
   {
      AtString camName = (AiNodeLookUpUserParameter(cam, str::dcc_name)) ? 
         AiNodeGetStr(cam, str::dcc_name) : AtString(AiNodeGetName(cam));
      
      list.add(camName.c_str());
      if (list.length() > 0)
         list.getDagPath(0, camera);
      else
         AiMsgError("[mtoa] display driver could not find render camera \"%s\"", camName.c_str());
      
   }
   
   // An alternate solution:
   //       MDagPath camera = CMayaScene::GetRenderSession()->GetCamera();
   status = MRenderView::setCurrentCamera(camera);
   // last arg is immediateFeedback

   
   if (data->isRegion)
   {
      unsigned int left(-1);
      unsigned int right(-1);
      unsigned int bottom(-1);
      unsigned int top(-1);

      status = MRenderView::getRenderRegion(left, right, bottom, top);
      status = MRenderView::startRegionRender(  data->imageWidth,
                                                data->imageHeight,
                                                left,
                                                right,
                                                bottom,
                                                top,
                                                // keep current image (true) or clear (false):
                                                data->isProgressive,
                                                true);
      const unsigned int regionSize = (right - left + 1) * (top - bottom + 1);
      std::vector<RV_PIXEL> regionData;
      regionData.resize(regionSize);
      unsigned int i = 0;
      for (unsigned int y = bottom; y <= top; ++y)
      {
         const unsigned int yw = y * data->imageWidth;
         for (unsigned int x = left; x <= right; ++x)
            regionData[i++] = data->oldPixels[x + yw];        
      }
      MRenderView::updatePixels(left, right, bottom, top, 
                                &regionData[0], true);
      MRenderView::refresh(left, right, bottom, top);
   }
   else
   {
      status = MRenderView::startRender(data->imageWidth,
                                        data->imageHeight,
                                        // keep current image (true) or clear (false):
                                        data->isProgressive,
                                        true);
      MRenderView::updatePixels(0, data->imageWidth - 1, 0, data->imageHeight - 1, 
                                &data->oldPixels[0], true);
      MRenderView::refresh(0, data->imageWidth - 1, 0, data->imageHeight - 1);
   } 

   CHECK_MSTATUS(status);

   data->rendering  = true;
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

void GenerateRenderViewCaptionCmd(AtNode *node, time_t elapsed, unsigned int mem_used, MString& cmd)
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);     
   std::string sessionName = AiNodeGetStr(node, str::renderSession).c_str();
   CArnoldRenderSession *session = (CArnoldRenderSession *)CSessionManager::FindActiveSession(sessionName);
   if (session == nullptr)
      return;

   cmd = "";
   if (data->panel_name != "")
   {
      int renderViewOptionVars[RV_OPTION_VAR_COUNT];
      ReadRenderViewOptionVars(renderViewOptionVars);

      cmd = ("renderWindowEditor -edit -pcaption (\"    (Arnold Renderer)\\n");
      
      if (renderViewOptionVars[RV_SHOW_FRAME_NUMBER])
      {
         const double frame = MAnimControl::currentTime().as(MTime::uiUnit());
         cmd += "Frame: ";
         cmd += int(frame);
         cmd += "    ";
      }

      cmd += "Memory: ";
      cmd += (unsigned int)mem_used;
      cmd += "Mb";
      cmd += "    ";

      cmd += "Sampling: ";
      cmd += "[";
      cmd += data->AA_Samples;
      cmd += "/";
      cmd += data->GI_diffuse_samples;
      cmd += "/";
      cmd += data->GI_specular_samples;
      cmd += "/";
      cmd += data->GI_transmission_samples;
      cmd += "/";
      cmd += data->GI_sss_samples;
      cmd += "/";
      cmd += data->GI_volume_samples;
      cmd += "]";
      cmd += "    ";

      if (renderViewOptionVars[RV_SHOW_RENDER_TIME])
      {
         cmd += "Render Time: ";
         cmd += int(elapsed / 60);
         cmd += ":";
         const int secondsPart = int(elapsed % 60);
         if (secondsPart < 10)
            cmd += "0";
         cmd += secondsPart;
         cmd += "    ";
      }
      
      if (renderViewOptionVars[RV_SHOW_CAMERA_NAME])
      {
         if (data->camera_name != "")
         {
            cmd += "Camera: ";
            cmd += data->camera_name;
            cmd += "    ";
         }
      }

      if (renderViewOptionVars[RV_SHOW_LAYER_NAME])
      {
         if (data->layer_name != "")
         {
            // Get layer display name
            MString layerDisplayName = data->layer_name;
            int scriptExists = 0;
            MGlobal::executeCommand("exists renderLayerDisplayName", scriptExists);
            if (scriptExists)
               MGlobal::executeCommand("renderLayerDisplayName " + data->layer_name, layerDisplayName);
            cmd += "Layer: ";
            cmd += layerDisplayName;
         }
      }

      cmd += "\") " + data->panel_name;
   }

   // Cache the last generated caption command.
   // Needed to override the caption that Maya sets after rendering.
   session->SetRenderViewCaptionCmd(cmd);
}
void RenderEnd(AtNode *node)
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);     

   std::string sessionName = AiNodeGetStr(node, str::renderSession).c_str();
   CArnoldRenderSession *session = (CArnoldRenderSession *)CSessionManager::FindActiveSession(sessionName);
   // clear callbacks
   
   data->driverLock.lock();
   
   if ((data->newRender == false) && session && session->GetRenderCallback() != 0)
   {
      session->SetRenderCallback(nullptr, node);
      ClearDisplayUpdateQueue(data);
   }
   

   
   data->driverLock.unlock();

   data->rendering = false;
   MRenderView::endRender();
}

void ClearDisplayUpdateQueue(COutputDriverData *data)
{
   data->displayUpdateQueue.clear();
   data->finishedRendering = false;
}

void BeginImage(AtNode *node)
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
   MStatus status;
   AtUniverse *universe = (node) ? AiNodeGetUniverse(node) : nullptr;

   AtNode* options = AiUniverseGetOptions(universe);
   data->AA_Samples               = AiNodeGetInt(options, str::AA_samples);
   data->GI_diffuse_samples       = AiNodeGetInt(options, str::GI_diffuse_samples);
   data->GI_specular_samples      = AiNodeGetInt(options, str::GI_specular_samples);
   data->GI_transmission_samples  = AiNodeGetInt(options, str::GI_transmission_samples);
   data->GI_sss_samples           = AiNodeGetInt(options, str::GI_sss_samples);
   data->GI_volume_samples        = AiNodeGetInt(options, str::GI_volume_samples);

   data->start_time = time(NULL);
   if (data->isRegion)
   {
      const int region_min_x = AiNodeGetInt(options, str::region_min_x);
      const int region_min_y = AiNodeGetInt(options, str::region_min_y);
      const int region_max_x = AiNodeGetInt(options, str::region_max_x);
      const int region_max_y = AiNodeGetInt(options, str::region_max_y);
      data->totalPixels = (region_max_x - region_min_x + 1) * (region_max_y - region_min_y + 1);
   }
   else
      data->totalPixels = data->imageWidth * data->imageHeight;
   data->renderedPixels = 0;
   
   MString cmd;
   cmd += "global string $gMainProgressBar;";
   cmd += "progressBar -edit -beginProgress -status \"Arnold Render ...\" -maxValue 100 -progress 0 $gMainProgressBar;";
   MGlobal::executeCommand(cmd, false);
  
}

void EndImage(AtNode *node)
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
 
   // Calculate the time taken.
   const time_t elapsed = time(NULL) - data->start_time;
   // And ram used
   const unsigned int mem_used = (unsigned int)AiMsgUtilGetUsedMemory() / 1024 / 1024;

   // Format a bit of info for the renderview.
   MString captionCmd;
   GenerateRenderViewCaptionCmd(node, elapsed, mem_used, captionCmd);
   if (captionCmd != "")
   {
      MGlobal::executeCommand(captionCmd, false);
   }

   MGlobal::executeCommand("global string $gMainProgressBar; progressBar -edit -endProgress $gMainProgressBar;", false);
}

// return false if render is done
bool ProcessUpdateMessage(AtNode *node)
{
   COutputDriverData *data = (COutputDriverData*)AiNodeGetLocalData(node);
   if (data->displayUpdateQueue.waitForNotEmpty(DISPLAY_QUEUE_WAIT))
   {
      CDisplayUpdateMessage msg;
      if (data->displayUpdateQueue.pop(msg))
      {
         switch (msg.msgType)
         {
         case MSG_RENDER_BEGIN:
            RenderBegin(node, msg);
            break;
         case MSG_BUCKET_PREPARE:
            // TODO: Implement this...
            break;
         case MSG_BUCKET_UPDATE:
            UpdateBucket(node, msg);
            break;
         case MSG_IMAGE_BEGIN:
            BeginImage(node);
            break;
         case MSG_IMAGE_COMPLETE:
            // Received "end-of-image" message.
            EndImage(node);
            break;
         case MSG_RENDER_END:
            // Recieved "end-of-rendering" message.
            RenderEnd(node);
            break;
         }
      }
      return true;
   }
   return false;
}

/* http://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c */

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

AtUInt64 GetTimeMs()
{
#ifdef _WIN32
   FILETIME ft;
   LARGE_INTEGER li;

   GetSystemTimeAsFileTime(&ft);
   li.LowPart = ft.dwLowDateTime;
   li.HighPart = ft.dwHighDateTime;

   AtUInt64 ret = li.QuadPart;
   ret -= 116444736000000000LL;
   ret /= 10000;

   return ret;
#else
   struct timeval tv;
   gettimeofday(&tv, NULL);
   AtUInt64 ret = tv.tv_usec;
   ret /= 1000;
   ret += (tv.tv_sec * 1000);
   return ret;
#endif
}

void TransferTilesToRenderView(AtNode *node)
{
   if (node == nullptr)
      return;
   
   static AtUInt64 past = GetTimeMs();
   unsigned int i = 0;
   while (true)
   {
      ++i;
      if (!ProcessUpdateMessage(node))
         break;
   }

   const AtUInt64 now = GetTimeMs();
   const AtUInt64 mseconds = now - past;
   if ((mseconds > 1000 / FPS) || (mseconds == 0)) // second check just to be sure
   {
      past = now;
      RefreshRenderViewBBox(node);
   }
}


/// \}
/// @}

