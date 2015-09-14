/*
 * Windows window and event handling
 */

#include <time.h>
#include <string.h>
#include <sys/timeb.h>
#include <deque>

// Assume Windows XP as the build target
#define _WIN32_WINNT    0x501
#define _WIN32_WINDOWS  0x501

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include "display_gl.h"
#include "render_gl_widget.h"



#include "utility.h"
#include <emmintrin.h>
#include <intrin.h>

#include "renderview.h"
#include "render_loop.h"

#include "scene/MayaScene.h"
#include "session/ArnoldSession.h"
#include "icons/SA_logo_32.xpm"
#include "icons/SA_logo.xpm"

#include "icons/SA_icon_continuous_off.xpm"
#include "icons/SA_icon_continuous_on.xpm"
#include "icons/SA_icon_play.xpm"
#include "icons/SA_icon_region_off.xpm"
#include "icons/SA_icon_region_on.xpm"
#include "icons/SA_icon_stop.xpm"
#include "icons/SA_icon_store.xpm"
#include "icons/SA_icon_delete_stored.xpm"
#include "icons/SA_icon_transparent.xpm"

#include <maya/MQtUtil.h>

#include <sstream>
//#include <QtGui/qimage.h>

#pragma warning (disable : 4244)

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
   m_render_thread = NULL;
   display_sync = NULL;

   m_width = w;
   m_height = h;
   
   m_main_window = new CRenderViewMainWindow(MQtUtil::mainWindow(), *this);

   m_main_window->setWindowTitle("Arnold Render View");
   m_gl = NULL;
   m_buffer = NULL;



   initSize(w, h);

   init();

   m_main_window->initMenus();
   m_main_window->show();

}


CRenderView::~CRenderView()
{
   finishRender();

   AiCritSecClose(&display_sync->event_lock);
   AiCritSecClose(&display_sync->lock);
   AiFree(display_sync);
   display_sync = 0;
   AiFree(m_buffer);
   if (!m_storedImages.empty())
   {
      for (size_t i = 0; i < m_storedImages.size(); ++i)
      {
         AiFree(m_storedImages[i]);
      }
      m_storedImages.clear();
   }
   delete m_main_window;
}

void CRenderView::initSize(int w, int h)
{
   m_width = w;
   m_height = h;

   m_main_window->resize(w, h+menuHeight + toolbarHeight + statusbarHeight);

   if (m_gl != NULL) delete m_gl;

   m_gl = new CRenderGLWidget(m_main_window, *this, m_width, m_height);

   m_buffer = (AtRGBA *)AiMalloc(m_width * m_height * sizeof(AtRGBA));

   const size_t fillsize = m_width * m_height;

   for(size_t i = 0; i < fillsize; i++)
   {
      m_buffer[i] = AI_RGBA_BLACK;
   }

   m_gl->resize(m_width, m_height);

}
void CRenderView::init()
{

   //m_main_window->show();

   if (display_sync != NULL)
   {
      AiCritSecClose(&display_sync->event_lock);
      AiCritSecClose(&display_sync->lock);
      AiFree(display_sync);
      display_sync = 0;
   }

   m_color_mode  = COLOR_MODE_RGBA;
   m_show_rendering_tiles = false;
   m_region_crop = false;
   m_status_changed = false;
   m_status_bar_enabled = true;
   m_status_bar_pixel_info = false;
   m_restore_continuous = false;
   m_color_mode  = COLOR_MODE_RGBA;
   m_displayID = false;

   AiCritSecInit(&window_close_lock);

   K_render_window = true;
   K_progressive = true;
   m_continuous_updates = CMayaScene::GetArnoldSession()->GetContinuousUpdates();


   AtNode *filter;
   AtNode *driver;
   
   AiNodeEntryInstall(AI_NODE_DRIVER, AI_TYPE_NONE, K_DISPLAY_NAME,  NULL, kick_driver_mtd , AI_VERSION);

   driver = AiNode(K_DISPLAY_NAME);
   AiNodeSetStr(driver, "name", "kick_display");

   AtNode *id_filter = AiNode("closest_filter");
   AiNodeSetStr(id_filter, "name", "_renderViewDefault@closest_filter");



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


   m_displayedImageIndex = -1; // -1 means the current rendering
   // >= 0 refers to a stored Image

   m_displayedAovIndex = -1; // -1 means we're showing the beauty
   // >0 refers to an AOV

   K_InitGlobalVars();
   if (!m_storedImages.empty())
   {
      for (size_t i = 0; i < m_storedImages.size(); ++i)
      {
         AiFree(m_storedImages[i]);
      }
      m_storedImages.clear();
   }
   
   // setup syncing
   displaySyncCreate();

   updateRenderOptions();


}

void CRenderView::updateRenderOptions()
{

   // Parameters we check

   // bucket size
   // xres
   // yres
   // AA_samples
   // outputs


   AtNode *options = AiUniverseGetOptions();
   // enlarge resolution to capture overscan data
   int bucket_size = AiNodeGetInt(options, "bucket_size");
   int xres = AiNodeGetInt(options, "xres");
   int yres = AiNodeGetInt(options, "yres");

   x_res       = xres;
   y_res       = yres;
   reg_x       = xres; // check region_min_x ?
   reg_y       = yres;
   bucket_size = bucket_size;
   min_x       = 0; // chec region_min_x ?
   min_y       = 0;

   K_AA_samples = AiNodeGetInt(options, "AA_samples");
   if (K_AA_samples == 0)
   K_AA_samples = 1;
   
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
   AtBBox2 full_window;
   AtBBox2 display_window, data_window;
   display_window.minx = display_window.miny = 0;
   display_window.maxx = xres;
   display_window.maxy = yres;

   // Region data
   data_window = display_window;

   AiBBox2Union(full_window, display_window, data_window);

   x_res       = full_window.maxx - full_window.minx + 1;
   y_res       = full_window.maxy - full_window.miny + 1;
   reg_x       = data_window.maxx - data_window.minx + 1;
   reg_y       = data_window.maxy - data_window.miny + 1;
   bucket_size = bucket_size;
   min_x       = data_window.minx;
   min_y       = data_window.miny;
   
   

   // compute number of buckets we need to render the full frame
   if (display_sync)
   {
      int x_res = data_window.maxx - data_window.minx + 1;
      int y_res = data_window.maxy - data_window.miny + 1;
      int nbx = CEIL(x_res / (float) bucket_size);
      int nby = CEIL(y_res / (float) bucket_size);

      display_sync->buckets_left = nbx * nby;

      // detect if we restarted rendering at the lowesst resolution
      int AA_samples = AiNodeGetInt(options, "AA_samples");
      if (AA_samples <= display_sync->previous_AA_samples)
         display_sync->interrupted = false;
      display_sync->previous_AA_samples = AA_samples;
   }

}


void CRenderView::render()
{
   interruptRender();

   if (m_render_thread != NULL)
   {      
      K_aborted = true;
      K_wait_for_changes = false;      
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
   }  

   // make sure m_aovBuffers is resized to the appropriate amount of AOVs

   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   arnoldSession->SetContinuousUpdates(false);
   m_render_thread = AiThreadCreate(kickWindowRender, (void *)this, AI_PRIORITY_LOW);  

}
bool CRenderView::canRestartRender() const
{
   if (this == 0) return false;

   return true;

}
void CRenderView::close()
{
   delete m_main_window;
}

