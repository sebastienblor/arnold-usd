/*
 * Windows window and event handling
 */

#ifndef _WIN64
#include "render_gl_widget.h"
#endif


#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include <deque>
#include <limits>

#ifdef _WIN64
#define _WIN32_WINNT    0x501
#define _WIN32_WINDOWS  0x501

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <intrin.h>
#else
#include <sys/time.h>
#endif

#include "display_gl.h"

#ifdef _WIN64
#include "render_gl_widget.h"
#endif

#include "utility.h"
#include <emmintrin.h>


#include "renderview.h"
#include "render_loop.h"

#include "scene/MayaScene.h"
#include "session/ArnoldSession.h"
//#include "icons/SA_logo_32.xpm"
#include "icons/SA_logo.xpm"

#include "icons/SA_icon_continuous_off.xpm"
#include "icons/SA_icon_continuous_on.xpm"
#include "icons/SA_icon_play.xpm"
#include "icons/SA_icon_region_off.xpm"
#include "icons/SA_icon_region_on.xpm"
#include "icons/SA_icon_stop.xpm"
#include "icons/SA_icon_store.xpm"
//#include "icons/SA_icon_delete_stored.xpm"
#include "icons/SA_icon_transparent.xpm"
#include "icons/SA_icon_lut_off.xpm"
#include "icons/SA_icon_lut_on.xpm"
#include "icons/SA_icon_ch_rgba.xpm"
#include "icons/SA_icon_ch_red.xpm"
#include "icons/SA_icon_ch_green.xpm"
#include "icons/SA_icon_ch_blue.xpm"
#include "icons/SA_icon_ch_alpha.xpm"


#include "manipulators.h"
#include <maya/MQtUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MEventMessage.h>
#include <maya/MImage.h>

#include <sstream>

//#ifdef _MSC_VER
//#pragma warning (disable : 4244)
//#endif
static MCallbackId rvSelectionCb = 0;

/*****************************
 *
 *   CRenderView
 *
 ***************************/

static int menuHeight = 10;
static int toolbarHeight = 15;
static int statusbarHeight = 10;



CRenderView::CRenderView(int w, int h)
{
   m_renderThread = NULL;
   displaySync = NULL;

   m_width = w;
   m_height = h;
   
   m_mainWindow = new CRenderViewMainWindow(MQtUtil::mainWindow(), *this);

   m_mainWindow->setWindowTitle("Arnold Render View");
   m_gl = NULL;
   m_buffer = NULL;
   m_pickedId = NULL;

   Init();

   m_centralWidget = new QWidget(m_mainWindow);
   m_mainWindow->setCentralWidget(m_centralWidget);
   m_centralWidget->resize(w, h + toolbarHeight + statusbarHeight);

   m_mainWindow->InitMenus();
   InitRender(w, h);   
   m_mainWindow->show();
   m_mainWindow->EnableMenus(false);
}


CRenderView::~CRenderView()
{
   FinishRender();

   AiCritSecClose(&displaySync->eventLock);
   AiCritSecClose(&displaySync->lock);
   AiFree(displaySync);
   displaySync = 0;
   AiFree(m_buffer);
   if (!m_storedSnapshots.empty())
   {
      for (size_t i = 0; i < m_storedSnapshots.size(); ++i)
      {
         AiFree(m_storedSnapshots[i].buffer);
         m_storedSnapshots[i].buffer = NULL;
      }
      m_storedSnapshots.clear();
   }
   AiCritSecClose(&m_windowCloseLock);
   AiCritSecClose(&m_pickLock);

   if (rvSelectionCb)
   {
      MMessage::removeCallback(rvSelectionCb);
      rvSelectionCb = 0;
   }
   delete m_mainWindow;
}

void CRenderView::Init()
{

   if (displaySync != NULL)
   {
      AiCritSecClose(&displaySync->eventLock);
      AiCritSecClose(&displaySync->lock);
      AiFree(displaySync);
      displaySync = 0;
   }

   m_colorMode  = COLOR_MODE_RGBA;
   m_showRenderingTiles = false;
   m_regionCrop = false;
   m_statusChanged = false;
   m_statusBarEnabled = true;
   m_statusBarPixelInfo = false;
   m_restoreContinuous = false;
   m_colorMode  = COLOR_MODE_RGBA;
   m_displayID = false;
   m_debugShading = RV_DBG_SHAD_DISABLED;

   AiCritSecInit(&m_windowCloseLock);
   AiCritSecInit(&m_pickLock);

   K_render_window = true;
   K_progressive = true;
   m_continuousUpdates = CMayaScene::GetArnoldSession()->GetContinuousUpdates();


   m_displayedImageIndex = -1; // -1 means the current rendering
   // >= 0 refers to a stored Image

   m_displayedAovIndex = -1; // -1 means we're showing the beauty
   // >0 refers to an AOV

   K_InitGlobalVars();
   if (!m_storedSnapshots.empty())
   {
      for (size_t i = 0; i < m_storedSnapshots.size(); ++i)
      {
         AiFree(m_storedSnapshots[i].buffer);
         m_storedSnapshots[i].buffer = NULL;
      }
      m_storedSnapshots.clear();
   }

   // setup syncing
   DisplaySyncCreate();
}

void CRenderView::InitRender(int w, int h)
{
   m_width = w;
   m_height = h;   

   m_mainWindow->resize(w, h+menuHeight + toolbarHeight + statusbarHeight + 26);

   if (m_gl != NULL)
   {
      m_gl->InitSize(m_width, m_height);
      m_gl->initializeGL();
   }
   else  m_gl = new CRenderGLWidget(m_centralWidget, *this, m_width, m_height);

   if (m_buffer != NULL) AiFree(m_buffer);
   m_buffer = (AtRGBA *)AiMalloc(m_width * m_height * sizeof(AtRGBA));

   const size_t fillsize = m_width * m_height;

   for(size_t i = 0; i < fillsize; i++)
   {
      m_buffer[i] = AI_RGBA_BLACK;
   }

   m_gl->resize(m_width, m_height);
   m_gl->move(0, 0);

   UpdateRenderOptions();

   if (AiNodeLookUpByName("kick_display")) return;

   AtNode *filter;
   AtNode *driver;
   
   AiNodeEntryInstall(AI_NODE_DRIVER, AI_TYPE_NONE, K_DISPLAY_NAME,  NULL, kick_driver_mtd , AI_VERSION);

   driver = AiNode(K_DISPLAY_NAME);
   AiNodeSetStr(driver, "name", "kick_display");

   AtNode *idFilter = AiNode("closest_filter");
   AiNodeSetStr(idFilter, "name", "_renderViewDefault@closest_filter");

   if (K_filter_type[0] != 0)
   {
     filter = AiNode(K_filter_type);
     if (filter == NULL)
     {
        AiMsgWarning("[kick] could not create %s filter for render-window -- using %s instead", K_filter_type, K_DEFAULT_FILTER);
        filter = AiNode(K_DEFAULT_FILTER);
     }
     if (AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(filter), "width"))
        AiNodeSetFlt(filter, "width", K_filter_width);
   }
   else
   {
     filter = AiNode(K_DEFAULT_FILTER);
   }
   AiNodeSetStr(filter, "name", "kick_display_filter");
   AiNodeSetPtr(driver, "userdata", (void *)this);

   K_AA_samples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
   if (K_AA_samples == 0)
   K_AA_samples = 1;

   // handle all other arnold nodes created by this renderView
   // like debug shading
   ManageDebugShading();
}

void CRenderView::UpdateRenderOptions()
{

   AtNode *options = AiUniverseGetOptions();
   // enlarge resolution to capture overscan data
   int bucketSize = AiNodeGetInt(options, "bucket_size");
   int xres = AiNodeGetInt(options, "xres");
   int yres = AiNodeGetInt(options, "yres");

   m_xRes       = xres;
   m_yRes       = yres;
   m_regX       = xres; // check region_min_x ?
   m_regY       = yres;
   m_bucketSize = bucketSize;
   m_minX       = 0; // chec region_min_x ?
   m_minY       = 0;
   
   //unsigned int i;
   //char kick_drv[1024];
   AtArray *outputs;
   
   // We should support the options changes during a session
   // duplicate the old outputs array
   outputs = AiNodeGetArray(options, "outputs");

   if (!m_aovBuffers.empty()) 
   {
      m_aovBuffers.clear();
      m_aovNames.clear();
   }

   m_aovBuffers.reserve(outputs->nelements + 1);
   m_aovNames.reserve(outputs->nelements + 1);

   // list of outputs stored as std::string for 
   // manipulation simplicity
   std::vector<std::string> outputValues;
   outputValues.reserve(outputs->nelements);

   bool foundBeauty = false;
   for (size_t i = 0; i < outputs->nelements; ++i)
   {      
      std::string outputStr = AiArrayGetStr(outputs, i);
      std::string outputName = outputStr.substr(0, outputStr.find_first_of(' '));

      while(outputStr[outputStr.length() - 1] == ' ')
      {
         outputStr = outputStr.substr(0, outputStr.length() - 1);
      }
      size_t lastStep = outputStr.find_last_of(' ');
      outputStr = outputStr.substr(0, lastStep);
      outputStr += " ";
      outputStr += "kick_display";

      // we'll add the ID AOV below
      if (outputName == "ID") continue;

      if (outputName == "RGB" || outputName == "RGBA")
      {
         // this is my beauty
         if (foundBeauty) continue; // only 1 beauty please

         foundBeauty = true;
         outputValues.insert(outputValues.begin(), outputStr);
      } else
      {
         outputValues.push_back(outputStr);

         // add the aov name to the list
         m_aovNames.push_back(outputName);
         AtRGBA *aovBuffer = (AtRGBA *)AiMalloc(xres * yres * sizeof(AtRGBA));
         memset(aovBuffer, 0, xres * yres * sizeof(AtRGBA));
         //add the aov buffer
         m_aovBuffers.push_back(aovBuffer);      
      }
   }

   m_aovNames.push_back("ID");
   AtRGBA *aovBuffer = (AtRGBA *)AiMalloc(xres * yres * sizeof(AtRGBA));
   memset(aovBuffer, 0, xres * yres * sizeof(AtRGBA));
   m_aovBuffers.push_back(aovBuffer);  
   outputValues.push_back("ID INT _renderViewDefault@closest_filter kick_display");

   AtArray *allOutputsArray = AiArrayAllocate(outputValues.size(), 1, AI_TYPE_STRING);
   for (size_t i=0; i < outputValues.size(); i++)
   {
      AiArraySetStr(allOutputsArray, i, outputValues[i].c_str());
   }
   AiNodeSetArray(options, "outputs", allOutputsArray);
   
   // enlarge resolution to capture overscan data
   AtBBox2 fullWindow;
   AtBBox2 displayWindow, dataWindow;
   displayWindow.minx = displayWindow.miny = 0;
   displayWindow.maxx = xres;
   displayWindow.maxy = yres;

   // Region data
   dataWindow = displayWindow;

   AiBBox2Union(fullWindow, displayWindow, dataWindow);

   m_xRes       = fullWindow.maxx - fullWindow.minx + 1;
   m_yRes       = fullWindow.maxy - fullWindow.miny + 1;
   m_regX       = dataWindow.maxx - dataWindow.minx + 1;
   m_regY       = dataWindow.maxy - dataWindow.miny + 1;
   m_bucketSize = bucketSize;
   m_minX       = dataWindow.minx;
   m_minY       = dataWindow.miny;
   
   

   // compute number of buckets we need to render the full frame
   if (displaySync)
   {
      int xRes = dataWindow.maxx - dataWindow.minx + 1;
      int yRes = dataWindow.maxy - dataWindow.miny + 1;
      int nbx = CEIL(xRes / (float) bucketSize);
      int nby = CEIL(yRes / (float) bucketSize);

      displaySync->bucketsLeft = nbx * nby;

      // detect if we restarted rendering at the lowesst resolution
      int AASamples = AiNodeGetInt(options, "AA_samples");
      if (AASamples <= displaySync->previousAASamples)
         displaySync->interrupted = false;
      displaySync->previousAASamples = AASamples;
   }

   // Now that AOVs have changed, 
   // let's update the AOVs menu
   m_mainWindow->PopulateAOVsMenu();

}


