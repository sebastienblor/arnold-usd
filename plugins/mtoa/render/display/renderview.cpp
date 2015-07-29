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


//#include <QtGui/qimage.h>

#pragma warning (disable : 4244)

/*****************************
 *
 *   CRenderView
 *
 ***************************/

static int menuHeight = 10;

CRenderView::CRenderView(int w, int h)
{
   m_width = w;
   m_height = h;
   
   m_main_window = new CRenderViewMainWindow(0, *this);

   m_main_window->resize(w, h+menuHeight);
   m_main_window->setWindowTitle("Arnold Render View");
   m_gl = new CRenderGLWidget(m_main_window, *this, m_width, m_height);

   m_render_thread = NULL;
   display_sync = NULL;

   init();

   m_main_window->initMenus();
   m_main_window->show();

}


CRenderView::~CRenderView()
{
   interruptRender();
   if (m_render_thread != NULL)
   {  
      K_aborted = true;
      K_wait_for_changes = false;      
  
      AiThreadWait(m_render_thread);
      AiThreadClose(m_render_thread);
   }  


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

void CRenderView::init()
{
   m_buffer = (AtRGBA *)AiMalloc(m_width * m_height * sizeof(AtRGBA));

   const size_t fillsize = m_width * m_height;

   for(size_t i = 0; i < fillsize; i++)
   {
      m_buffer[i] = AI_RGBA_BLACK;
   }

   m_gl->resize(m_width, m_height);
   //m_main_window->show();

   if (display_sync != NULL)
   {
      AiCritSecClose(&display_sync->event_lock);
      AiCritSecClose(&display_sync->lock);
      AiFree(display_sync);
      display_sync = 0;
   }

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
   m_dither      = false;
   m_color_mode  = COLOR_MODE_RGBA;
   m_gamma       = 1.f; // we'll control that later on
   m_show_rendering_tiles = false;
   m_region_crop = false;

   K_AA_samples = AiNodeGetInt(options, "AA_samples");
   if (K_AA_samples == 0)
   K_AA_samples = 1;

   AiCritSecInit(&window_close_lock);

   K_render_window = true;
   K_progressive = true;

   //unsigned int i;
   AtNode *filter;
   AtNode *driver;
   //char kick_drv[1024];
   AtArray *outputs;

   AiNodeEntryInstall(AI_NODE_DRIVER, AI_TYPE_NONE, K_DISPLAY_NAME,  NULL, kick_driver_mtd , AI_VERSION);

   driver = AiNode(K_DISPLAY_NAME);
   AiNodeSetStr(driver, "name", "kick_display");

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

   
   // We should support the options changes during a session
   // duplicate the old outputs array
   outputs = AiNodeGetArray(AiUniverseGetOptions(), "outputs");

   m_aovBuffers.reserve(outputs->nelements);
   m_aovNames.reserve(outputs->nelements);

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
      outputStr += AiNodeGetName(driver);

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
         AtRGBA *aovBuffer = (AtRGBA *)AiMalloc(m_width * m_height * sizeof(AtRGBA));
         memset(aovBuffer, 0, m_width * m_height * sizeof(AtRGBA));
         //add the aov buffer
         m_aovBuffers.push_back(aovBuffer);         
      }
   }   


   AtArray *allOutputsArray = AiArrayAllocate(outputValues.size(), 1, AI_TYPE_STRING);
   for (size_t i=0; i < outputValues.size(); i++)
   {
      AiArraySetStr(allOutputsArray, i, outputValues[i].c_str());
   }
   AiNodeSetArray(AiUniverseGetOptions(), "outputs", allOutputsArray);

   AiNodeSetPtr(driver, "userdata", (void *)this);

   // enlarge resolution to capture overscan data
   AtBBox2 full_window;
   AtBBox2 display_window, data_window;
   display_window.minx = display_window.miny = 0;
   display_window.maxx = m_width;
   display_window.maxy = m_height;

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
   m_dither      = true;
   m_color_mode  = COLOR_MODE_RGBA;
   m_gamma       = 1;

   // setup syncing
   displaySyncCreate(x_res, y_res);

   AiCritSecInit(&window_close_lock);

   // compute number of buckets we need to render the full frame
   if (display_sync)
   {
      int x_res = data_window.maxx - data_window.minx + 1;
      int y_res = data_window.maxy - data_window.miny + 1;
      int nbx = CEIL(x_res / (float) bucket_size);
      int nby = CEIL(y_res / (float) bucket_size);

      display_sync->buckets_left = nbx * nby;

      // detect if we restarted rendering at the lowesst resolution
      int AA_samples = AiNodeGetInt(AiUniverseGetOptions(), "AA_samples");
      if (AA_samples <= display_sync->previous_AA_samples)
         display_sync->interrupted = false;
      display_sync->previous_AA_samples = AA_samples;
   }

   K_InitGlobalVars();
   if (!m_storedImages.empty())
   {
      for (size_t i = 0; i < m_storedImages.size(); ++i)
      {
         AiFree(m_storedImages[i]);
      }
      m_storedImages.clear();
   }
   
   m_displayedImageIndex = -1; // -1 means the current rendering
   // >= 0 refers to a stored Image

   m_displayedAovIndex = -1; // -1 means we're showing the beauty
   // >0 refers to an AOV

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

   m_render_thread = AiThreadCreate(kickWindowRender, (void *)this, AI_PRIORITY_LOW);  

}
bool CRenderView::canRefresh() const
{
   return true; // for now always return true
   static AtUInt64 renderView_refresh_time = 1000 / 10   ; // 1/10 seconds
   AtUInt64 current_time = time();
   return (current_time - K_render_timestamp < renderView_refresh_time);
}
void CRenderView::close()
{
   delete m_main_window;
}