void CRenderView::show()
{  
   m_main_window->show();
}

void CRenderView::finishRender()
{
   interruptRender();
   if (m_render_thread != NULL)
   {  
      K_aborted = true;
      K_wait_for_changes = false;      
  
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
   }  

}
void CRenderView::interruptRender()
{
   if (AiRendering()) AiRenderInterrupt();
   
   // In theory I should wait until the rendering is finished
   // since the render loop is in another thread.
   // But we should check if it hurts interactivity
   //while (AiRendering()) {sleep(10);}
   K_wait_for_changes = true;
}
// utilities copied from sync.h and time.h in core

AtUInt64 CRenderView::time()
{
   LARGE_INTEGER PerformanceCount;
   LARGE_INTEGER Frequency;

   QueryPerformanceCounter(&PerformanceCount);
   QueryPerformanceFrequency(&Frequency);

   return (AtUInt64) (PerformanceCount.QuadPart / (Frequency.QuadPart * 1e-6));
}

void CRenderView::syncPause()
{
   _mm_pause();
}

void CRenderView::sleep(AtUInt64 usecs)
{
   Sleep(usecs / 1000);
}

   
void CRenderView::draw(AtBBox2 *region)  
{
   bool already_in_queue = false;
   AtDisplaySync *sync = display_sync;

   // Lock to update the region
   AiCritSecEnter(&sync->lock);

   already_in_queue = sync->waiting_draw;

   if (region)
   {
      // bucket update: enlarge region to be updated on draw
      AiBBox2Union(sync->front_update_region, sync->front_update_region, *region);
      display_sync->buckets_left--;    
   }
   else if (sync->buckets_left == 0 && !sync->interrupted)
   {
      // full frame finished: copy to back buffer
      m_gl->copyToBackBuffer();

      sync->back_update_region.minx = 0;
      sync->back_update_region.miny = 0;
      sync->back_update_region.maxx = reg_x;
      sync->back_update_region.maxy = reg_y;
      sync->back_buffer_ready = true;
   }

   // immediately draw or wait for timer to run out
   sync->waiting_draw = true;
   
   // if we're displayed a previously stored image
   // we don't want to prevent that we need a draw
   AiCritSecLeave(&sync->lock);

   if ( already_in_queue == false) 
   {
      m_gl->update();
   }
}


void CRenderView::displaySyncCreate()
{
   // Minimum frames per second at which we will always interrupt and redraw
   // regardless of what has been finished
   //FPS_min = 5.0f;
   // Maximum frames per second at which we will interrupt and redraw if a
   // a full frame has finished rendering
   //FPS_max = 30.0f;

   AtDisplaySync *sync = (AtDisplaySync *) AiMalloc(sizeof(AtDisplaySync));

   AtUInt64 now = time();

   sync->last_draw_time = now;
   sync->last_interrupt_time = now;

   AiBBox2Init(sync->front_update_region, std::numeric_limits<int>::max());
   AiBBox2Init(sync->back_update_region, std::numeric_limits<int>::max());
   sync->back_buffer_ready = false;

   sync->waiting_draw = false;
   sync->waiting_interrupt = false;
   

   sync->buckets_left = 0;
   sync->previous_AA_samples = std::numeric_limits<int>::max();
   sync->interrupted = true;

   AiCritSecInit(&sync->lock);
   AiCritSecInit(&sync->event_lock);

   display_sync = sync;
}


void CRenderView::updateRender()
{
   interruptRender();


   // we need to do some special stuff when one of those render settings
   // parameters have changed

   // bucket / xres / yres / outputs / AA_samples
   AtNode *options = AiUniverseGetOptions();
   
   int bucket_size = AiNodeGetInt(options, "bucket_size");
   int xres = AiNodeGetInt(options, "xres");
   int yres = AiNodeGetInt(options, "yres");
   //K_AA_samples = AiNodeGetInt(options, "AA_samples");
   AtArray *outputs = AiNodeGetArray(options, "outputs");

   AiNodeSetInt(options, "AA_samples", K_AA_samples); // setting back AA samples to its original value

   CMayaScene::UpdateSceneChanges();

   options = AiUniverseGetOptions();
   AtArray *new_outputs = AiNodeGetArray(options, "outputs");
   bool size_changed = (xres != AiNodeGetInt(options, "xres") || yres != AiNodeGetInt(options, "yres"));

   if (bucket_size != AiNodeGetInt(options, "bucket_size") ||
      size_changed ||
      K_AA_samples != AiNodeGetInt(options, "AA_samples") ||
      outputs != new_outputs)
   {
      if (size_changed) initSize(AiNodeGetInt(options, "xres"), AiNodeGetInt(options, "yres"));

      updateRenderOptions();
      m_main_window->populateAOVsMenu();
   }

   // check if amount of cameras have changed
   restartRender();
}

void CRenderView::restartRender()
{

   show();
   K_render_timestamp = time();
   K_restartLoop = true;
   K_wait_for_changes = false;

   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   arnoldSession->SetContinuousUpdates(false);

}

void CRenderView::checkSceneUpdates()
{  

   if (m_status_bar_enabled)
   {

      if (m_status_changed)
      {
         refreshStatusBar();
         m_status_changed = false;
      }
   }

   if (!m_continuous_updates) return;

   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();

   if (m_restore_continuous)
   {
      arnoldSession->SetContinuousUpdates(true);
      m_restore_continuous = false;
   }
   if (arnoldSession->HasObjectsToUpdate())
   {
      AtUInt64 loop_time = CRenderView::time();
      // 1 / 15 seconds minimum before restarting a render
      if (loop_time - K_render_timestamp > (AtUInt64)1000000/15)
      {
         K_wait_for_changes = true;
         // setting continuous updates to true
         // will trigger the update
         arnoldSession->SetContinuousUpdates(true);
      }
   }
}

void CRenderView::saveImage(const std::string &filename)
{
   // Write down the displayed image

   // We're using QT stuff, but maybe we should use Maya's instead ?
   QImage outImg(m_width, m_height, QImage::Format_ARGB32 );
   AtRGBA *buffer = getDisplayedBuffer();

   for (int j = 0; j < m_height; ++j)
   {
      for (int i = 0; i < m_width; ++i, ++buffer)
      {
         AtRGBA &color = *buffer;
         outImg.setPixel(i, j, qRgba((int)255*(color.r), (int)255*(color.g), (int)255*(color.b), (int)255*(color.a)));
      }
   }
   outImg.save(QString(filename.c_str()));
}

void CRenderView::storeImage()
{
   AtRGBA *storedBuffer = (AtRGBA *)AiMalloc(m_width * m_height * sizeof(AtRGBA));
   memcpy(storedBuffer, m_buffer, m_width * m_height * sizeof(AtRGBA));
   m_storedImages.push_back(storedBuffer);
   m_storedImagesStatus.push_back(m_status_log);
}