void CRenderView::Render()
{
   InterruptRender();

   if (m_renderThread != NULL)
   {      
      K_aborted = true;
      K_wait_for_changes = false;      
      AiThreadWait(m_renderThread);
      AiThreadClose(m_renderThread);

   }
   // make sure m_aovBuffers is resized to the appropriate amount of AOVs

   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   arnoldSession->SetContinuousUpdates(false);

   K_aborted = false;
   m_renderThread = AiThreadCreate(kickWindowRender, (void *)this, AI_PRIORITY_LOW);  

}
bool CRenderView::CanRestartRender() const
{
   if (this == 0) return false;

   return true;

}


void CRenderView::Close()
{
   m_mainWindow->hide();
}

void CRenderView::Show()
{  
   m_mainWindow->show();
}

void CRenderView::FinishRender()
{
   K_aborted = true;
   InterruptRender();

   while (AiRendering()) {Sleep(1000);}
   if (m_renderThread != NULL)
   {  
      K_aborted = true;
      K_wait_for_changes = false;      
      AiThreadWait(m_renderThread);
      AiThreadClose(m_renderThread);
      m_renderThread = NULL;
   }
}

void CRenderView::InterruptRender()
{
   if (AiRendering()) AiRenderInterrupt();
   
   // In theory I should wait until the rendering is finished
   // since the render loop is in another thread.
   // But we should check if it hurts interactivity
   //while (AiRendering()) {sleep(10);}
   
   // I'm already trying to restart the render loop
   // so let's not interfere with it
   if (!K_restartLoop) {K_wait_for_changes = true;}
}
// utilities copied from sync.h and time.h in core

#ifdef _WIN64
AtUInt64 CRenderView::Time()
{
   LARGE_INTEGER PerformanceCount;
   LARGE_INTEGER Frequency;

   QueryPerformanceCounter(&PerformanceCount);
   QueryPerformanceFrequency(&Frequency);

   return (AtUInt64) (PerformanceCount.QuadPart / (Frequency.QuadPart * 1e-6));
}

void CRenderView::Sleep(AtUInt64 usecs)
{
   ::Sleep(usecs / 1000);
}
#else

AtUInt64 CRenderView::Time()
{
   struct timeval tp;
   gettimeofday(&tp, NULL);
   return ((AtUInt64) tp.tv_sec * 1000000) + ((AtUInt64) tp.tv_usec);
}

void CRenderView::Sleep(AtUInt64 usecs)
{
   usleep(usecs);
}

#endif


void CRenderView::SyncPause()
{
   _mm_pause();
}

// This function is called by each of the Render threads
// Beware not to do any Qt UI stuff here
void CRenderView::Draw(AtBBox2 *region)  
{

   bool alreadyInQueue = false;
   AtDisplaySync *sync = displaySync;

   // Lock to update the region
   AiCritSecEnter(&sync->lock);

   alreadyInQueue = sync->waitingDraw;

   if (region)
   {
      // bucket update: enlarge region to be updated on draw
      AiBBox2Union(sync->frontUpdateRegion, sync->frontUpdateRegion, *region);
      displaySync->bucketsLeft--;    
   }
   else if (sync->bucketsLeft == 0 && !sync->interrupted)
   {
      sync->backUpdateRegion.minx = 0;
      sync->backUpdateRegion.miny = 0;
      sync->backUpdateRegion.maxx = m_regX;
      sync->backUpdateRegion.maxy = m_regY;
      sync->backBufferReady = true;
   }

   // immediately draw or wait for timer to run out
   sync->waitingDraw = true;
   
   // if we're displayed a previously stored image
   // we don't want to prevent that we need a draw
   AiCritSecLeave(&sync->lock);

   if ( alreadyInQueue == false) 
   {
      // update() can be called by any threads
      // after that, the paintGL will be called "soon" from the main (UI) thread
      m_gl->update();
   }
}


void CRenderView::DisplaySyncCreate()
{
   if (displaySync != NULL)
   {
      AiCritSecClose(&displaySync->eventLock);
      AiCritSecClose(&displaySync->lock);
      AiFree(displaySync);
      displaySync = 0;
   }

   // Minimum frames per second at which we will always interrupt and redraw
   // regardless of what has been finished
   //FPS_min = 5.0f;
   // Maximum frames per second at which we will interrupt and redraw if a
   // a full frame has finished rendering
   //FPS_max = 30.0f;

   AtDisplaySync *sync = (AtDisplaySync *) AiMalloc(sizeof(AtDisplaySync));

   AtUInt64 now = Time();

   sync->lastDrawTime = now;
   sync->lastInterruptTime = now;

   AiBBox2Init(sync->frontUpdateRegion, std::numeric_limits<int>::max());
   AiBBox2Init(sync->backUpdateRegion, std::numeric_limits<int>::max());
   sync->backBufferReady = false;

   sync->waitingDraw = false;
   sync->waitingInterrupt = false;
   

   sync->bucketsLeft = 0;
   sync->previousAASamples = std::numeric_limits<int>::max();
   sync->interrupted = true;

   AiCritSecInit(&sync->lock);
   AiCritSecInit(&sync->eventLock);

   displaySync = sync;
}


void CRenderView::UpdateRender()
{
   InterruptRender();

   // we need to do some special stuff when one of those render settings
   // parameters have changed

   // bucket / xres / yres / outputs / AASamples
   AtNode *options = AiUniverseGetOptions();
   
   int bucketSize = AiNodeGetInt(options, "bucket_size");
   int xres = AiNodeGetInt(options, "xres");
   int yres = AiNodeGetInt(options, "yres");
   //K_AA_samples = AiNodeGetInt(options, "AA_samples");
   AtArray *outputs = AiNodeGetArray(options, "outputs");

   // should I wait until rendering is really finished ?
   while (AiRendering()) CRenderView::Sleep(1000);

   CMayaScene::UpdateSceneChanges();
   options = AiUniverseGetOptions();
   AtArray *new_outputs = AiNodeGetArray(options, "outputs");
   bool sizeChanged = (xres != AiNodeGetInt(options, "xres") || yres != AiNodeGetInt(options, "yres"));

 
   // Since we're manually modifying the Options AASamples parameter in the Render Loop
   // and in parallel our Translator is setting AASamples from Maya's value
   // it's better to come back to Maya's value here to make sure
   // we're having the right AA samples
   MSelectionList activeList;
   activeList.add(MString("defaultArnoldRenderOptions"));
   MObject optObject;
   activeList.getDependNode(0, optObject);
   MFnDependencyNode fnOpt(optObject);
   K_AA_samples = fnOpt.findPlug("AASamples", true).asInt();
   AiNodeSetInt(options, "AA_samples", K_AA_samples);

   if (bucketSize != AiNodeGetInt(options, "bucket_size") ||
      sizeChanged ||
      outputs != new_outputs)
   {
      if (sizeChanged) InitRender(AiNodeGetInt(options, "xres"), AiNodeGetInt(options, "yres"));

      UpdateRenderOptions();
      m_mainWindow->PopulateAOVsMenu();
   }

   if(m_debugShading == RV_DBG_SHAD_ISOLATE_SELECTED)
   {
      m_shadingManager.IsolateSelected();
   }
   // check if amount of cameras have changed
   RestartRender();
}

void CRenderView::RestartRender()
{
   Show();

   if (!K_progressive)
   {
      // clear the buffer
      memset(m_buffer, 0, m_width * m_height * sizeof(AtRGBA));
      AtRGBA8 *gl_buffer = m_gl->GetBuffer();
      memset(gl_buffer, 0, m_width * m_height * sizeof(AtRGBA8));
      m_gl->ReloadBuffer(m_colorMode);
   }

   K_render_timestamp = Time();
   K_restartLoop = true;
   K_wait_for_changes = false;
   
   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   arnoldSession->SetContinuousUpdates(false);
}

// For "Isolate Selected" debug shading mode,
// we need to receive the events that current
// Selection has changed
void CRenderView::SelectionChangedCallback(void *data)
{
   MSelectionList activeList;
   MGlobal::getActiveSelectionList(activeList);
   if( activeList.isEmpty()) return;

   MObject depNode;
   activeList.getDependNode(0, depNode);
   if (depNode.hasFn(MFn::kTransform))
   {
      // from Transform to Shape
      MDagPath dagPath;
      activeList.getDagPath(0, dagPath);
      depNode = dagPath.child(0);
   }

   MFnDependencyNode nodeFn( depNode );


   //AtNode *selected_shader =  AiNodeLookUpByName (nodeFn.name().asChar());

   CRenderView *rv = ((CRenderView*)data);

   // selection is valid
   rv->m_shadingManager.SetShaderName(nodeFn.name().asChar());
   
   rv->UpdateRender();
}


void CRenderView::SetDebugShading(RenderViewDebugShading d)
{
   if (d == m_debugShading) return;

   InterruptRender();
   while(AiRendering())
   {
      CRenderView::Sleep(1000);
   }

   // if previous shading mode was "isolate selected"
   // I need the remove the "selection change" callback
   // and also restore the specific stuff done by the shading manager
   if (m_debugShading == RV_DBG_SHAD_ISOLATE_SELECTED)
   {
      MMessage::removeCallback(rvSelectionCb);
      rvSelectionCb = 0;
      m_shadingManager.Restore();
   }
   m_debugShading = d; 

   ManageDebugShading();
   UpdateRender();
}