void CRenderView::show()
{  
   m_main_window->show();
}

void CRenderView::interruptRender()
{
   if (AiRendering()) AiRenderInterrupt();
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

/*
AtRGBA8 *CRenderView::getBuffer()
{
   return m_gl->buffer();
}

int CRenderView::getBufferWidth() const
{
   return m_gl->width();
}

int CRenderView::getBufferHeight() const
{
   return m_gl->height();
}
*/
   
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
   if (m_displayedImageIndex < 0) sync->waiting_draw = true;
   else sync->waiting_draw = false;
   // if we're displayed a previously stored image
   // we don't want to prevent that we need a draw
   AiCritSecLeave(&sync->lock);

   if ( already_in_queue == false && sync->waiting_draw == true) 
   {
      m_gl->update();
   }
}


void CRenderView::displaySyncCreate(int width, int height)
{
   // Minimum frames per second at which we will always interrupt and redraw
   // regardless of what has been finished
   static const float FPS_min = 5.0f;
   // Maximum frames per second at which we will interrupt and redraw if a
   // a full frame has finished rendering
   static const float FPS_max = 30.0f;

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




void CRenderView::refresh()
{
   show();
   K_restartLoop = true;
   K_wait_for_changes = false;   
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
}

void CRenderView::refreshGLBuffer()
{  
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

   if (m_displayedImageIndex >= 0)
   {
      AtDisplaySync *sync = displaySync();
      sync->waiting_draw = false;
   }

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


   if (m_displayedImageIndex >= 0)
   {
      AtDisplaySync *sync = displaySync();
      sync->waiting_draw = false;
   }

   refreshGLBuffer();

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
   if (m_displayedImageIndex >= (int)m_storedImages.size()) m_displayedImageIndex--;

   refreshGLBuffer();

}


void
CRenderViewMainWindow::initMenus()
{

   QMenuBar *menubar = menuBar();
   m_menu_file = menubar->addMenu("File");

   QAction *action;
   action = m_menu_file->addAction("Save Image");
   connect(action, SIGNAL(triggered()), this, SLOT(saveImage()));

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

   action = m_menu_view->addAction("Store Image in RenderView");
   connect(action, SIGNAL(triggered()), this, SLOT(storeImage()));
   action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Plus);
   action->setStatusTip("Store the displayed Image in memory");

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

   action = m_menu_view->addAction("Delete Stored Image");
   connect(action, SIGNAL(triggered()), this, SLOT(deleteStoredImage()));
   action->setCheckable(false);
   //action->setShortcut(Qt::CTRL + Qt::Key_Minus);
   action->setStatusTip("Delete the Stored Image being currently displayed");


   m_menu_render = menubar->addMenu("Render");

   action = m_menu_render->addAction("Update Render");
   connect(action, SIGNAL(triggered()), this, SLOT(updateRender()));
   action->setCheckable(false);
   action->setShortcut(Qt::Key_F5);
   action->setStatusTip("Update the rendering");

   action = m_menu_render->addAction("Abort Render");
   connect(action, SIGNAL(triggered()), this, SLOT(abortRender()));
   action->setCheckable(false);
   action->setStatusTip("Abort the current Render");
   action->setShortcut(Qt::Key_Escape);

   m_action_auto_refresh = m_menu_render->addAction("Continuous Updates");
   connect(m_action_auto_refresh, SIGNAL(triggered()), this, SLOT(autoRefresh()));
   m_action_auto_refresh->setCheckable(true);
   m_action_auto_refresh->setChecked(CMayaScene::GetArnoldSession()->GetContinuousUpdates());
   m_action_auto_refresh->setStatusTip("Automatically update any change in the scene and restart the rendering");

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
}