void CRenderView::refreshGLBuffer()
{
   m_displayID = (m_displayedImageIndex < 0) && (m_displayedAovIndex == (m_aovNames.size() -1));

   AtRGBA *displayedBuffer = getDisplayedBuffer();
   AtRGBA8 *gl_buffer = m_gl->getBuffer();

   int ind = 0;
   for (int j = 0; j < m_height; ++j)
   {
      for (int i = 0; i < m_width; ++i, ++ind)
      {
         copyToRGBA8(displayedBuffer[ind], gl_buffer[ind], i, j);
      }
   }

   m_gl->reloadBuffer(m_color_mode);
   draw();
   refreshStatusBar();
}
void CRenderView::refreshStatusBar(int *mouse_position)
{
   QString status_log(getDisplayedStatus().c_str());

   if (m_status_bar_pixel_info)
   {
      if (mouse_position != NULL)
      {
         // we have mouse position
         // get information about current pixel

         std::stringstream status_pixel;
         status_pixel<<"  Pixel: "<<mouse_position[0]<<","<<mouse_position[1];
         status_pixel<<"  RGBA: ("; // Get color from this pixel's buffer color 

         AtRGBA *displayedBuffer = getDisplayedBuffer();

         const AtRGBA &pixel_rgba = displayedBuffer[mouse_position[0] + m_width*mouse_position[1]];
         status_pixel<<CEIL(pixel_rgba.r * 1000.f) / 1000.f<<", "<<CEIL(pixel_rgba.g * 1000.f) / 1000.f<<", "<<CEIL(pixel_rgba.b * 1000.f) / 1000.f<<", "<<CEIL(pixel_rgba.a * 1000.f) / 1000.f<<")";
         status_log += QString(status_pixel.str().c_str());

      } else 
      {
         // check if previous log had pixel information and keep it
         std::string previous_log = m_main_window->statusBar()->currentMessage().toStdString();
         size_t pixel_pos = previous_log.find("  Pixel");
         if (pixel_pos != std::string::npos)
         {
            previous_log = previous_log.substr(pixel_pos);
            status_log += QString(previous_log.c_str());
         }
      }
   }

   m_main_window->statusBar()->showMessage(status_log);


}

void CRenderView::showPreviousStoredImage()
{
   if (m_storedImages.empty())
   {
      AiMsgError("[mtoa] No Image currently Stored in the Render View");      
      return;
   }
   if (m_displayedImageIndex < 0)
   {
      m_gl->copyToBackBuffer();
      m_displayedImageIndex = m_storedImages.size() - 1;
   }
   else m_displayedImageIndex--;

/*
   if (m_displayedImageIndex >= 0)
   {
      AtDisplaySync *sync = displaySync();
      sync->waiting_draw = false;
   }
*/
   refreshGLBuffer();
}
void CRenderView::showNextStoredImage()
{
   if (m_storedImages.empty())
   {
      AiMsgError("[mtoa] No Image currently Stored in the Render View");      
      return;
   }
   if (m_displayedImageIndex < 0)
   {
      m_gl->copyToBackBuffer();
   }

   if (m_displayedImageIndex >= (int)m_storedImages.size() - 1) m_displayedImageIndex = -1;
   else m_displayedImageIndex++;


/*
   if (m_displayedImageIndex >= 0)
   {
      AtDisplaySync *sync = displaySync();
      sync->waiting_draw = false;
   }
*/
   refreshGLBuffer();
   //m_gl->update();


}

void CRenderView::deleteStoredImage()
{
   if (m_storedImages.empty())
   {
      AiMsgError("[mtoa] No Image currently Stored in the Render View");      
      return;
   }
   if (m_displayedImageIndex < 0 || m_displayedImageIndex >= (int)m_storedImages.size()) return; // nothing to delete

   AiFree(m_storedImages[m_displayedImageIndex]);
   m_storedImages.erase(m_storedImages.begin() + m_displayedImageIndex);
   m_storedImagesStatus.erase(m_storedImagesStatus.begin() + m_displayedImageIndex);

   if (m_displayedImageIndex >= (int)m_storedImages.size()) m_displayedImageIndex--;

   refreshGLBuffer();
   //draw();

}