void CRenderView::ManageDebugShading()
{
   // for debug modes like basic, occlusion, wireframe, etc...   
   // we can set options.ignore_shaders to TRUE
   // and edit the default shader (ai_default_reflection_shader)

   // for isolate selected :
   // set the default shader to black, and for all non-visible 
   // shaders set AiNodeSetDisabled(AtNode* node, bool disabled) to false
   // so that they use this default black matte

   // if selection is a light, call AiNodeSetDisabled(AtNode* node, bool disabled)
   // to  all other lights

   // don't forget to restore all this later
   AtNode *options = AiUniverseGetOptions();
   if (m_debugShading == RV_DBG_SHAD_DISABLED)
   {
      AiNodeSetBool(options, "ignore_shaders", false);

   } else if (m_debugShading == RV_DBG_SHAD_ISOLATE_SELECTED)
   {
      AiNodeSetBool(options, "ignore_shaders", false);
      if (rvSelectionCb) MMessage::removeCallback(rvSelectionCb);

      rvSelectionCb = MEventMessage::addEventCallback("SelectionChanged",
                                                  CRenderView::SelectionChangedCallback,
                                                  (void*)this);

      AtNode *utilityShader = AiNodeLookUpByName("ai_default_reflection_shader");
      AiNodeSetStr(utilityShader, "color_mode", "color");
      AiNodeSetStr(utilityShader, "shade_mode", "flat");
      AiNodeSetRGB(utilityShader, "color", 0.f, 0.f, 0.f);

   } else
   {
      AiNodeSetBool(options, "ignore_shaders", true);
      // all shapes will now be shaded with arnold default shader (ai_default_reflection_shader)
      AtNode *utilityShader = AiNodeLookUpByName("ai_default_reflection_shader");
      AiNodeSetRGB(utilityShader, "color", 1.f, 1.f, 1.f);

      switch (m_debugShading)
      {
         case RV_DBG_SHAD_WIREFRAME:
            AiNodeSetStr(utilityShader, "color_mode", "polywire");
            AiNodeSetStr(utilityShader, "shade_mode", "ndoteye");
         break;
         case RV_DBG_SHAD_BASIC:
            AiNodeSetStr(utilityShader, "color_mode", "color");
            AiNodeSetStr(utilityShader, "shade_mode", "ndoteye");
         break;
         case RV_DBG_SHAD_OCCLUSION:
            AiNodeSetStr(utilityShader, "color_mode", "color");
            AiNodeSetStr(utilityShader, "shade_mode", "ambocc");
         break;   
         case RV_DBG_SHAD_UV:
            AiNodeSetStr(utilityShader, "color_mode", "uv");
            AiNodeSetStr(utilityShader, "shade_mode", "flat");
         break;
         case RV_DBG_SHAD_NORMAL:
            AiNodeSetStr(utilityShader, "color_mode", "n");
            AiNodeSetStr(utilityShader, "shade_mode", "flat");
         break;
         case RV_DBG_SHAD_PRIMITIVE_ID:
            AiNodeSetStr(utilityShader, "color_mode", "prims");
            AiNodeSetStr(utilityShader, "shade_mode", "flat");
         break;
         case RV_DBG_SHAD_OBJECT:
            AiNodeSetStr(utilityShader, "color_mode", "obj");
            AiNodeSetStr(utilityShader, "shade_mode", "flat");
         break;
         case RV_DBG_SHAD_BARY:
            AiNodeSetStr(utilityShader, "color_mode", "bary");
            AiNodeSetStr(utilityShader, "shade_mode", "flat");
         break;
         default:
         break;
      }
   }

}

void CRenderView::PickShape(int px, int py)
{
   if (m_displayedImageIndex >= 0) return; // I can't pick a stored image

   int x, y;
   m_gl->Project(px, py, x, y, false);
   
   // picking out of image bounds...
   if (x < 0 || x >= m_width || y < 0 || y >= m_height) return; 

   // check if we are picking the toolbar


   // Get the ID AOV (last one in our list), and get the index value for the given pixel
   const AtRGBA *idAov = (m_aovBuffers.back());
   const AtRGBA &idVal = idAov[x + y * m_width];

   int opId = reinterpret_type<float, int>(idVal.r);

   AiCritSecEnter(&m_pickLock);
   if (m_pickedId) delete m_pickedId;
   m_pickedId = new int(opId);
   AiCritSecLeave(&m_pickLock);

   AtRGBA *displayedBuffer = GetDisplayedBuffer();
   AtRGBA *idBuffer = m_aovBuffers.back();
   
   int ind = 0;
   for (int j = 0; j < m_height; ++j)
   {
      for (int i = 0; i < m_width; ++i, ++ind)
      {
         //int int_id = *((int*)&idBuffer[ind].r);
         int int_id = reinterpret_type<float, int>(idBuffer[ind].r);
         if (int_id != opId) continue;

         FillGLPixel(displayedBuffer[ind], i, j, ind);
      }
   }

   m_gl->ReloadBuffer(m_colorMode);
   Draw();


   // for now we do a linear search over the Shapes in the scene.
   // In the future we might want to store the Maya nodes in a map (per ID) for faster lookup
   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *node = AiNodeIteratorGetNext(iter);
      int nodeId = AiNodeGetInt(node, "id");
      if (nodeId == opId)
      {
         // Selection :
         // If CTRL is pressed, append the selected node to the list
         if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
            MGlobal::selectByName(MString(AiNodeGetName(node)), MGlobal::kAddToList);
         else
            MGlobal::selectByName(MString(AiNodeGetName(node)), MGlobal::kReplaceList);

         break;
      }
   }
   AiNodeIteratorDestroy(iter);
}

void CRenderView::ClearPicking()
{
   // search in the ID Aov all the pixels corresponding to given ID and refresh them
   // (less expensive than calling RefreshGLBuffer())
 
   AiCritSecEnter(&m_pickLock);
   delete m_pickedId;
   m_pickedId = NULL;
   AiCritSecLeave(&m_pickLock);

   RefreshGLBuffer();
}

void CRenderView::ObjectNameChanged(const std::string &newName, const std::string &oldName)
{
   m_shadingManager.ObjectNameChanged(newName, oldName);
}

// this function is being called by renderview_gl::paintGL
// so we're in the main (UI) thread here
// here we can do any stuff related to UI
void CRenderView::CheckSceneUpdates()
{  
   // now that buckets are being painted
   // we can turn the menus as active again
   m_mainWindow->EnableMenus(true);

   if (m_statusBarEnabled)
   {
      if (m_statusChanged)
      {
         RefreshStatusBar();
         m_statusChanged = false;
      }
   }

   if (!m_continuousUpdates) return;

   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   if (arnoldSession == NULL) return;
   if (m_restoreContinuous)
   {
      arnoldSession->SetContinuousUpdates(true);
      m_restoreContinuous = false;
   }
   if (arnoldSession->HasObjectsToUpdate())
   {
      AtUInt64 loopTime = CRenderView::Time();
      // 1 / 15 seconds minimum before restarting a render
      if (loopTime - K_render_timestamp > (AtUInt64)1000000/20)
      {
         K_wait_for_changes = true;
         // setting continuous updates to true
         // will trigger the update
         arnoldSession->SetContinuousUpdates(true);
      }
   }
}

void CRenderView::SaveImage(const std::string &filename)
{
   // Write down the displayed image

   size_t dotPos = filename.find_last_of('.');
   
   std::string extension = (dotPos == std::string::npos) ? "jpg" : filename.substr(dotPos + 1);

   MImage outImg;
   outImg.create(m_width, m_height, 4 );
   
   AtRGBA8 *buffer = m_gl->GetBuffer();
   outImg.setPixels((unsigned char*)buffer, m_width, m_height);
   outImg.verticalFlip();
   MString fname(filename.c_str());
   
   outImg.writeToFile(fname, MString(extension.c_str()));
      

   /*
   // We're using QT stuff, but maybe we should use Maya's instead ?
   QImage outImg(m_width, m_height, QImage::Format_ARGB32 );
   AtRGBA *buffer = GetDisplayedBuffer();

   for (int j = 0; j < m_height; ++j)
   {
      for (int i = 0; i < m_width; ++i, ++buffer)
      {
         AtRGBA &color = *buffer;
         outImg.setPixel(i, j, qRgba(int(255*(color.r)), int(255*(color.g)), int(255*(color.b)), int(255*(color.a))));
      }
   }
   outImg.save(QString(filename.c_str()));

   */
}

void CRenderView::StoreImage()
{
   AtRGBA *storedBuffer = (AtRGBA *)AiMalloc(m_width * m_height * sizeof(AtRGBA));
   memcpy(storedBuffer, m_buffer, m_width * m_height * sizeof(AtRGBA));
   m_storedSnapshots.push_back(StoredSnapshot());
   m_storedSnapshots.back().buffer = storedBuffer;
   m_storedSnapshots.back().width = m_width;
   m_storedSnapshots.back().height = m_height;
   m_storedSnapshots.back().status = m_statusLog;
}