void CRenderViewMainWindow::showRenderingTiles()
{
   m_renderView.setShowRenderingTiles(m_action_show_rendering_tiles->isChecked());
}


void CRenderViewMainWindow::updateRender()
{
   m_renderView.interruptRender();
   CMayaScene::UpdateSceneChanges();
   m_renderView.refresh();

}

void CRenderViewMainWindow::autoRefresh()
{
   CMayaScene::GetArnoldSession()->SetContinuousUpdates(m_action_auto_refresh->isChecked());
}

void CRenderViewMainWindow::storeImage()
{
   m_renderView.storeImage();
}

void CRenderViewMainWindow::previousStoredImage()
{
   m_renderView.showPreviousStoredImage();

}
void CRenderViewMainWindow::nextStoredImage()
{
   m_renderView.showNextStoredImage();
}
void CRenderViewMainWindow::deleteStoredImage()
{
   m_renderView.deleteStoredImage();

}

void CRenderViewMainWindow::progressiveRefinement()
{
   K_progressive = m_action_progressive_refinement->isChecked();
   m_renderView.interruptRender();
   m_renderView.refresh();
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

   if (!m_renderView.m_region_crop || !m_leftButtonDown) return;
   
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

   // simply pressed the mouse

   if (!m_renderView.m_region_crop) return;

/*
   if ((event->buttons() & Qt::RightButton) && m_leftButtonDown)
   {
      // if for example we right-click while we're draggin the region,
      // just cancel the region
      m_leftButtonDown = false;
      m_pickPoint[0] = m_pickPoint[1] = -1;
      m_renderView.m_gl->clearRegionCrop();
      return;
   }
*/

   if(!(event->buttons() & Qt::LeftButton)) return;

   m_leftButtonDown = true;
      
   // get mouse position
   // set in m_pickPoint
   m_pickPoint[0] = event->x();
   m_pickPoint[1] = event->y();


}
void CRenderViewMainWindow::mouseReleaseEvent( QMouseEvent * event )
{
   if (!m_renderView.m_region_crop || !m_leftButtonDown) return;
   
   
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

      m_renderView.refresh();
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
   
   m_renderView.refresh();

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

   m_aovs_action_group->addAction(action);
   m_menu_aovs->addSeparator();

   const std::vector<std::string> &aovNames = m_renderView.m_aovNames;

   for (int i = 0; i < (int)aovNames.size(); ++i)
   {
      action = m_menu_aovs->addAction(QString(aovNames[i].c_str()));
      connect(action, SIGNAL(triggered()), this, SLOT(showAOV()));
      action->setCheckable(true);
      if (m_renderView.m_displayedAovIndex == i) action->setChecked(true);
      m_aovs_action_group->addAction(action);
   }
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
   m_renderView.refreshGLBuffer();
   m_renderView.draw();
}

void CRenderViewMainWindow::cropRegion()
{
   m_renderView.m_region_crop = m_action_crop_region->isChecked();
   if (!m_renderView.m_region_crop)
   {
      m_renderView.m_gl->clearRegionCrop();
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



// If you add some slots, you'll have to run moc
#include "renderview.moc"