void
CRenderViewMainWindow::initMenus()
{
   QMenuBar *menubar = menuBar();

   //setWindowIcon(QIcon(QPixmap((const char **) SA_logo_32_xpm)));
   setWindowIcon(QIcon(QPixmap((const char **) SA_logo_xpm)));



   m_tool_bar =  addToolBar("Arnold");
   m_tool_bar->setAutoFillBackground(true);
   m_tool_bar->setPalette(palette());

   statusBar()->show();
   statusBar()->showMessage(" ");
   statusBar()->setAutoFillBackground(true);
   statusBar()->setPalette(palette()) ;

   m_menu_file = menubar->addMenu("File");

   QAction *action;
   action = m_menu_file->addAction("Save Image");
   connect( action, SIGNAL(triggered()), this, SLOT(saveImage()));

   action->setCheckable(false);
   action->setStatusTip("Save the Image currently being displayed");
   
   m_menu_view = menubar->addMenu("View");

   m_action_enable_aovs = m_menu_view->addAction("Enable AOVs");
   connect(m_action_enable_aovs, SIGNAL(triggered()), this, SLOT(enableAOVs()));
   m_action_enable_aovs->setCheckable(true);
   m_action_enable_aovs->setChecked(K_enable_aovs);
   m_action_enable_aovs->setStatusTip("Enable AOVs in the RenderView");
   
   m_menu_aovs = new QMenu("AOVs");
   m_menu_view->addMenu(m_menu_aovs);
   m_menu_aovs->setEnabled(K_enable_aovs);

   m_aovs_action_group = 0;

   m_aovs_combo = new QComboBox(this);
   m_tool_bar->addWidget(m_aovs_combo);
   m_tool_bar->addSeparator();
   
   populateAOVsMenu();

   m_menu_view->addSeparator();
   
   m_channel_action_group = new QActionGroup(this);

   action = m_menu_view->addAction("Red Channel");
   connect(action, SIGNAL(triggered()), this, SLOT(showChannel()));
   action->setCheckable(true);
   action->setStatusTip("Display the Red Channel");
   m_channel_action_group->addAction(action);

   action = m_menu_view->addAction("Green Channel");
   connect(action, SIGNAL(triggered()), this, SLOT(showChannel()));
   action->setCheckable(true);
   action->setStatusTip("Display the Green Channel");
   m_channel_action_group->addAction(action);

   action = m_menu_view->addAction("Blue Channel");
   connect(action, SIGNAL(triggered()), this, SLOT(showChannel()));
   action->setCheckable(true);
   action->setStatusTip("Display the Blue Channel");
   m_channel_action_group->addAction(action);

   action = m_menu_view->addAction("Alpha Channel");
   connect(action, SIGNAL(triggered()), this, SLOT(showChannel()));
   action->setCheckable(true);
   action->setStatusTip("Display the Alpha Channel");
   m_channel_action_group->addAction(action);

   action = m_menu_view->addAction("All Channels");
   connect(action, SIGNAL(triggered()), this, SLOT(showChannel()));
   action->setCheckable(true);
   action->setChecked(true);
   action->setStatusTip("Display all channels");
   m_channel_action_group->addAction(action);

   m_menu_view->addSeparator();

   action = m_menu_view->addAction("Frame All");
   connect(action, SIGNAL(triggered()), this, SLOT(frameAll()));
   action->setCheckable(false);
   action->setStatusTip("Frame the whole Image to fit the window size");
   action->setShortcut(Qt::Key_A);

   action = m_menu_view->addAction("Frame Region");
   connect(action, SIGNAL(triggered()), this, SLOT(frameRegion()));
   action->setCheckable(false);
   action->setStatusTip("Frame the Crop Region to fit the window size");
   action->setShortcut(Qt::Key_F);

   action = m_menu_view->addAction("Real Size");
   connect(action, SIGNAL(triggered()), this, SLOT(realSize()));
   action->setCheckable(false);
   action->setStatusTip("Display image with its real size");
   action->setShortcut(Qt::Key_O);

   m_menu_view->addSeparator();

   m_action_show_rendering_tiles = m_menu_view->addAction("Show Rendering Tiles");
   connect(m_action_show_rendering_tiles, SIGNAL(triggered()), this, SLOT(showRenderingTiles()));
   m_action_show_rendering_tiles->setCheckable(true);
   m_action_show_rendering_tiles->setStatusTip("Display the Tiles being rendered");

   QAction *store_action = m_menu_view->addAction("Store Image in RenderView");
   connect(store_action, SIGNAL(triggered()), this, SLOT(storeImage()));
   store_action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Plus);
   store_action->setStatusTip("Store the displayed Image in memory");

   action = m_menu_view->addAction("Previous Stored Image");
   connect(action, SIGNAL(triggered()), this, SLOT(previousStoredImage()));
   action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Left);
   action->setStatusTip("Display the Previous Image Stored in Memory");

   action = m_menu_view->addAction("Next Stored Image");
   connect(action, SIGNAL(triggered()), this, SLOT(nextStoredImage()));
   action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Right);
   action->setStatusTip("Display the Next Image Stored in Memory");

   m_delete_stored_action = m_menu_view->addAction("Delete Stored Image");
   connect(m_delete_stored_action, SIGNAL(triggered()), this, SLOT(deleteStoredImage()));
   m_delete_stored_action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Minus);
   m_delete_stored_action->setStatusTip("Delete the Stored Image being currently displayed");

   m_menu_view->addSeparator();

   action = m_menu_view->addAction("Color Correction");
   connect(action, SIGNAL(triggered()), this, SLOT(colorCorrection()));
   action->setCheckable(false);
   action->setStatusTip("Apply Color Correction on the displayed image");

   m_menu_view->addSeparator();   
   QMenu *status_menu = new QMenu("Status Bar");
   m_menu_view->addMenu(status_menu);


   m_action_status_bar = status_menu->addAction("Show Status Bar");
   connect(m_action_status_bar, SIGNAL(triggered()), this, SLOT(enableStatusBar()));
   m_action_status_bar->setCheckable(true);
   m_action_status_bar->setChecked(true);
   m_action_status_bar->setStatusTip("Display the Status Bar");

   m_action_status_info = status_menu->addAction("Display Pixel Information");
   connect(m_action_status_info, SIGNAL(triggered()), this, SLOT(displayPixelInfo()));
   m_action_status_info->setCheckable(true);
   m_action_status_info->setChecked(false);
   m_action_status_info->setStatusTip("Display Pixel Information in the Status Bar");


   m_menu_render = menubar->addMenu("Render");


   QAction *render_action = m_menu_render->addAction("Render");
   connect(render_action, SIGNAL(triggered()), this, SLOT(updateRender()));
   render_action->setCheckable(false);
   render_action->setShortcut(Qt::Key_F5);
   render_action->setStatusTip("Render / Update");
   
   QAction *abort_action = m_menu_render->addAction("Abort Render");
   connect(abort_action, SIGNAL(triggered()), this, SLOT(abortRender()));
   abort_action->setCheckable(false);
   abort_action->setStatusTip("Abort the current Render");
   abort_action->setShortcut(Qt::Key_Escape);

   m_action_auto_refresh = m_menu_render->addAction("Continuous Updates");
   connect(m_action_auto_refresh, SIGNAL(triggered()), this, SLOT(autoRefresh()));
   m_action_auto_refresh->setCheckable(true);
   m_action_auto_refresh->setChecked(CMayaScene::GetArnoldSession()->GetContinuousUpdates());
   m_action_auto_refresh->setStatusTip("Automatically update any change in the scene and restart the rendering");

//   m_action_auto_refresh->setIcon(QIcon(QPixmap((const char **) SA_continuous_update_xpm)));
   //m_action_auto_refresh->setIconVisibleInMenu(false);
   
   

   m_action_progressive_refinement = m_menu_render->addAction("Progressive Refinement");
   connect(m_action_progressive_refinement, SIGNAL(triggered()), this, SLOT(progressiveRefinement()));
   m_action_progressive_refinement->setCheckable(true);
   m_action_progressive_refinement->setChecked(true);
   m_action_progressive_refinement->setStatusTip("Display the Tiles being rendered");

   m_action_crop_region = m_menu_render->addAction("Crop Region");
   connect(m_action_crop_region, SIGNAL(triggered()), this, SLOT(cropRegion()));
   m_action_crop_region->setCheckable(true);
   m_action_crop_region->setChecked(false);
   m_action_crop_region->setStatusTip("Allow to drag a Crop Render Region");

   setMouseTracking(true);
   m_leftButtonDown = false;
   m_pickPoint[0] = m_pickPoint[1] = -1;
   m_previousPan[0] = m_previousPan[1] = 0;
   m_previousZoom = 1.f;

   m_menu_render->addSeparator();

   m_menu_camera = new QMenu("Camera");
   m_menu_render->addMenu(m_menu_camera);

   m_cameras_action_group = 0;

   m_cameras_combo = new QComboBox(this);
   m_tool_bar->addWidget(m_cameras_combo);
   
   populateCamerasMenu();