void CRenderView::RefreshGLBuffer()
{
   m_displayID = (m_displayedImageIndex < 0) && (m_displayedAovIndex == (int(m_aovNames.size()) -1));

   AtRGBA *displayedBuffer = GetDisplayedBuffer();

   int width = m_width;
   int height = m_height;
   if (m_displayedImageIndex >= 0 && 
      (m_width != m_storedSnapshots[m_displayedImageIndex].width || m_height != m_storedSnapshots[m_displayedImageIndex].height))
   {
      width = MIN(m_width, m_storedSnapshots[m_displayedImageIndex].width);
      height = MIN(m_height, m_storedSnapshots[m_displayedImageIndex].height);

      memset(m_gl->GetBuffer(), 0, m_width * m_height * sizeof(AtRGBA8));
      for (int j = 0; j < height; ++j)
      {
         int storedIndex = j * m_storedSnapshots[m_displayedImageIndex].width;
         int glIndex = j * m_width;
         for (int i = 0; i < width; ++i, ++glIndex, ++storedIndex)
         {
            FillGLPixel(displayedBuffer[storedIndex], i, j, glIndex);
         }
      }
   } else {
      int ind = 0;
      for (int j = 0; j < height; ++j)
      {
         for (int i = 0; i < width; ++i, ++ind)
         {
            FillGLPixel(displayedBuffer[ind], i, j, ind);
         }
      }
   }

   m_gl->ReloadBuffer(m_colorMode);
   Draw();
   RefreshStatusBar();
}
void CRenderView::RefreshStatusBar(int *mousePosition)
{
   QString statusLog(GetDisplayedStatus().c_str());

   if (statusLog.isEmpty()) return;

   // add the zoom factor
   int zoomPercent = int(m_gl->GetZoomFactor() * 100.f);
   QString zoomStr;
   if (zoomPercent == 100)
   {
      zoomStr ="(1:1) ";
   } else
   {
      
      zoomStr.setNum(zoomPercent);
      zoomStr = "("+zoomStr+"%) ";
   }
   statusLog.insert(statusLog.indexOf('|', 12), zoomStr);



   // add eventual information about debug shading
   if (m_debugShading != RV_DBG_SHAD_DISABLED)
   {
      std::string debugShadingStr;

      switch (m_debugShading)
      {

         case RV_DBG_SHAD_WIREFRAME:
            debugShadingStr = "[WIREFRAME] ";
         break;
         case RV_DBG_SHAD_BASIC:
            debugShadingStr = "[BASIC] ";
         break;
         case RV_DBG_SHAD_OCCLUSION:
            debugShadingStr = "[OCCLUSION] ";
         break;   
         case RV_DBG_SHAD_UV:
            debugShadingStr = "[UV] ";
         break;
         case RV_DBG_SHAD_NORMAL:
            debugShadingStr = "[NORMAL] ";
         break;
         case RV_DBG_SHAD_PRIMITIVE_ID:
            debugShadingStr = "[PRIMITIVE ID] ";
         break;
         case RV_DBG_SHAD_OBJECT:
            debugShadingStr = "[OBJECT] ";
         break;
         case RV_DBG_SHAD_BARY:
            debugShadingStr = "[BARYCENTRIC] ";
         break;
         case RV_DBG_SHAD_ISOLATE_SELECTED:
            debugShadingStr = "["+m_shadingManager.GetShaderName()+"] ";
         break;         
         default:
         break;
      }
      statusLog = debugShadingStr.c_str() + statusLog;
   }

   if (m_statusBarPixelInfo)
   {
      if (mousePosition != NULL)
      {
         // we have mouse position
         // get information about current pixel

         std::stringstream status_pixel;
         status_pixel<<" XY: "<<mousePosition[0]<<","<<mousePosition[1];
         status_pixel<<" RGBA: ("; // Get color from this pixel's buffer color 

         AtRGBA *displayedBuffer = GetDisplayedBuffer();

         const AtRGBA &pixel_rgba = displayedBuffer[mousePosition[0] + m_width*mousePosition[1]];
         status_pixel<<CEIL(pixel_rgba.r * 1000.f) / 1000.f<<", "<<CEIL(pixel_rgba.g * 1000.f) / 1000.f<<", "<<CEIL(pixel_rgba.b * 1000.f) / 1000.f<<", "<<CEIL(pixel_rgba.a * 1000.f) / 1000.f<<")";
         statusLog += QString(status_pixel.str().c_str());

      } else 
      {
         // check if previous log had pixel information and keep it
         std::string previousLog = m_mainWindow->statusBar()->currentMessage().toStdString();
         size_t pixel_pos = previousLog.find("  Pixel");
         if (pixel_pos != std::string::npos)
         {
            previousLog = previousLog.substr(pixel_pos);
            statusLog += QString(previousLog.c_str());
         }
      }
   }
   m_mainWindow->statusBar()->showMessage(statusLog);
}

void CRenderView::ShowPreviousStoredImage()
{
   if (m_storedSnapshots.empty())
   {
      AiMsgError("[mtoa] No Image currently Stored in the Render View");      
      return;
   }
   if (m_displayedImageIndex < 0)
   {
      m_displayedImageIndex = m_storedSnapshots.size() - 1;
   }
   else m_displayedImageIndex--;

   RefreshGLBuffer();
}
void CRenderView::ShowNextStoredImage()
{
   if (m_storedSnapshots.empty())
   {
      AiMsgError("[mtoa] No Image currently Stored in the Render View");      
      return;
   }
   
   if (m_displayedImageIndex >= (int)m_storedSnapshots.size() - 1) m_displayedImageIndex = -1;
   else m_displayedImageIndex++;

   RefreshGLBuffer();
   
}

void CRenderView::DeleteStoredImage()
{
   if (m_storedSnapshots.empty())
   {
      AiMsgError("[mtoa] No Image currently Stored in the Render View");      
      return;
   }
   if (m_displayedImageIndex < 0 || m_displayedImageIndex >= (int)m_storedSnapshots.size()) return; // nothing to delete

   AiFree(m_storedSnapshots[m_displayedImageIndex].buffer);
   m_storedSnapshots[m_displayedImageIndex].buffer = NULL;

   m_storedSnapshots.erase(m_storedSnapshots.begin() + m_displayedImageIndex);
   
   if (m_displayedImageIndex >= (int)m_storedSnapshots.size()) m_displayedImageIndex--;

   RefreshGLBuffer();
}


/**********************************************************************
 *   CRenderViewMainWindows
 *   Inherits from QMainWindow
 */

CRenderViewMainWindow::~CRenderViewMainWindow()
{
   delete m_manipulator;
}

// Overriding QComboBox to receive "mousePressEvent"
class CRvCameraComboBox : public QComboBox{
public:
   CRvCameraComboBox(CRenderViewMainWindow &rv, QWidget*w) : QComboBox(w), m_rv(rv) {}
   virtual ~CRvCameraComboBox() {}
   virtual void mousePressEvent(QMouseEvent * e)
   {
   
      m_rv.UpdateCamerasMenu();
      QComboBox::mousePressEvent(e);
   }

   CRenderViewMainWindow &m_rv;
};

