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

   K_AA_samples = AiNodeGetInt(options, "AA_samples");
   if (K_AA_samples == 0)
   K_AA_samples = 1;

   AiCritSecInit(&window_close_lock);

   K_render_window = true;
   K_progressive = true;

   unsigned int i;
   AtNode *filter;
   AtNode *driver;
   char kick_drv[1024];
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

   /*
   * duplicate the old outputs array
   */
   outputs = AiNodeGetArray(AiUniverseGetOptions(), "outputs");

   K_all_outputs = AiArrayAllocate(outputs->nelements+1, 1, AI_TYPE_STRING);
   for (i=0; i < outputs->nelements; i++)
     AiArraySetStr(K_all_outputs, i, AiArrayGetStr(outputs,i));
   /*
   * append our new outputs directive to the existing ones
   */
   sprintf(kick_drv, "RGBA RGBA %s %s", AiNodeGetName(filter), AiNodeGetName(driver));
   AiArraySetStr(K_all_outputs, outputs->nelements, kick_drv);


   /*
   * create an output with just the display driver, save it for later
   */
   K_display_output = AiArray(1, 1, AI_TYPE_STRING, kick_drv);
   /*
   * overwrite the old outputs line
   */
   AiNodeSetArray(AiUniverseGetOptions(), "outputs", AiArrayCopy(K_all_outputs));
   /*
   * configure the kick display driver -- make sure it has something
   * to store local information in
   */
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


void CRenderView::saveImage(const std::string &filename) const
{
   // Write down the displayed image

   // We're using QT stuff, but maybe we should use Maya's instead ?
   QImage outImg(m_width, m_height, QImage::Format_ARGB32 );
   AtRGBA *buffer = (m_displayedImageIndex < 0) ? m_buffer : m_storedImages[m_displayedImageIndex];

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
   AtRGBA *displayedBuffer = (m_displayedImageIndex < 0) ? m_buffer : m_storedImages[m_displayedImageIndex];
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


// If you add some slots, you'll have to run moc
#include "renderview.moc"