// Now dealing with the ToolButtons   

   setIconSize(QSize(18, 18));

   m_tool_bar->addSeparator();
   QString style_button = "QToolButton { border: none transparent;  border-radius: 1px;   background-color: transparent; min-width: 18;}  QToolButton:checked {    background-color: transparent ;}  QToolButton:flat {    border: none;} QToolButton:default {   border-color: transparent;}";

   QToolButton *render_button = new QToolButton(m_tool_bar);
   render_button->setDefaultAction(render_action);
   m_tool_bar->addWidget(render_button);
   QIcon render_icon(QPixmap((const char **) SA_icon_play_xpm));
   render_button->setIcon(render_icon);
   render_action->setIcon(render_icon);
   render_action->setIconVisibleInMenu(false);
   render_button->setStyleSheet(style_button);

   QToolButton *abort_button = new QToolButton(m_tool_bar);
   abort_button->setDefaultAction(abort_action);
   m_tool_bar->addWidget(abort_button);
   QIcon abort_icon(QPixmap((const char **) SA_icon_stop_xpm));
   abort_button->setIcon(abort_icon);
   abort_action->setIcon(abort_icon);
   abort_action->setIconVisibleInMenu(false);
   abort_button->setStyleSheet(style_button);

   m_tool_bar->addSeparator();
   QToolButton *refresh_button = new QToolButton(m_tool_bar);
   refresh_button->setDefaultAction(m_action_auto_refresh);
   m_tool_bar->addWidget(refresh_button);
   QIcon refresh_icon;
   refresh_icon.addPixmap(QPixmap((const char **) SA_icon_continuous_on_xpm), QIcon::Normal, QIcon::On);
   refresh_icon.addPixmap(QPixmap((const char **) SA_icon_continuous_off_xpm), QIcon::Normal, QIcon::Off);
   refresh_button->setIcon(refresh_icon);
   m_action_auto_refresh->setIcon(refresh_icon);
   m_action_auto_refresh->setIconVisibleInMenu(false);
   refresh_button->setStyleSheet(style_button);

   QToolButton *region_button = new QToolButton(m_tool_bar);
   region_button->setDefaultAction(m_action_crop_region);
   m_tool_bar->addWidget(region_button);
   QIcon region_icon;
   region_icon.addPixmap(QPixmap((const char **) SA_icon_region_on_xpm), QIcon::Normal, QIcon::On);
   region_icon.addPixmap(QPixmap((const char **) SA_icon_region_off_xpm), QIcon::Normal, QIcon::Off);
   region_button->setIcon(region_icon);
   m_action_crop_region->setIcon(region_icon);
   m_action_crop_region->setIconVisibleInMenu(false);
   region_button->setStyleSheet(style_button);


   m_tool_bar->addSeparator();
   QToolButton *store_button = new QToolButton(m_tool_bar);
   store_button->setDefaultAction(store_action);
   m_tool_bar->addWidget(store_button);
   QIcon store_icon(QPixmap((const char **) SA_icon_store_xpm));
   store_button->setIcon(store_icon);
   store_action->setIcon(store_icon);
   store_action->setIconVisibleInMenu(false);
   store_button->setStyleSheet(style_button);

   m_stored_slider = new QSlider(Qt::Horizontal, m_tool_bar);
   m_stored_slider->setTickInterval(1);
   m_stored_slider->setTickPosition(QSlider::TicksBothSides);
   m_stored_slider->resize(100, 10);
   m_stored_slider->setMaximumWidth(100);
   m_stored_slider->setMaximumHeight(10);

   m_stored_slider_action = m_tool_bar->addWidget(m_stored_slider);
   m_stored_slider->resize(100, 10);
   m_stored_slider->setMaximumWidth(100);
   m_stored_slider->setMaximumHeight(10);
   connect(m_stored_slider, SIGNAL(valueChanged(int)), this, SLOT(storedSliderMoved(int)));

   QToolButton *delete_stored_button = new QToolButton(m_tool_bar);
   delete_stored_button->setDefaultAction(m_delete_stored_action);
   m_tool_bar->addWidget(delete_stored_button);
   QIcon delete_stored_icon;
   delete_stored_icon.addPixmap(QPixmap((const char **) SA_icon_delete_stored_xpm), QIcon::Normal, QIcon::Off);
   delete_stored_icon.addPixmap(QPixmap((const char **) SA_icon_transparent_xpm), QIcon::Disabled, QIcon::Off);
   delete_stored_button->setIcon(delete_stored_icon);
   m_delete_stored_action->setIcon(delete_stored_icon);
   m_delete_stored_action->setIconVisibleInMenu(false);
   delete_stored_button->setStyleSheet(style_button);
   delete_stored_button->hide();

   updateStoredSlider();

}

void
CRenderViewMainWindow::saveImage()
{

   QString filename;
   QFileDialog dialog(this);
   dialog.setFileMode(QFileDialog::AnyFile);
   dialog.setAcceptMode(QFileDialog::AcceptSave);
   dialog.setDefaultSuffix("jpg");
   dialog.setWindowTitle("Save Image As");

   QStringList filters;
   filters <<"Image files (*.bmp, *.jpg, *.jpeg, *.png, *.ppm, *.xbm, *.xpm)"
         << "Any files (*)";
   dialog.setNameFilters(filters);

   if (!dialog.exec()) return;

   QStringList selected = dialog.selectedFiles();
   if (selected.empty()) return;
   filename = selected.at(0);

   m_renderView.saveImage(filename.toStdString());
}

void CRenderViewMainWindow::abortRender()
{
   m_renderView.interruptRender();
   CArnoldSession *arnoldSession = CMayaScene::GetArnoldSession();
   if (m_renderView.m_continuous_updates)   arnoldSession->SetContinuousUpdates(true);
}

void CRenderViewMainWindow::showRenderingTiles()
{
   m_renderView.setShowRenderingTiles(m_action_show_rendering_tiles->isChecked());
}


void CRenderViewMainWindow::updateRender()
{
   m_renderView.updateRender();

}

void CRenderViewMainWindow::autoRefresh()
{
   m_renderView.m_continuous_updates = m_action_auto_refresh->isChecked();
   CMayaScene::GetArnoldSession()->SetContinuousUpdates(m_action_auto_refresh->isChecked());
}

void CRenderViewMainWindow::storeImage()
{
   m_renderView.storeImage();
   updateStoredSlider();
}

void CRenderViewMainWindow::previousStoredImage()
{
   m_renderView.showPreviousStoredImage();
   updateStoredSlider();

}
void CRenderViewMainWindow::nextStoredImage()
{
   m_renderView.showNextStoredImage();
   updateStoredSlider();
}
void CRenderViewMainWindow::deleteStoredImage()
{
   m_renderView.deleteStoredImage();
   updateStoredSlider();  
}

void
CRenderViewMainWindow::storedSliderMoved(int i)
{
   m_renderView.m_displayedImageIndex = i -1;
   m_renderView.refreshGLBuffer();

}


void
CRenderViewMainWindow::updateStoredSlider()
{
   if (m_renderView.m_storedImages.empty())
   {
      m_stored_slider_action->setVisible(false);
      m_delete_stored_action->setVisible(false);
      return;
   }
   m_stored_slider_action->setVisible(true);
   m_delete_stored_action->setVisible(true);

   m_stored_slider->setMinimum(0);
   m_stored_slider->setMaximum(m_renderView.m_storedImages.size());   

   m_stored_slider->setSliderPosition(m_renderView.m_displayedImageIndex + 1);
}


void CRenderViewMainWindow::enableStatusBar()
{
   m_renderView.m_status_bar_enabled = m_action_status_bar->isChecked();
   if (m_renderView.m_status_bar_enabled) statusBar()->show();
   else statusBar()->hide();

}
void CRenderViewMainWindow::displayPixelInfo()
{
   m_renderView.m_status_bar_pixel_info = m_action_status_info->isChecked();
   m_renderView.m_status_changed = true;
}


void CRenderViewMainWindow::progressiveRefinement()
{
   K_progressive = m_action_progressive_refinement->isChecked();
   m_renderView.interruptRender();
   m_renderView.restartRender();
}