void
CRenderViewMainWindow::InitMenus()
{
   m_activeMenus = false;
   QMenuBar *menubar = menuBar();

   //setWindowIcon(QIcon(QPixmap((const char **) SA_logo_32_xpm)));
   setWindowIcon(QIcon(QPixmap((const char **) SA_logo_xpm)));

   m_toolBar =  addToolBar("Arnold");
   m_toolBar->setAutoFillBackground(true);
   m_toolBar->setPalette(palette());

   statusBar()->show();
   statusBar()->showMessage(" ");
   statusBar()->setAutoFillBackground(true);
   statusBar()->setPalette(palette()) ;

   m_menuFile = menubar->addMenu("File");

   QAction *action;
   action = m_menuFile->addAction("Save Image");
   connect( action, SIGNAL(triggered()), this, SLOT(SaveImage()));

   action->setCheckable(false);
   action->setStatusTip("Save the Image currently being displayed");
   
   m_menuView = menubar->addMenu("View");

   m_actionEnableAovs = m_menuView->addAction("Enable AOVs");
   connect(m_actionEnableAovs, SIGNAL(triggered()), this, SLOT(EnableAOVs()));
   m_actionEnableAovs->setCheckable(true);
   m_actionEnableAovs->setChecked(K_enable_aovs);
   m_actionEnableAovs->setStatusTip("Enable AOVs in the RenderView");
   
   m_menuAovs = new QMenu("AOVs");
   m_menuView->addMenu(m_menuAovs);
   m_menuAovs->setEnabled(K_enable_aovs);

   m_aovsActionGroup = 0;

   m_aovsCombo = new QComboBox(this);
   
   m_toolBar->addWidget(m_aovsCombo);
   m_aovsCombo->setMinimumWidth(90);
   m_toolBar->addSeparator();
   
   PopulateAOVsMenu();

   m_menuView->addSeparator();
   
   m_channelActionGroup = new QActionGroup(this);

   m_channelRedAction = m_menuView->addAction("Red Channel");
   connect(m_channelRedAction, SIGNAL(triggered()), this, SLOT(ShowChannel()));
   m_channelRedAction->setCheckable(true);
   m_channelRedAction->setStatusTip("Display the Red Channel");
   m_channelActionGroup->addAction(m_channelRedAction);

   m_channelGreenAction = m_menuView->addAction("Green Channel");
   connect(m_channelGreenAction, SIGNAL(triggered()), this, SLOT(ShowChannel()));
   m_channelGreenAction->setCheckable(true);
   m_channelGreenAction->setStatusTip("Display the Green Channel");
   m_channelActionGroup->addAction(m_channelGreenAction);

   m_channelBlueAction = m_menuView->addAction("Blue Channel");
   connect(m_channelBlueAction, SIGNAL(triggered()), this, SLOT(ShowChannel()));
   m_channelBlueAction->setCheckable(true);
   m_channelBlueAction->setStatusTip("Display the Blue Channel");
   m_channelActionGroup->addAction(m_channelBlueAction);

   m_channelAlphaAction = m_menuView->addAction("Alpha Channel");
   connect(m_channelAlphaAction, SIGNAL(triggered()), this, SLOT(ShowChannel()));
   m_channelAlphaAction->setCheckable(true);
   m_channelAlphaAction->setStatusTip("Display the Alpha Channel");
   m_channelActionGroup->addAction(m_channelAlphaAction);

   m_channelRgbaAction = m_menuView->addAction("All Channels");
   connect(m_channelRgbaAction, SIGNAL(triggered()), this, SLOT(ShowChannel()));
   m_channelRgbaAction->setCheckable(true);
   m_channelRgbaAction->setChecked(true);
   m_channelRgbaAction->setStatusTip("Display all channels");
   m_channelActionGroup->addAction(m_channelRgbaAction);

   m_menuView->addSeparator();

   action = m_menuView->addAction("Frame All");
   connect(action, SIGNAL(triggered()), this, SLOT(FrameAll()));
   action->setCheckable(false);
   action->setStatusTip("Frame the whole Image to fit the window size");
   action->setShortcut(Qt::Key_A);

   action = m_menuView->addAction("Frame Selection");
   connect(action, SIGNAL(triggered()), this, SLOT(FrameRegion()));
   action->setCheckable(false);
   action->setStatusTip("Frame the Crop Region to fit the window size");
   action->setShortcut(Qt::Key_F);

   action = m_menuView->addAction("Real Size");
   connect(action, SIGNAL(triggered()), this, SLOT(RealSize()));
   action->setCheckable(false);
   action->setStatusTip("Display image with its real size");
   action->setShortcut(Qt::Key_O);

   m_menuView->addSeparator();

   m_actionShowRenderingTiles = m_menuView->addAction("Show Rendering Tiles");
   connect(m_actionShowRenderingTiles, SIGNAL(triggered()), this, SLOT(ShowRenderingTiles()));
   m_actionShowRenderingTiles->setCheckable(true);
   m_actionShowRenderingTiles->setStatusTip("Display the Tiles being rendered");

   m_storeAction = m_menuView->addAction("Store Snapshot");
   connect(m_storeAction, SIGNAL(triggered()), this, SLOT(StoreImage()));
   m_storeAction->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Plus);
   m_storeAction->setStatusTip("Stores a snapshot of the displayed Image in memory");

   action = m_menuView->addAction("Previous Snapshot");
   connect(action, SIGNAL(triggered()), this, SLOT(PreviousStoredImage()));
   action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Left);
   action->setStatusTip("Display the Previous Snapshot Stored in the RenderView");

   action = m_menuView->addAction("Next Snapshot");
   connect(action, SIGNAL(triggered()), this, SLOT(NextStoredImage()));
   action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Right);
   action->setStatusTip("Display the Next Snapshot Stored in the RenderView");

   m_deleteStoredAction = m_menuView->addAction("Delete Snapshot");
   connect(m_deleteStoredAction, SIGNAL(triggered()), this, SLOT(DeleteStoredImage()));
   m_deleteStoredAction->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Minus);
   m_deleteStoredAction->setStatusTip("Delete the Stored Snapshot being currently displayed");

   m_menuView->addSeparator();

   m_lutAction = m_menuView->addAction("LUT / Color Correction");
   connect(m_lutAction, SIGNAL(triggered()), this, SLOT(ColorCorrection()));
   m_lutAction->setCheckable(true);
   m_lutAction->setChecked(false);
   m_lutAction->setStatusTip("Apply Color Correction on the displayed image");

   m_menuView->addSeparator();   

   m_3dManipulation = false;

   m_3dManipulationAction = m_menuView->addAction("3D Manipulation");
   connect(m_3dManipulationAction, SIGNAL(triggered()), this, SLOT(ToggleManipulationMode()));
   m_3dManipulationAction->setCheckable(true);
   m_3dManipulationAction->setChecked(m_3dManipulation);
   m_3dManipulationAction->setStatusTip("Manipulate the rendered image in 3D");
   

   m_menuView->addSeparator();      
   QMenu *statusMenu = new QMenu("Status Bar");
   m_menuView->addMenu(statusMenu);


   m_actionStatusBar = statusMenu->addAction("Show Status Bar");
   connect(m_actionStatusBar, SIGNAL(triggered()), this, SLOT(EnableStatusBar()));
   m_actionStatusBar->setCheckable(true);
   m_actionStatusBar->setChecked(true);
   m_actionStatusBar->setStatusTip("Display the Status Bar");

   m_actionStatusInfo = statusMenu->addAction("Display Pixel Information");
   connect(m_actionStatusInfo, SIGNAL(triggered()), this, SLOT(DisplayPixelInfo()));
   m_actionStatusInfo->setCheckable(true);
   m_actionStatusInfo->setChecked(false);
   m_actionStatusInfo->setStatusTip("Display Pixel Information in the Status Bar");


   m_menuRender = menubar->addMenu("Render");


   m_renderAction = m_menuRender->addAction("Render");
   connect(m_renderAction, SIGNAL(triggered()), this, SLOT(UpdateRender()));
   m_renderAction->setCheckable(false);
   m_renderAction->setShortcut(Qt::Key_F5);
   m_renderAction->setStatusTip("Render / Update");
   
   m_abortAction = m_menuRender->addAction("Abort Render");
   connect(m_abortAction, SIGNAL(triggered()), this, SLOT(AbortRender()));
   m_abortAction->setCheckable(false);
   m_abortAction->setStatusTip("Abort the current Render");
   m_abortAction->setShortcut(Qt::Key_Escape);

   m_actionAutoRefresh = m_menuRender->addAction("Continuous Updates");
   connect(m_actionAutoRefresh, SIGNAL(triggered()), this, SLOT(AutoRefresh()));
   m_actionAutoRefresh->setCheckable(true);
   m_actionAutoRefresh->setChecked(CMayaScene::GetArnoldSession()->GetContinuousUpdates());
   m_actionAutoRefresh->setStatusTip("Automatically update any change in the scene and restart the rendering");

   m_actionProgressiveRefinement = m_menuRender->addAction("Progressive Refinement");
   connect(m_actionProgressiveRefinement, SIGNAL(triggered()), this, SLOT(ProgressiveRefinement()));
   m_actionProgressiveRefinement->setCheckable(true);
   m_actionProgressiveRefinement->setChecked(true);
   m_actionProgressiveRefinement->setStatusTip("Display the Tiles being rendered");

   m_actionCropRegion = m_menuRender->addAction("Crop Region");
   connect(m_actionCropRegion, SIGNAL(triggered()), this, SLOT(CropRegion()));
   m_actionCropRegion->setCheckable(true);
   m_actionCropRegion->setChecked(false);
   m_actionCropRegion->setStatusTip("Allow to drag a Crop Render Region");

   m_menuRender->addSeparator();

   m_menuCamera = new QMenu("Camera");
   m_menuRender->addMenu(m_menuCamera);

   m_camerasActionGroup = 0;

   m_camerasCombo = new CRvCameraComboBox(*this, this);
   m_toolBar->addWidget(m_camerasCombo);
   
   PopulateCamerasMenu();

   connect(m_menuCamera, SIGNAL(aboutToShow()), this, SLOT(ShowCamerasMenu()));

   m_menuRender->addSeparator();
   QMenu *debugShadingMenu = new QMenu("Debug Shading");
   debugShadingMenu->setTearOffEnabled(true);
   m_menuRender->addMenu(debugShadingMenu);
   m_debugShadingActionGroup = new QActionGroup(this);

   m_debugShadingActionDisabled = debugShadingMenu->addAction("Disabled");
   connect(m_debugShadingActionDisabled, SIGNAL(triggered()), this, SLOT(DebugShading()));
   m_debugShadingActionDisabled->setCheckable(true);
   m_debugShadingActionDisabled->setStatusTip("Regular rendering");
   m_debugShadingActionDisabled->setChecked(true);
   m_debugShadingActionGroup->addAction(m_debugShadingActionDisabled);

   debugShadingMenu->addSeparator();

   action = debugShadingMenu->addAction("Basic");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Basic Shading (ndoteye)");
   m_debugShadingActionGroup->addAction(action);

   action = debugShadingMenu->addAction("Occlusion");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Occlusion Rendering");
   m_debugShadingActionGroup->addAction(action);

   action = debugShadingMenu->addAction("Wireframe");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Wireframe Rendering");
   m_debugShadingActionGroup->addAction(action);

   action = debugShadingMenu->addAction("Normal");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Normal display");

   m_debugShadingActionGroup->addAction(action);
   action = debugShadingMenu->addAction("UV");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("UV Display");
   m_debugShadingActionGroup->addAction(action);

   action = debugShadingMenu->addAction("Primitive ID");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Display per-primitive color");
   m_debugShadingActionGroup->addAction(action);

   action = debugShadingMenu->addAction("Barycentric");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Barycentric coordinates");
   m_debugShadingActionGroup->addAction(action);

   action = debugShadingMenu->addAction("Object");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Per-Object Color");
   m_debugShadingActionGroup->addAction(action);

   debugShadingMenu->addSeparator();

   action = debugShadingMenu->addAction("Isolate Selected");
   connect(action, SIGNAL(triggered()), this, SLOT(DebugShading()));
   action->setCheckable(true);
   action->setStatusTip("Isolate the selected shader");
   m_debugShadingActionGroup->addAction(action);


// Now dealing with the ToolButtons   

   setIconSize(QSize(18, 18));

   m_toolBar->addSeparator();
   QString styleButton = "QToolButton { border: none transparent;  border-radius: 1px;   background-color: transparent; min-width: 18;}  QToolButton:checked {    background-color: transparent ;}  QToolButton:flat {    border: none;} QToolButton:default {   border-color: transparent;}";

   m_rgbaButton = new QToolButton(m_toolBar);
   m_toolBar->addWidget(m_rgbaButton);
   QIcon rgbaIcon;
   rgbaIcon.addPixmap(QPixmap((const char **) SA_icon_ch_rgba_xpm));
   m_rgbaButton->setIcon(rgbaIcon);
   m_rgbaButton->setStyleSheet(styleButton);

   connect(m_rgbaButton, SIGNAL(clicked()), this, SLOT(RgbaClicked()));

   m_toolBar->addSeparator();


   QToolButton *renderButton = new QToolButton(m_toolBar);
   renderButton->setDefaultAction(m_renderAction);
   m_toolBar->addWidget(renderButton);
   QIcon renderIcon(QPixmap((const char **) SA_icon_play_xpm));
   renderButton->setIcon(renderIcon);
   m_renderAction->setIcon(renderIcon);
   m_renderAction->setIconVisibleInMenu(false);
   renderButton->setStyleSheet(styleButton);

   QToolButton *abortButton = new QToolButton(m_toolBar);
   abortButton->setDefaultAction(m_abortAction);
   m_toolBar->addWidget(abortButton);
   QIcon abortIcon(QPixmap((const char **) SA_icon_stop_xpm));
   abortButton->setIcon(abortIcon);
   m_abortAction->setIcon(abortIcon);
   m_abortAction->setIconVisibleInMenu(false);
   abortButton->setStyleSheet(styleButton);

   m_toolBar->addSeparator();
   QToolButton *refreshButton = new QToolButton(m_toolBar);
   refreshButton->setDefaultAction(m_actionAutoRefresh);
   m_toolBar->addWidget(refreshButton);
   QIcon refreshIcon;
   refreshIcon.addPixmap(QPixmap((const char **) SA_icon_continuous_on_xpm), QIcon::Normal, QIcon::On);
   refreshIcon.addPixmap(QPixmap((const char **) SA_icon_continuous_off_xpm), QIcon::Normal, QIcon::Off);
   refreshButton->setIcon(refreshIcon);
   m_actionAutoRefresh->setIcon(refreshIcon);
   m_actionAutoRefresh->setIconVisibleInMenu(false);
   refreshButton->setStyleSheet(styleButton);

   QToolButton *regionButton = new QToolButton(m_toolBar);
   regionButton->setDefaultAction(m_actionCropRegion);
   m_toolBar->addWidget(regionButton);
   QIcon regionIcon;
   regionIcon.addPixmap(QPixmap((const char **) SA_icon_region_on_xpm), QIcon::Normal, QIcon::On);
   regionIcon.addPixmap(QPixmap((const char **) SA_icon_region_off_xpm), QIcon::Normal, QIcon::Off);
   regionButton->setIcon(regionIcon);
   m_actionCropRegion->setIcon(regionIcon);
   m_actionCropRegion->setIconVisibleInMenu(false);
   regionButton->setStyleSheet(styleButton);

   QToolButton *lutButton = new QToolButton(m_toolBar);
   lutButton->setDefaultAction(m_lutAction);
   m_toolBar->addWidget(lutButton);
   QIcon lutIcon;
   lutIcon.addPixmap(QPixmap((const char **) SA_icon_lut_on_xpm), QIcon::Normal, QIcon::On);
   lutIcon.addPixmap(QPixmap((const char **) SA_icon_lut_off_xpm), QIcon::Normal, QIcon::Off);
   lutButton->setIcon(lutIcon);
   m_lutAction->setIcon(lutIcon);
   m_lutAction->setIconVisibleInMenu(false);
   lutButton->setStyleSheet(styleButton);

   m_toolBar->addSeparator();
   
   QToolButton *storeButton = new QToolButton(m_toolBar);
   storeButton->setDefaultAction(m_storeAction);
   m_toolBar->addWidget(storeButton);
   QIcon store_icon(QPixmap((const char **) SA_icon_store_xpm));
   storeButton->setIcon(store_icon);
   m_storeAction->setIcon(store_icon);
   m_storeAction->setIconVisibleInMenu(false);
   storeButton->setStyleSheet(styleButton);

   m_storedSlider = new QSlider(Qt::Horizontal, m_toolBar);
   m_storedSlider->setTickInterval(1);
   m_storedSlider->setTickPosition(QSlider::TicksBothSides);
   m_storedSlider->resize(100, 10);
   m_storedSlider->setMaximumWidth(100);
   m_storedSlider->setMaximumHeight(10);

   m_storedSliderAction = m_toolBar->addWidget(m_storedSlider);
   m_storedSlider->resize(100, 10);
   m_storedSlider->setMaximumWidth(100);
   m_storedSlider->setMaximumHeight(10);
   connect(m_storedSlider, SIGNAL(valueChanged(int)), this, SLOT(StoredSliderMoved(int)));

   QToolButton *deleteStoredButton = new QToolButton(m_toolBar);
   deleteStoredButton->setDefaultAction(m_deleteStoredAction);
   m_toolBar->addWidget(deleteStoredButton);
   QIcon deleteStoredIcon = QApplication::style()->standardIcon(QStyle::SP_TrashIcon);
   deleteStoredIcon.addPixmap(QPixmap((const char **) SA_icon_transparent_xpm), QIcon::Disabled, QIcon::Off);

   
   deleteStoredButton->setIcon(deleteStoredIcon);
   m_deleteStoredAction->setIcon(deleteStoredIcon);
   m_deleteStoredAction->setIconVisibleInMenu(false);
   deleteStoredButton->setStyleSheet(styleButton);
   deleteStoredButton->hide();

   UpdateStoredSlider();

   m_renderAction->setEnabled(false);
   m_actionAutoRefresh->setEnabled(false);
   m_actionCropRegion->setEnabled(false);
   m_activeMenus = false;
}