void CRenderViewMainWindow::showChannel()
{
   std::string colorMode = m_channel_action_group->checkedAction()->text().toStdString();
   if (colorMode == "Red Channel")
   {
      m_renderView.m_color_mode = COLOR_MODE_R;
   } else if(colorMode == "Green Channel")
   {
      m_renderView.m_color_mode = COLOR_MODE_G;
   } else if (colorMode == "Blue Channel")
   {
      m_renderView.m_color_mode = COLOR_MODE_B;
   } else if (colorMode == "Alpha Channel")
   {
      m_renderView.m_color_mode = COLOR_MODE_A;
   } else
   {
      m_renderView.m_color_mode = COLOR_MODE_RGBA;
   }
   m_renderView.getGlWidget()->reloadBuffer(m_renderView.m_color_mode);
}



void CRenderViewMainWindow::enableAOVs()
{
   K_enable_aovs = m_action_enable_aovs->isChecked();   
   m_menu_aovs->setEnabled(K_enable_aovs);
}

void CRenderViewMainWindow::mouseMoveEvent( QMouseEvent * event )
{
   if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier))
   {
      if ((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::MidButton))
      {
         m_renderView.m_gl->setPan(event->x() - m_pickPoint[0] + m_previousPan[0], event->y() - m_pickPoint[1] + m_previousPan[1]);
         m_renderView.draw();
      }
      if ((event->buttons() & Qt::RightButton))
      {
         float zoomFactor = powf(2.f, ((float)(event->x() - m_pickPoint[0]) / (float)m_renderView.m_width));
         zoomFactor *= m_previousZoom;
         m_renderView.m_gl->setZoomFactor(zoomFactor);

         m_renderView.draw();

      }
      return;
   }


   if( m_renderView.m_status_bar_enabled && m_renderView.m_status_bar_pixel_info)
   {
      int mouse_position[2];
      mouse_position[0] = event->x();
      mouse_position[1] = event->y();

      if (mouse_position[0] >= 0 && mouse_position[0] < m_renderView.m_width && mouse_position[1] >= 0 && mouse_position[1] < m_renderView.m_height)
      {
         m_renderView.refreshStatusBar(mouse_position);
      }
   }

   if (!m_leftButtonDown) return;

   // if SHIFT is pressed, region cropping is temporarily enabled
   if (m_renderView.m_region_crop || QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
   {
      
      int regionEnd[2];
      regionEnd[0] = event->x();
      regionEnd[1] = event->y();

      if (regionEnd[0] == m_pickPoint[0] || regionEnd[1] == m_pickPoint[1]) return;

      // drag region from m_pickPoint and current Point

      int bufferStart[2];
      int bufferEnd[2];
      m_renderView.m_gl->project(m_pickPoint[0], m_pickPoint[1], bufferStart[0], bufferStart[1], true);
      m_renderView.m_gl->project(regionEnd[0], regionEnd[1], bufferEnd[0], bufferEnd[1], true);


      // -> project in image space
      m_renderView.m_gl->setRegionCrop(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);
   }
}

void CRenderViewMainWindow::mousePressEvent( QMouseEvent * event )
{

   if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier))
   {
      if ((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::MidButton) || (event->buttons() & Qt::RightButton))
      {
         // TRANSLATION
         m_pickPoint[0] = event->x();
         m_pickPoint[1] = event->y();

         m_renderView.m_gl->getPan(m_previousPan[0], m_previousPan[1]);
         m_previousZoom = m_renderView.m_gl->getZoomFactor();
      }
      return;
   }

   // if SHIFT is pressed, region cropping is temporarily enabled
   if (m_renderView.m_region_crop || QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
   {
      if(!(event->buttons() & Qt::LeftButton)) return;

      m_leftButtonDown = true;
         
      // get mouse position
      // set in m_pickPoint
      m_pickPoint[0] = event->x();
      m_pickPoint[1] = event->y();
      return;
   }

   // Pick  The Shape
   
   int x, y;
   m_renderView.m_gl->project(event->x(), event->y(), x, y, true);

   // Get the ID AOV (last one in our list), and get the index value for the given pixel
   const AtRGBA &id_val = m_renderView.m_aovBuffers.back()[x + y * m_renderView.m_width];
   int Op_id;
   Op_id = *((int*)&id_val.r);

   // for now we do a linear search over the Shapes in the scene.
   // In the future we might want to store the Maya nodes in a map (per ID) for faster lookup
   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_SHAPE);
   
   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *node = AiNodeIteratorGetNext(iter);
      int nodeId = AiNodeGetInt(node, "id");
      if (nodeId == Op_id)
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
}
void CRenderViewMainWindow::mouseReleaseEvent( QMouseEvent * event )
{
   if (!m_leftButtonDown) return;
   
   // if SHIFT is pressed, region cropping is temporarily enabled
   if (m_renderView.m_region_crop || QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
   {
      
      m_leftButtonDown = false;
      int regionEnd[2];
      regionEnd[0] = event->x();
      regionEnd[1] = event->y();

      if (ABS(regionEnd[0] - m_pickPoint[0]) < 3 || ABS(regionEnd[1] - m_pickPoint[1]) < 3)
      {
         m_pickPoint[0] = m_pickPoint[1] = -1;
         m_renderView.m_gl->clearRegionCrop();
         m_renderView.interruptRender();
         CRenderSession* renderSession = CMayaScene::GetRenderSession();
         renderSession->SetRegion(0, 0 , m_renderView.m_width, m_renderView.m_height);
         AtNode *options = AiUniverseGetOptions();
         AiNodeSetInt(options, "region_min_x", -1);
         AiNodeSetInt(options, "region_min_y", -1);
         AiNodeSetInt(options, "region_max_x", -1);
         AiNodeSetInt(options, "region_max_y", -1);

         m_renderView.restartRender();
      // tell GL Widget to remove region
         return;
      }

      // draw region from m_pickPoint and current Point
      // set region in Arnold

      int bufferStart[2];
      int bufferEnd[2];
      m_renderView.m_gl->project(m_pickPoint[0], m_pickPoint[1], bufferStart[0], bufferStart[1], true);
      m_renderView.m_gl->project(regionEnd[0], regionEnd[1], bufferEnd[0], bufferEnd[1], true);

      m_renderView.m_gl->setRegionCrop(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);

      
      m_renderView.interruptRender();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->SetRegion(bufferStart[0], bufferStart[1], bufferEnd[0], bufferEnd[1]);
      AtNode *options = AiUniverseGetOptions();
      AiNodeSetInt(options, "region_min_x", bufferStart[0]);
      AiNodeSetInt(options, "region_min_y", bufferStart[1]);
      AiNodeSetInt(options, "region_max_x", bufferEnd[0]);
      AiNodeSetInt(options, "region_max_y", bufferEnd[1]);
      
      m_renderView.restartRender();
   }

}

void CRenderViewMainWindow::resizeEvent(QResizeEvent *event)
{
   const QSize &newSize = event->size();
   m_renderView.m_gl->resize(newSize.width(), newSize.height());
   m_renderView.draw();
}

void CRenderViewMainWindow::wheelEvent ( QWheelEvent * event )
{
   if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier)) return;
   
   float previousZoom = m_renderView.m_gl->getZoomFactor();
   float zoomFactor = powf(2.f, event->delta() / 240.0);

   int pivot[2];
   m_renderView.m_gl->project(int(width() * 0.5), int (height() * 0.5), pivot[0], pivot[1], true);

   AtPoint2 regionCenter;
   // this was the previous image center
   regionCenter.x = pivot[0] - m_renderView.m_width*0.5;
   regionCenter.y = pivot[1] - m_renderView.m_height*0.5;

   zoomFactor *= previousZoom;
   m_renderView.m_gl->setZoomFactor(zoomFactor);

   // I want my image center to be the same as before
   m_renderView.m_gl->setPan(-regionCenter.x * zoomFactor,-regionCenter.y*zoomFactor);
   m_renderView.draw();
}


void CRenderViewMainWindow::populateAOVsMenu()
{
   if (m_menu_aovs == NULL) return; // not initialized

   if (m_aovs_action_group != 0)
   {
      delete m_aovs_action_group;
      m_aovs_action_group = 0;
   }

   // clear all previous actions in the menu
   m_menu_aovs->clear();
   // if AOVs are not enabled, we don't have anything to show
   if (!K_enable_aovs) 
   {
      //AiMsgError("[mtoa] AOVs must first be enabled to appear in the Render View");
      return; 
   }
   m_aovs_action_group = new QActionGroup(this);

   QAction *action = m_menu_aovs->addAction("Beauty");
   connect(action, SIGNAL(triggered()), this, SLOT(showAOV()));
   action->setCheckable(true);
   if (m_renderView.m_displayedAovIndex < 0) action->setChecked(true);

   m_aovs_combo->clear();
   m_aovs_combo->addItem("Beauty");

   m_aovs_action_group->addAction(action);
   m_menu_aovs->addSeparator();

   const std::vector<std::string> &aovNames = m_renderView.m_aovNames;
   for (int i = 0; i < (int)aovNames.size(); ++i)
   {
      action = m_menu_aovs->addAction(QString(aovNames[i].c_str()));
      connect(action, SIGNAL(triggered()), this, SLOT(showAOV()));
      action->setCheckable(true);

      m_aovs_combo->addItem(QString(aovNames[i].c_str()));
      if (m_renderView.m_displayedAovIndex == i) 
      {
         action->setChecked(true);
      }
      m_aovs_action_group->addAction(action);
   }
   m_aovs_combo->setCurrentIndex(m_renderView.m_displayedAovIndex + 1);
   connect(m_aovs_combo, SIGNAL(activated(int)), this, SLOT(showBoxAOV()));
}

void CRenderViewMainWindow::populateCamerasMenu()
{
   if (m_menu_camera == NULL) return;

   // don't forget to call this when new cameras are created
   if (m_cameras_action_group != 0)
   {
      delete m_cameras_action_group;
      m_cameras_action_group = 0;
   }

   // clear all previous actions in the menu
   m_menu_camera->clear();
   m_cameras_combo->clear();

   m_cameras_action_group = new QActionGroup(this);

   AtNodeIterator *iter = AiUniverseGetNodeIterator(AI_NODE_CAMERA);
   AtNode *currentCamera = (AtNode*)AiNodeGetPtr(AiUniverseGetOptions(), "camera");
   int i = 0;
   int cam_index = 0;

   while (!AiNodeIteratorFinished(iter))
   {
      AtNode *node = AiNodeIteratorGetNext(iter);

      QAction *action = m_menu_camera->addAction(QString(AiNodeGetName(node)));
      connect(action, SIGNAL(triggered()), this, SLOT(selectCamera()));
      action->setCheckable(true);
      

      if (node == currentCamera)
      {
         action->setChecked(true);
         cam_index = i;
      } else
         action->setChecked(false);
      
      m_cameras_action_group->addAction(action);
      m_cameras_combo->addItem(QString(AiNodeGetName(node)));

      i++;
   }

   connect(m_cameras_combo, SIGNAL(activated(int)), this, SLOT(selectBoxCamera()));
   if (i > 0) m_cameras_combo->setCurrentIndex(cam_index);
   AiNodeIteratorDestroy(iter);
}


void CRenderViewMainWindow::showAOV()
{
   std::string aovName = m_aovs_action_group->checkedAction()->text().toStdString();
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
   m_aovs_combo->setCurrentIndex(m_renderView.m_displayedAovIndex + 1);
   // update the toolbar widget   
   m_renderView.refreshGLBuffer();
}
void CRenderViewMainWindow::showBoxAOV()
{
   int index = m_aovs_combo->currentIndex();
   m_renderView.m_displayedAovIndex = index - 1;
   std::string aovName = m_aovs_combo->currentText().toStdString();
   
   QList<QAction *>  menu_actions = m_aovs_action_group->actions();
   for (int i = 0; i < menu_actions.length(); ++i)
   {
      if (menu_actions.at(i)->text().toStdString() == aovName)
      {
         menu_actions.at(i)->setChecked(true);
         break;
      }
   }

   m_renderView.refreshGLBuffer();

}

static void setCamera(CRenderView &renderView, const std::string &cameraName)
{
   AtNode *camera =  AiNodeLookUpByName (cameraName.c_str());
   if (camera == NULL)
   {
      AiMsgError("Camera Not found");
      return;
   }

   renderView.interruptRender();

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


   while (AiRendering()) {renderView.sleep(10);}
   AiNodeSetPtr(AiUniverseGetOptions(), "camera", (void*)camera);

   renderView.restartRender();
}

void CRenderViewMainWindow::selectCamera()
{
   std::string cameraName = m_cameras_action_group->checkedAction()->text().toStdString();

   for (int i = 0; i < m_cameras_combo->count(); ++i)
   {
      if (m_cameras_combo->itemText(i).toStdString() == cameraName)
      {
         m_cameras_combo->setCurrentIndex(i);
         break;
      }
   }
   setCamera(m_renderView, cameraName);
   
}
void CRenderViewMainWindow::selectBoxCamera()
{
   std::string cameraName = m_cameras_combo->currentText().toStdString();
   QList<QAction *>  menu_actions = m_cameras_action_group->actions();
   for (int i = 0; i < menu_actions.length(); ++i)
   {
      if (menu_actions.at(i)->text().toStdString() == cameraName)
      {
         menu_actions.at(i)->setChecked(true);
         break;
      }
   }
   setCamera(m_renderView, cameraName);
}
void CRenderViewMainWindow::cropRegion()
{
   m_renderView.m_region_crop = m_action_crop_region->isChecked();
   if (!m_renderView.m_region_crop)
   {
      m_renderView.m_gl->clearRegionCrop();
      m_renderView.interruptRender();
      CRenderSession* renderSession = CMayaScene::GetRenderSession();
      renderSession->SetRegion(0, 0 , m_renderView.m_width, m_renderView.m_height);
      AtNode *options = AiUniverseGetOptions();
      AiNodeSetInt(options, "region_min_x", -1);
      AiNodeSetInt(options, "region_min_y", -1);
      AiNodeSetInt(options, "region_max_x", -1);
      AiNodeSetInt(options, "region_max_y", -1);

      m_renderView.restartRender();
   }
}