void
CRenderViewMainWindow::SaveImage()
{

   QString filename;
   QFileDialog dialog(this);
   dialog.setFileMode(QFileDialog::AnyFile);
   dialog.setAcceptMode(QFileDialog::AcceptSave);
   dialog.setDefaultSuffix("jpg");
   dialog.setWindowTitle("Save Image As");

   QStringList filters;
   filters <<"Image files (*.bmp *.jpg *.cin *.png *.gif *.als *.rla *.sgi *.tga *.tif *.iff)"
         << "Any files (*)";
   dialog.setNameFilters(filters);

   if (!dialog.exec()) return;

   QStringList selected = dialog.selectedFiles();
   if (selected.empty()) return;
   filename = selected.at(0);

   m_renderView.SaveImage(filename.toStdString());
}

void CRenderViewMainWindow::AbortRender()
{
   m_renderView.InterruptRender();
   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   if (m_renderView.m_continuousUpdates)   arnoldSession->SetContinuousUpdates(true);
}

void CRenderViewMainWindow::ShowRenderingTiles()
{
   m_renderView.SetShowRenderingTiles(m_actionShowRenderingTiles->isChecked());
}


void CRenderViewMainWindow::UpdateRender()
{
   // re-render has been manually triggered
   // let's disable the menus until the first buckets 
   // are painted
   EnableMenus(false);
   m_renderView.UpdateRender();

}

void CRenderViewMainWindow::AutoRefresh()
{
   m_renderView.m_continuousUpdates = m_actionAutoRefresh->isChecked();
   CMayaScene::GetArnoldSession()->SetContinuousUpdates(m_actionAutoRefresh->isChecked());
}

void CRenderViewMainWindow::StoreImage()
{
   m_renderView.StoreImage();
   UpdateStoredSlider();
}

void CRenderViewMainWindow::PreviousStoredImage()
{
   m_renderView.ShowPreviousStoredImage();
   UpdateStoredSlider();

}
void CRenderViewMainWindow::NextStoredImage()
{
   m_renderView.ShowNextStoredImage();
   UpdateStoredSlider();
}
void CRenderViewMainWindow::DeleteStoredImage()
{
   m_renderView.DeleteStoredImage();
   UpdateStoredSlider();  
}

void
CRenderViewMainWindow::StoredSliderMoved(int i)
{
   m_renderView.m_displayedImageIndex = (i == (int)m_renderView.m_storedSnapshots.size()) ? -1 : i;

   m_deleteStoredAction->setVisible(m_renderView.m_displayedImageIndex >= 0);
   m_storeAction->setEnabled(m_renderView.m_displayedImageIndex < 0);
   m_renderView.RefreshGLBuffer();
}

void
CRenderViewMainWindow::UpdateStoredSlider()
{
   if (m_renderView.m_storedSnapshots.empty())
   {
      m_storeAction->setEnabled(true);
      m_storedSliderAction->setVisible(false);
      m_deleteStoredAction->setVisible(false);
      return;
   }

   m_storedSliderAction->setVisible(true);
   m_deleteStoredAction->setVisible(m_renderView.m_displayedImageIndex >= 0);
   m_storeAction->setEnabled(m_renderView.m_displayedImageIndex < 0);

   m_storedSlider->setMinimum(0);
   m_storedSlider->setMaximum(m_renderView.m_storedSnapshots.size());   

   m_storedSlider->setSliderPosition((m_renderView.m_displayedImageIndex < 0) ? m_renderView.m_storedSnapshots.size() : m_renderView.m_displayedImageIndex);
}


void CRenderViewMainWindow::EnableStatusBar()
{
   m_renderView.m_statusBarEnabled = m_actionStatusBar->isChecked();
   if (m_renderView.m_statusBarEnabled) statusBar()->show();
   else statusBar()->hide();

}
void CRenderViewMainWindow::DisplayPixelInfo()
{   
   m_renderView.m_statusBarPixelInfo = m_actionStatusInfo->isChecked();
   m_renderView.m_statusChanged = true;
   setMouseTracking(m_renderView.m_statusBarPixelInfo);
   m_renderView.m_gl->setMouseTracking(m_renderView.m_statusBarPixelInfo);
   m_renderView.m_centralWidget->setMouseTracking(m_renderView.m_statusBarPixelInfo);
}


void CRenderViewMainWindow::ProgressiveRefinement()
{
   K_progressive = m_actionProgressiveRefinement->isChecked();
   m_renderView.UpdateRender();
}


void CRenderViewMainWindow::ShowChannel()
{
   std::string colorMode = m_channelActionGroup->checkedAction()->text().toStdString();
   QIcon channelIcon;
   if (colorMode == "Red Channel")
   {
      m_renderView.m_colorMode = COLOR_MODE_R;
      channelIcon.addPixmap(QPixmap((const char **) SA_icon_ch_red_xpm));
   } else if(colorMode == "Green Channel")
   {
      channelIcon.addPixmap(QPixmap((const char **) SA_icon_ch_green_xpm));
      m_renderView.m_colorMode = COLOR_MODE_G;
   } else if (colorMode == "Blue Channel")
   {
      channelIcon.addPixmap(QPixmap((const char **) SA_icon_ch_blue_xpm));
      m_renderView.m_colorMode = COLOR_MODE_B;
   } else if (colorMode == "Alpha Channel")
   {
      channelIcon.addPixmap(QPixmap((const char **) SA_icon_ch_alpha_xpm));
      m_renderView.m_colorMode = COLOR_MODE_A;
   } else
   {
      m_renderView.m_colorMode = COLOR_MODE_RGBA;
      channelIcon.addPixmap(QPixmap((const char **) SA_icon_ch_rgba_xpm));
   }
      
   m_rgbaButton->setIcon(channelIcon);

   m_renderView.GetGlWidget()->ReloadBuffer(m_renderView.m_colorMode);
}

void CRenderViewMainWindow::ToggleManipulationMode()
{
   m_3dManipulation = m_3dManipulationAction->isChecked();
   FrameAll();
}

void CRenderViewMainWindow::UpdateCamerasMenu()
{
   // we're opening the camera menu, or the toolbar's combo box
   // let's check if the camera list has changed in my scene

   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_CAMERA);
   int camerasCount = 0;
   bool newCamera = false;

   // loop over Arnold Cameras
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *node = AiNodeIteratorGetNext(iter);
      // check if this camera name is already in my combo box list
      if (m_camerasCombo->findText(QString(AiNodeGetName(node))) < 0)
      {
         // this camera wasn't in my list !
         newCamera = true;
         break;
      }
      camerasCount++;
   }

   // also check if the camera amount has changed to
   // track deleted cameras
   if (newCamera || (camerasCount != m_camerasCombo->count()))
   {
      // re-populate the menu AND the combo box
      PopulateCamerasMenu();
   }
}
void CRenderViewMainWindow::ShowCamerasMenu()
{
   UpdateCamerasMenu();
}
void CRenderViewMainWindow::DebugShading()
{
   std::string debugShading = m_debugShadingActionGroup->checkedAction()->text().toStdString();
   if (debugShading == "Basic")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_BASIC);
   } else if (debugShading == "Occlusion")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_OCCLUSION);
   } else if (debugShading == "Wireframe")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_WIREFRAME);
   } else if (debugShading == "Normal")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_NORMAL);
   } else if (debugShading == "UV")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_UV);
   } else if (debugShading == "Primitive ID")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_PRIMITIVE_ID);
   } else if (debugShading == "Barycentric")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_BARY);
   } else if (debugShading == "Object")
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_OBJECT);
   } else if (debugShading == "Isolate Selected")
   {
      MSelectionList activeList;
      MGlobal::getActiveSelectionList(activeList);
      if( !activeList.isEmpty())
      {
         MObject depNode;
         activeList.getDependNode(0, depNode);
         if (depNode.hasFn(MFn::kTransform))
         {
            // from Transform to Shape
            MDagPath dagPath;
            activeList.getDagPath(0, dagPath);
            depNode = dagPath.child(0);
         }
         MFnDependencyNode nodeFn( depNode );

         // selection is valid
         m_renderView.m_shadingManager.SetShaderName(nodeFn.name().asChar());
         // connect to selection changed
      } else
      {
         m_renderView.m_shadingManager.SetShaderName("");
      }
      m_renderView.SetDebugShading(RV_DBG_SHAD_ISOLATE_SELECTED);
   }
   else
   {
      m_renderView.SetDebugShading(RV_DBG_SHAD_DISABLED);
   }
}