void CRenderViewMainWindow::frameRegion()
{
   const AtBBox2 *region = m_renderView.m_gl->getRegion();
   if (region == 0) 
   {
      frameAll();
      return;
   }

   float zoomFactor = MIN((float)width() / (region->maxx - region->minx), (float)height() / (region->maxy - region->miny) );
   m_renderView.m_gl->setZoomFactor(zoomFactor);

   AtPoint2 regionCenter;
   regionCenter.x = 0.5 * (region->maxx + region->minx);
   regionCenter.y = 0.5 * (region->maxy + region->miny);

   regionCenter.x -= m_renderView.m_width*0.5;
   regionCenter.y -= m_renderView.m_height*0.5;

   m_renderView.m_gl->setPan(-regionCenter.x * zoomFactor,-regionCenter.y*zoomFactor);
   m_renderView.draw();
}

void CRenderViewMainWindow::frameAll()
{
   float zoomFactor = MIN((float)width() / (float)m_renderView.m_width, (float)height() / (float)m_renderView.m_height);
   m_renderView.m_gl->setZoomFactor(zoomFactor);
   m_renderView.m_gl->setPan(0, 0);
   m_renderView.draw();
}

void CRenderViewMainWindow::realSize()
{
   m_renderView.m_gl->setPan (0, 0);
   m_renderView.m_gl->setZoomFactor(1.f);
   m_renderView.draw();
}

void CRenderViewMainWindow::colorCorrection()
{

   if (m_cc_window == NULL)
   {
      m_cc_window = new CRenderViewCCWindow(this, m_renderView, m_renderView.m_colorCorrectSettings);
      m_cc_window->init();
   }
   m_cc_window->show();
}
void CRenderViewMainWindow::closeEvent(QCloseEvent *event)
{
   CRenderSession* renderSession = CMayaScene::GetRenderSession();
   if (renderSession)
   {
      CMayaScene::GetArnoldSession()->SetContinuousUpdates(false);
      renderSession->SetRendering(false);
      m_renderView.finishRender(); // this stops the rendering and destroys the render threads
      CMayaScene::End(); // This will delete my RenderView, along with all my options
      // I guess people will want to keep their options, stored images, etc... so we'd need to change that
      AiEnd();
   }
   event->accept();
}

void CRenderViewCCWindow::init()
{
   setWindowTitle("Color Correction");
   
   int line = 20;
   m_gamma_edit = new QLineEdit(this);
   m_gamma_edit->move(80, line);

   m_gamma_edit->resize(40, 20);
   m_gamma_edit->setValidator( new QDoubleValidator(0., 100., 2, this) );
   m_gamma_edit->setText(QString::number(m_colorCorrectSettings.gamma));
   

   m_gamma_slider = new QSlider(Qt::Horizontal, this);
   m_gamma_slider->move(130, line);

   
   m_gamma_slider->resize(150, 20);
   m_gamma_slider->setMinimum(0);
   m_gamma_slider->setMaximum(500);
   m_gamma_slider->setValue(m_colorCorrectSettings.gamma * 100);

   QLabel *label = new QLabel(QString("Gamma"), this);
   label->resize(40, 20);
   label->move(20, line );

   connect(m_gamma_slider, SIGNAL(valueChanged(int)), this, SLOT(gammaSliderChanged()));
   connect(m_gamma_edit, SIGNAL(returnPressed()), this, SLOT(gammaTextChanged()));

   line += 30;

   m_brightness_edit = new QLineEdit(this);
   m_brightness_edit->move(80, line);

   
   m_brightness_edit->resize(40, 20);
   m_brightness_edit->setValidator( new QDoubleValidator(0., 100., 2, this) );
   m_brightness_edit->setText(QString::number(m_colorCorrectSettings.gamma));
   
   m_brightness_slider = new QSlider(Qt::Horizontal, this);
   m_brightness_slider->move(130, line);
   m_brightness_slider->resize(150, 20);
   m_brightness_slider->setMinimum(0);
   m_brightness_slider->setMaximum(500);
   m_brightness_slider->setValue(m_colorCorrectSettings.gamma * 100);
   label = new QLabel(QString("Brightness"), this);
   label->move(20, line);
   label->resize(55, 20);

   connect(m_brightness_slider, SIGNAL(valueChanged(int)), this, SLOT(brightnessSliderChanged()));
   connect(m_brightness_edit, SIGNAL(returnPressed()), this, SLOT(brightnessTextChanged()));

   line += 30;

   m_dither_box = new QCheckBox( this);
   m_dither_box->move(80, line);
   m_dither_box->setChecked(m_colorCorrectSettings.dither);

   connect(m_dither_box, SIGNAL( stateChanged(int) ), this, SLOT(ditherChanged()));

   label = new QLabel(QString("Dither"), this);
   label->move(20, line);
   label->resize(40, 20);

   line += 30;

   m_srgb_box = new QCheckBox(this);
   m_srgb_box->move(80, line);
   m_srgb_box->setChecked(m_colorCorrectSettings.srgb);
   label = new QLabel(QString("sRGB"), this);
   label->move(20, line);
   label->resize(40, 20);

   connect(m_srgb_box, SIGNAL( stateChanged(int) ), this, SLOT(srgbChanged()));

   line += 50;
   resize(300, line);
}

void CRenderViewCCWindow::gammaSliderChanged()
{
   m_colorCorrectSettings.gamma = ((float)m_gamma_slider->sliderPosition()) / 100.f;

   m_gamma_edit->blockSignals(true);
   m_gamma_edit->setText(QString::number(m_colorCorrectSettings.gamma));
   m_gamma_edit->blockSignals(false);
   m_renderView.refreshGLBuffer();
}

void CRenderViewCCWindow::gammaTextChanged()
{

   QString gamma = m_gamma_edit->text();
   m_colorCorrectSettings.gamma = gamma.toFloat();
   m_gamma_slider->blockSignals(true);
   m_gamma_slider->setValue(m_colorCorrectSettings.gamma * 100);
   m_gamma_slider->blockSignals(false);
   m_renderView.refreshGLBuffer();
}


void CRenderViewCCWindow::brightnessSliderChanged()
{
   m_colorCorrectSettings.brightness = ((float)m_brightness_slider->sliderPosition()) / 100.f;

   m_brightness_edit->blockSignals(true);
   m_brightness_edit->setText(QString::number(m_colorCorrectSettings.brightness));
   m_brightness_edit->blockSignals(false);
   m_renderView.refreshGLBuffer();
}

void CRenderViewCCWindow::brightnessTextChanged()
{

   QString brightness = m_brightness_edit->text();
   m_colorCorrectSettings.brightness = brightness.toFloat();
   m_brightness_slider->blockSignals(true);
   m_brightness_slider->setValue(m_colorCorrectSettings.brightness * 100);
   m_brightness_slider->blockSignals(false);
   m_renderView.refreshGLBuffer();
}


void CRenderViewCCWindow::ditherChanged()
{
   m_colorCorrectSettings.dither = m_dither_box->isChecked();
   m_renderView.refreshGLBuffer();
}

void CRenderViewCCWindow::srgbChanged()
{
   m_colorCorrectSettings.srgb = m_srgb_box->isChecked();
   m_renderView.refreshGLBuffer();
}


// If you add some slots, you'll have to run moc
#include "renderview.moc"