void CRenderViewMainWindow::EnableAOVs()
{
   K_enable_aovs = m_actionEnableAovs->isChecked();   
   m_menuAovs->setEnabled(K_enable_aovs);
}

void CRenderViewMainWindow::mousePressEvent( QMouseEvent * event )
{

   if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier))
   {
      if (m_3dManipulation)
      {
         if (event->buttons() & Qt::LeftButton)
         {
            m_manipulator = new CRenderView3DRotate(m_renderView, event->x(), event->y());
         } else if (event->buttons() & Qt::RightButton)
         {
            m_manipulator = new CRenderView3DZoom(m_renderView, event->x(), event->y());
         } else if (event->buttons() & Qt::MidButton)
         {
            m_manipulator = new CRenderView3DPan(m_renderView, event->x(), event->y());
         }

      } else
      {
         if ((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::MidButton))
         {
            m_manipulator = new CRenderView2DPan(m_renderView, event->x(), event->y());

         } else if (event->buttons() & Qt::RightButton)
         {            
            m_manipulator = new CRenderView2DZoom(m_renderView, event->x(), event->y());
         }

      }
      
      return;
   }


   const AtBBox2 *regionBox = m_renderView.m_gl->GetRegion();
   if (regionBox != NULL) 
   {
      // get pixel position
      int x = event->x();
      int y = event->y();
      int mousePosition[2];
      m_renderView.m_gl->Project(x, y, mousePosition[0], mousePosition[1], false);

      int regionStart[2];
      int regionEnd[2];
      AtNode *options = AiUniverseGetOptions();
      regionStart[0] = (int) regionBox->minx;//AiNodeGetInt(options, "region_min_x");
      regionStart[1] = (int) regionBox->miny;//AiNodeGetInt(options, "region_min_y");
      regionEnd[0] = (int) regionBox->maxx;//AiNodeGetInt(options, "region_max_x");
      regionEnd[1] = (int) regionBox->maxy;//AiNodeGetInt(options, "region_max_y");

      if ((mousePosition[0] <  regionStart[0] && mousePosition[0] >= regionStart[0] - 11) &&
         (mousePosition[1] <  regionStart[1] + 11 && mousePosition[1] >= regionStart[1]))
      {

         m_renderView.m_gl->ClearRegionCrop();
         m_renderView.InterruptRender();
         CRenderSession* renderSession = CMayaScene::GetRenderSession();
         renderSession->SetRegion(0, 0 , m_renderView.Width(), m_renderView.Height());
         AtNode *options = AiUniverseGetOptions();
         AiNodeSetInt(options, "region_min_x", -1);
         AiNodeSetInt(options, "region_min_y", -1);
         AiNodeSetInt(options, "region_max_x", -1);
         AiNodeSetInt(options, "region_max_y", -1);

         m_renderView.RestartRender();

         if (m_manipulator)
         {
            delete m_manipulator;
            m_manipulator = NULL;
         }
         return;
      }
   }


   // if SHIFT is pressed, region cropping is temporarily enabled
   if (m_renderView.m_regionCrop || QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
   {
      if(!(event->buttons() & Qt::LeftButton)) return;

      m_actionCropRegion->setChecked(true);
      CropRegion();
   
      m_manipulator = new CRenderViewCropRegion(m_renderView, event->x(), event->y());
      return;
   }


   // Pick the Shape
   m_renderView.PickShape(event->x(), event->y());
}

void CRenderViewMainWindow::mouseMoveEvent( QMouseEvent * event )
{
   if (m_manipulator)
   {
      m_manipulator->MouseMove(event->x(), event->y());
      return;
   }
   
   if( m_renderView.m_statusBarEnabled && m_renderView.m_statusBarPixelInfo)
   {
      int x = event->x();
      int y = event->y();

      int mousePosition[2];
      
      m_renderView.m_gl->Project(x, y, mousePosition[0], mousePosition[1], false);

      if (mousePosition[0] >= 0 && mousePosition[0] < m_renderView.m_width && mousePosition[1] >= 0 && mousePosition[1] < m_renderView.m_height)
      {
         m_renderView.RefreshStatusBar(mousePosition);
      }
   }

}

void CRenderViewMainWindow::mouseReleaseEvent( QMouseEvent * event )
{
   if (m_renderView.m_pickedId)
   {
      m_renderView.ClearPicking();
   }

   if (m_manipulator == NULL) return;
   m_manipulator->MouseRelease(event->x(), event->y());
   delete m_manipulator;
   m_manipulator = NULL;

   // once mouse button is down, we remove the crop region
   if (m_actionCropRegion->isChecked()) 
   {
      m_actionCropRegion->setChecked(false);
      m_renderView.m_regionCrop = false;
      // not calling CropRegion(), otherwise it would clear my region
   }

}
void CRenderViewMainWindow::moveEvent(QMoveEvent *event)
{
   if (m_ccWindow) m_ccWindow->AdjustPosition();
}

void CRenderViewMainWindow::resizeEvent(QResizeEvent *event)
{
   const QSize &newSize = event->size();
   m_renderView.m_gl->resize(newSize.width(), newSize.height() - (menuHeight + toolbarHeight + statusbarHeight + 26));


   if (m_3dManipulation) FrameAll();

   AtDisplaySync *sync = m_renderView.displaySync;

   // Lock to update the region
   AiCritSecEnter(&sync->lock);

   bool alreadyInQueue = sync->waitingDraw;
   // immediately draw or wait for timer to run out
   sync->waitingDraw = true;
   
   // if we're displayed a previously stored image
   // we don't want to prevent that we need a draw
   AiCritSecLeave(&sync->lock);

   if ( alreadyInQueue == false) 
   {
      m_renderView.m_gl->update();
   }
   if (m_ccWindow) m_ccWindow->AdjustPosition();

}

void CRenderViewMainWindow::wheelEvent ( QWheelEvent * event )
{
   if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier)) return;

   if (m_3dManipulation)
   {
      CRenderView3DZoom::Wheel(m_renderView, event->delta());
   } else
   {
      CRenderView2DZoom::Wheel(m_renderView, event->delta());
   }
}


void CRenderViewMainWindow::PopulateAOVsMenu()
{
   if (m_menuAovs == NULL) return; // not initialized

   if (m_aovsActionGroup != 0)
   {
      delete m_aovsActionGroup;
      m_aovsActionGroup = 0;
   }

   // clear all previous actions in the menu
   m_menuAovs->clear();
   // if AOVs are not enabled, we don't have anything to show
   if (!K_enable_aovs) 
   {
      //AiMsgError("[mtoa] AOVs must first be enabled to appear in the Render View");
      return; 
   }
   m_aovsActionGroup = new QActionGroup(this);

   QAction *action = m_menuAovs->addAction("Beauty");
   connect(action, SIGNAL(triggered()), this, SLOT(ShowAOV()));
   action->setCheckable(true);
   if (m_renderView.m_displayedAovIndex < 0) action->setChecked(true);

   m_aovsCombo->clear();
   m_aovsCombo->addItem("Beauty");

   m_aovsActionGroup->addAction(action);
   m_menuAovs->addSeparator();

   const std::vector<std::string> &aovNames = m_renderView.m_aovNames;
   for (int i = 0; i < (int)aovNames.size(); ++i)
   {
      action = m_menuAovs->addAction(QString(aovNames[i].c_str()));
      connect(action, SIGNAL(triggered()), this, SLOT(ShowAOV()));
      action->setCheckable(true);

      m_aovsCombo->addItem(QString(aovNames[i].c_str()));
      if (m_renderView.m_displayedAovIndex == i) 
      {
         action->setChecked(true);
      }
      m_aovsActionGroup->addAction(action);
   }
   m_aovsCombo->setCurrentIndex(m_renderView.m_displayedAovIndex + 1);
   connect(m_aovsCombo, SIGNAL(activated(int)), this, SLOT(ShowBoxAOV()));
}

void CRenderViewMainWindow::PopulateCamerasMenu()
{
   if (m_menuCamera == NULL) return;

   // don't forget to call this when new cameras are created
   if (m_camerasActionGroup != 0)
   {
      delete m_camerasActionGroup;
      m_camerasActionGroup = 0;
   }

   // clear all previous actions in the menu
   m_menuCamera->clear();
   m_camerasCombo->clear();

   m_camerasActionGroup = new QActionGroup(this);

   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_CAMERA);
   AtNode *currentCamera = (AtNode*)AiNodeGetPtr(AiUniverseGetOptions(), "camera");
   int i = 0;
   int cam_index = 0;

   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *node = AiNodeIteratorGetNext(iter);

      QAction *action = m_menuCamera->addAction(QString(AiNodeGetName(node)));
      connect(action, SIGNAL(triggered()), this, SLOT(SelectCamera()));
      action->setCheckable(true);      

      if (node == currentCamera)
      {
         action->setChecked(true);
         cam_index = i;
      } else
         action->setChecked(false);
      
      m_camerasActionGroup->addAction(action);
      m_camerasCombo->addItem(QString(AiNodeGetName(node)));

      i++;
   }

   connect(m_camerasCombo, SIGNAL(activated(int)), this, SLOT(SelectBoxCamera()));
   if (i > 0) m_camerasCombo->setCurrentIndex(cam_index);
   AiNodeIteratorDestroy(iter);
}


void CRenderViewMainWindow::ShowAOV()
{
   std::string aovName = m_aovsActionGroup->checkedAction()->text().toStdString();
   if (aovName == "Beauty")
   {
      // display the beauty
      m_renderView.m_displayedAovIndex = -1;

   } else
   {
      const std::vector<std::string> &aovNames = m_renderView.m_aovNames;
      for (size_t i = 0; i < aovNames.size(); ++i)
      {
         if (aovNames[i] == aovName)
         {
            // found my current AOV
            m_renderView.m_displayedAovIndex = i;
            break;
         }
      }
   }
   m_aovsCombo->setCurrentIndex(m_renderView.m_displayedAovIndex + 1);
   // update the toolbar widget   
   m_renderView.RefreshGLBuffer();
}

void CRenderViewMainWindow::ShowBoxAOV()
{
   int index = m_aovsCombo->currentIndex();
   m_renderView.m_displayedAovIndex = index - 1;
   std::string aovName = m_aovsCombo->currentText().toStdString();
   
   QList<QAction *>  menu_actions = m_aovsActionGroup->actions();
   for (int i = 0; i < menu_actions.length(); ++i)
   {
      if (menu_actions.at(i)->text().toStdString() == aovName)
      {
         menu_actions.at(i)->setChecked(true);
         break;
      }
   }

   m_renderView.RefreshGLBuffer();

}

static void SetCamera(CRenderView &renderView, const std::string &cameraName)
{
   AtNode *camera =  AiNodeLookUpByName (cameraName.c_str());
   if (camera == NULL)
   {
      AiMsgError("Camera Not found");
      return;
   }

   renderView.InterruptRender();

   // Since the Render Loop is rendering in a different thread, we must wait until 
   // rendering actually stops, otherwise the previous camera can pop back

   // Search for the MDagPath for this camera   
   MItDag itDag(MItDag::kDepthFirst, MFn::kCamera);
   itDag.reset();

   while (!itDag.isDone())
   {
      MDagPath camPath;
      itDag.getPath(camPath);
      std::string camName = camPath.partialPathName().asChar();
      if (camName == cameraName)
      {
         
         // Setting export camera in Arnold session will trigger an update and 
         // block my rendering :-/

//       CMayaScene::GetArnoldSession()->SetExportCamera(camPath);
         CMayaScene::GetRenderSession()->SetCamera(camPath);
         break;
      }      
      itDag.next();
   }


   while (AiRendering()) {CRenderView::Sleep(10);}
   AiNodeSetPtr(AiUniverseGetOptions(), "camera", (void*)camera);

   renderView.RestartRender();
}

void CRenderViewMainWindow::SelectCamera()
{
   std::string cameraName = m_camerasActionGroup->checkedAction()->text().toStdString();

   for (int i = 0; i < m_camerasCombo->count(); ++i)
   {
      if (m_camerasCombo->itemText(i).toStdString() == cameraName)
      {
         m_camerasCombo->setCurrentIndex(i);
         break;
      }
   }
   SetCamera(m_renderView, cameraName);
   
}
void CRenderViewMainWindow::SelectBoxCamera()
{
   std::string cameraName = m_camerasCombo->currentText().toStdString();
   QList<QAction *>  menu_actions = m_camerasActionGroup->actions();
   for (int i = 0; i < menu_actions.length(); ++i)
   {
      if (menu_actions.at(i)->text().toStdString() == cameraName)
      {
         menu_actions.at(i)->setChecked(true);
         break;
      }
   }
   SetCamera(m_renderView, cameraName);
}

void CRenderViewMainWindow::CropRegion()
{
   m_renderView.m_regionCrop = m_actionCropRegion->isChecked();
   if (!m_renderView.m_regionCrop)
   {
      m_renderView.m_gl->ClearRegionCrop();
      m_renderView.InterruptRender();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->SetRegion(0, 0 , m_renderView.m_width, m_renderView.m_height);
      AtNode *options = AiUniverseGetOptions();
      AiNodeSetInt(options, "region_min_x", -1);
      AiNodeSetInt(options, "region_min_y", -1);
      AiNodeSetInt(options, "region_max_x", -1);
      AiNodeSetInt(options, "region_max_y", -1);

      m_renderView.RestartRender();
   }

}

void CRenderViewMainWindow::FrameRegion()
{
   if (m_3dManipulation)
   {
      // Frame the selected geometries bounding box
      MSelectionList selected; 
      MBoundingBox globalBox;

      MGlobal::getActiveSelectionList(selected);
      for (unsigned int i = 0; i < selected.length(); ++i)
      {
         MDagPath dagPath;
         if (selected.getDagPath(i, dagPath) != MS::kSuccess) continue;
         MStatus status;
         MFnDagNode dagShape(dagPath, &status);
         if (status != MS::kSuccess) continue;

         MBoundingBox boundingBox = dagShape.boundingBox(&status);
         if (status != MS::kSuccess) continue;

         MMatrix mtx = dagPath.inclusiveMatrix();
   
         boundingBox.transformUsing(mtx);
         globalBox.expand(boundingBox);
      }
      AtNode *arnoldCamera = AiUniverseGetCamera();
      if (arnoldCamera == NULL) return;
      
      MSelectionList camList;
      camList.add(MString(AiNodeGetStr(arnoldCamera, "name")));

      MDagPath camDag;
      camList.getDagPath(0, camDag);

      if (!camDag.isValid()) return;
      MObject camNode = camDag.node();
      MFnCamera camera;
      camera.setObject(camDag);
      MMatrix camToWorld = camDag.inclusiveMatrix();

      // don't want to change the viewDirection & upDirection
      MVector viewDirection = camera.viewDirection(MSpace::kWorld);
      MVector upDirection = camera.upDirection(MSpace::kWorld);
      MPoint eyePoint = camera.eyePoint(MSpace::kWorld);
      MPoint centerInterest = camera.centerOfInterestPoint(MSpace::kWorld);

      MPoint center = globalBox.center();
      MFloatMatrix projectionMatrix = camera.projectionMatrix();

      MPoint newPos = eyePoint + center - centerInterest;
      float centerDist = center.distanceTo(newPos);

      camToWorld[3][0] = newPos.x;
      camToWorld[3][1] = newPos.y;
      camToWorld[3][2] = newPos.z;

      MMatrix worldToCam = camToWorld.inverse();

      globalBox.transformUsing(worldToCam);

      MPoint minBox = globalBox.min(); // in camera space
      MPoint maxBox = globalBox.max(); // in camera space

      MMatrix proj;
      for (int i =0; i < 4; ++i)
      {
         for (int j = 0; j < 4; ++j)
         {
            proj[i][j] = projectionMatrix[i][j];
         }
      }
      minBox = minBox * proj;
      maxBox = maxBox * proj;
      minBox.x /= minBox.z;
      minBox.y /= minBox.z;
      maxBox.x /= maxBox.z;
      maxBox.y /= maxBox.z;
      
      float maxScreen = MAX(MAX(ABS(minBox.x), ABS(maxBox.x)), MAX(ABS(minBox.y), ABS(maxBox.y)));
      // if maxScreen == 1 -> don't zoom
      // > 1 need to zoom out
      // < 1 need to zoom in

      newPos = center;
      newPos -=  viewDirection * centerDist * maxScreen;
      camera.set(newPos, viewDirection, upDirection, camera.horizontalFieldOfView(), camera.aspectRatio());
      camera.setCenterOfInterestPoint(center, MSpace::kWorld);

   } else
   {
      const AtBBox2 *region = m_renderView.m_gl->GetRegion();
      if (region == 0) 
      {
         FrameAll();
         return;
      }

      float zoomFactor = MIN((float)width() / (region->maxx - region->minx), (float)(height() - (menuHeight + toolbarHeight + statusbarHeight + 26) )/ (region->maxy - region->miny) );
      m_renderView.m_gl->SetZoomFactor(zoomFactor);
      
      AtPoint2 regionCenter;
      regionCenter.x = 0.5 * (region->maxx + region->minx);
      regionCenter.y = 0.5 * (region->maxy + region->miny);

      regionCenter.x -= m_renderView.m_width*0.5;
      regionCenter.y -= m_renderView.m_height*0.5;

      m_renderView.m_gl->SetPan(int(-regionCenter.x * zoomFactor),int(-regionCenter.y*zoomFactor));
      m_renderView.Draw();
   }
}

void CRenderViewMainWindow::FrameAll()
{
   // this function is called for 3d manipulation when the window is resized
   // so that the buffer always matches the windows size
   //if (m_3dManipulation) return; // we should frame the global bounding box

   float zoomFactor = MIN((float)width() / (float)m_renderView.m_width, (float)(height()- (menuHeight + toolbarHeight + statusbarHeight + 26))  / (float)m_renderView.m_height );
   m_renderView.m_gl->SetZoomFactor(zoomFactor);
   m_renderView.m_gl->SetPan(0, 0);
   m_renderView.Draw();
}

void CRenderViewMainWindow::RealSize()
{
   m_renderView.m_gl->SetPan (0, 0);
   m_renderView.m_gl->SetZoomFactor(1.f);
   m_renderView.Draw();
}

void CRenderViewMainWindow::ColorCorrection()
{
   if (m_lutAction->isChecked())
   {
      if (m_ccWindow == NULL)
      {
         m_ccWindow = new CRenderViewCCWindow(this, m_renderView, m_renderView.m_colorCorrectSettings);
         m_ccWindow->Init();
         m_ccWindow->AdjustPosition();
                
      }
      m_ccWindow->show();
   } else if(m_ccWindow)
   {
      delete m_ccWindow;
      m_ccWindow = NULL;
   }
}
void CRenderViewMainWindow::closeEvent(QCloseEvent *event)
{
   if (m_ccWindow)
   {
      delete m_ccWindow;
      m_ccWindow = NULL;
      m_lutAction->blockSignals(true);
      m_lutAction->setChecked(false);
      m_lutAction->blockSignals(false);
   }

   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
   {
      CMayaScene::GetArnoldSession()->SetContinuousUpdates(false);
      renderSession->SetRendering(false);
      m_renderView.FinishRender(); // this stops the rendering and destroys the render threads
      if (rvSelectionCb)
      {
         MMessage::removeCallback(rvSelectionCb);
         rvSelectionCb = 0;
         m_renderView.m_shadingManager.Restore();
         m_renderView.m_debugShading = RV_DBG_SHAD_DISABLED;
         m_debugShadingActionDisabled->blockSignals(true);
         m_debugShadingActionDisabled->setChecked(true);
         m_debugShadingActionDisabled->blockSignals(false);

      }
      CMayaScene::End();
      AiEnd();
   }
   event->accept();

}

void CRenderViewMainWindow::RgbaClicked()
{
   if (m_channelRedAction->isChecked())
   {
      m_channelGreenAction->setChecked(true);
   } else if (m_channelGreenAction->isChecked())
   {
      m_channelBlueAction->setChecked(true);

   } else if (m_channelBlueAction->isChecked())
   {
      m_channelAlphaAction->setChecked(true);

   } else if (m_channelAlphaAction->isChecked())
   {
      m_channelRgbaAction->setChecked(true);
   } else
   {
      m_channelRedAction->setChecked(true);
   }
   ShowChannel();
}



// If you add some slots, you'll have to run moc
#include "renderview.moc"
