/*
 * Common header file for macros, data types, prototypes and global vars
 * 
 * --------------------------------------------------------------------------
 * --------------------------------------------------------------------------
 */

#pragma once


#include <ai.h>

#include <math.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "shading_manager.h"

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <QtGui/qevent.h>
#include <QtGui/qmainwindow.h>

#include "render_gl_widget.h"
#include <QtCore/qbytearray.h>

#include <QtCore/qthread.h>
#include <iostream>
#include "luts.h"


struct AtDisplaySync;
class CRenderViewManipulator;
class CRenderGLWidget;
class QMenu;


class CRenderView;

class CRenderViewMainWindow : public QMainWindow
{

Q_OBJECT
 
public:
   CRenderViewMainWindow(QWidget *parent, CRenderView &rv) : QMainWindow(parent, Qt::Tool/* Qt::WindowStaysOnTopHint*/), 
      m_renderView(rv), 
      m_menu_file(NULL),
      m_menu_view(NULL),
      m_menu_render(NULL),
      m_menu_aovs(NULL),
      m_menu_camera(NULL),
      m_tool_bar(NULL),
      m_aovs_combo(NULL),
      m_cameras_combo(NULL),
      m_cc_window(NULL),
      m_manipulator(NULL) {}
    ~CRenderViewMainWindow();
 
   void initMenus();
   void populateAOVsMenu();
   void populateCamerasMenu();

   void enableMenus(bool b)
   {
      if (b == m_active_menus) return;

      m_render_action->setEnabled(b);
      m_action_auto_refresh->setEnabled(b);
      m_action_crop_region->setEnabled(b);
      m_active_menus = b;
   }
   
private:

   void updateStoredSlider();

   CRenderView &m_renderView;
   QMenu *m_menu_file;
   QMenu *m_menu_view;
   QMenu *m_menu_render;
   QMenu *m_menu_aovs;
   QMenu *m_menu_camera;

   QToolBar *m_tool_bar;
   QComboBox *m_aovs_combo;
   QComboBox *m_cameras_combo;

   QAction *m_action_show_rendering_tiles;
   QAction *m_action_auto_refresh;
   QAction *m_action_progressive_refinement;
   QAction *m_action_enable_aovs;
   QAction *m_action_crop_region;
   QAction *m_action_status_bar;
   QAction *m_action_status_info;
   QAction *m_stored_slider_action;
   QAction *m_delete_stored_action;
   QAction *m_render_action;
   QAction *m_abort_action;
   QAction *m_3d_manipulation_action;
   QAction *m_lut_action;

   QActionGroup *m_channel_action_group;
   QActionGroup *m_aovs_action_group;
   QActionGroup *m_cameras_action_group;
   QActionGroup *m_debug_shading_action_group;
   

   QSlider *m_stored_slider;
   CRenderViewCCWindow *m_cc_window;

   bool m_leftButtonDown;
   int  m_pickPoint[2];

   CRenderViewManipulator *m_manipulator;
   bool  m_active_menus;
   bool  m_3d_manipulation;
   
protected:
   void mouseMoveEvent ( QMouseEvent * event );
   void mousePressEvent ( QMouseEvent * event );
   void mouseReleaseEvent ( QMouseEvent * event );
   void resizeEvent(QResizeEvent *event);
   void moveEvent(QMoveEvent *event);
   void wheelEvent ( QWheelEvent * event );
   void closeEvent(QCloseEvent *event);

private:

private slots:
   void saveImage();
   void abortRender();
   void showRenderingTiles();
   void autoRefresh();
   void updateRender();
   void storeImage();
   void previousStoredImage();
   void nextStoredImage();
   void deleteStoredImage();
   void progressiveRefinement();
   void showChannel();
   void enableAOVs();
   void showAOV();
   void showBoxAOV();
   void selectCamera();
   void selectBoxCamera();
   void cropRegion();
   void frameAll();
   void frameRegion();
   void realSize();
   void colorCorrection();
   void enableStatusBar();
   void displayPixelInfo();
   void storedSliderMoved(int);
   void toggleManipulationMode();
   void debugShading();

// If you add a slot to this class,
// don't forget to run
// moc  renderview.h -o renderview.moc
   
};
 

extern AtNodeMethods *kick_driver_mtd;

class CRenderView
{
public:

   // create and destroy window
   CRenderView(int w, int h);

   ~CRenderView();

   
   // request to close the window
   void close();
   void show();

   void interruptRender();
   void finishRender();

   // properties
   int width() { return m_width; }
   int height() { return m_height; }

   AtRGBA *getBuffer() {return m_buffer;} 
   //AtRGBA *getRGBABuffer(); 

   int getWidth() const {return m_width;}
   int getHeight() const{return m_height;}

   void render();

   // utility functions, time is in microseconds
   static AtUInt64 time();
   static void syncPause();
   static void sleep(AtUInt64 usecs);

   void draw(AtBBox2 *region = 0);
   void draw(int xmin, int ymin, int width, int height)
   {
      AtBBox2 box;
      box.minx = xmin;
      box.miny = ymin;
      box.maxx = xmin + width;
      box.maxy = ymin + height;
      draw(&box);
   }

   CRenderGLWidget *getGlWidget() {return m_gl;}

   void displaySyncCreate();
   AtDisplaySync *displaySync() {return display_sync;}

   // image parameters
   int m_x_res, m_y_res;        // window and image size
   int m_bucket_size;
   int m_min_x, m_min_y;        // region window offset
   int m_reg_x, m_reg_y;        // region window size

   // OS window handle
   AtCritSec m_window_close_lock;
   AtCritSec m_pick_lock;

   bool canRestartRender() const;
   void restartRender();
   void updateRender();
   void updateRenderOptions();

   void saveImage(const std::string &filename);
   void storeImage();
   void showPreviousStoredImage();
   void showNextStoredImage();
   void deleteStoredImage();
   CRenderViewMainWindow *getMainWindow() {return m_main_window;}

   
   // this method doesn't check for boundaries
   void setPixelColor(int x, int y, const AtRGBA &rgba)
   {
      int pixel_index = y * m_width + x;

      m_buffer[pixel_index] = rgba;

      if (m_displayedImageIndex < 0 && m_displayedAovIndex < 0)
      {
         fillGLPixel(rgba, x, y, pixel_index);
      }
   }

   // this method doesn't check for boundaries
   void setAOVPixelColor(int aovIndex, int x, int y, const AtRGBA &rgba)
   {
      int pixel_index = y * m_width + x;

      m_aovBuffers[aovIndex][pixel_index] = rgba;

      if (m_displayedAovIndex == aovIndex && m_displayedImageIndex < 0)
      {
         fillGLPixel(rgba, x, y, pixel_index);
      }
   }


   void setPixelsBlock(int minx, int miny, int maxx, int maxy, const AtRGBA &rgba, int aovIndex = -1)
   {
      // Apply LUTs just once for the whole block of pixels
      AtRGBA outColor(rgba);
      AtRGBA *rgbaBuffer = (aovIndex < 0) ? m_buffer : m_aovBuffers[aovIndex];

      // is this buffer being currently displayed ?
      bool displayGL = (m_displayedImageIndex < 0 && m_displayedAovIndex == aovIndex);
      
      if (displayGL)
      {
         applyLUTs(outColor);

         // if picking in progress, compare the picked ID with the ID AOV
         if (m_picked_id)
         {
            AiCritSecEnter(&m_pick_lock);
            //int pixel_id = *((int*)(&m_aovBuffers.back()[x + y * m_width].r));
            int pixel_id = reinterpret_type<float, int>(m_aovBuffers.back()[minx + miny * m_width].r);

            if ( m_picked_id && pixel_id == *m_picked_id )
            {
               // mix the color with white (50%)
               outColor.rgb() *= 0.5f;
               outColor.rgb() += 0.5f;
            }
            AiCritSecLeave(&m_pick_lock);
         }

         AtRGBA8 rgba8;
         rgba8.r = AiQuantize8bit(minx, miny, 0, outColor.r, m_colorCorrectSettings.dither);
         rgba8.g = AiQuantize8bit(minx, miny, 1, outColor.g, m_colorCorrectSettings.dither);
         rgba8.b = AiQuantize8bit(minx, miny, 2, outColor.b, m_colorCorrectSettings.dither);
         rgba8.a = AiQuantize8bit(minx, miny, 2, outColor.a, m_colorCorrectSettings.dither);

         AtRGBA8 *glBuffer = m_gl->getBuffer();      
         int pixelIndex;
         for (int y = miny; y < maxy; ++y)
         {
            pixelIndex = minx + (y * m_width);
            for (int x = minx; x < maxx; ++x, pixelIndex++)
            {
               // original color
               rgbaBuffer[pixelIndex] = rgba;

               // Color-Corrected RGBA8 color
               glBuffer[pixelIndex] = rgba8;            
            }
         }
      } else
      {
         // This buffer is not being displayed
         // so just fill RgbaBuffer with the original, unmodified color
         int pixelIndex;
         for (int y = miny; y < maxy; ++y)
         {
            pixelIndex = minx + (y * m_width);
            for (int x = minx; x < maxx; ++x, pixelIndex++)
            {
               // original color
               rgbaBuffer[pixelIndex] = rgba;
            }
         }
      }
   }

   void setShowRenderingTiles(bool b) {m_show_rendering_tiles = b;}
   bool getShowRenderingTiles() const {return m_show_rendering_tiles;}
   
   AtRvColorMode m_color_mode;

   void refreshGLBuffer();
   void checkSceneUpdates();

   void setStatus (const std::string &status)
   {
      m_status_log = status;
      m_status_changed = true;
   }
   void refreshStatusBar(int *mouse_position = NULL);

   void restoreContinuous() {if (m_continuous_updates)m_restore_continuous = true;}
   void pickShape(int x, int y);
   void clearPicking();
   void setDebugShading(RenderViewDebugShading d);
   void ObjectNameChanged(const std::string &new_name, const std::string &old_name);

   static void SelectionChangedCallback(void *);
   
protected:

friend class CRenderViewMainWindow;

   void init();
   void initSize(int w, int h);

   void fillGLPixel(const AtRGBA &color, int x, int y, int pixelIndex)
   {
      AtRGBA outColor(color);
      applyLUTs(outColor);
      
      // if picking in progress, compare the picked ID with the ID AOV
      if (m_picked_id)
      {
         AiCritSecEnter(&m_pick_lock);
         //int pixel_id = *((int*)(&m_aovBuffers.back()[x + y * m_width].r));
         int pixel_id = reinterpret_type<float, int>(m_aovBuffers.back()[pixelIndex].r);

         if ( m_picked_id && pixel_id == *m_picked_id )
         {
            // mix the color with white (50%)
            outColor.rgb() *= 0.5f;
            outColor.rgb() += 0.5f;
         }
         AiCritSecLeave(&m_pick_lock);
      
      }

      // Now let'sfill the GLWidget's RGBA8 buffer
      AtRGBA8 &rgba8 = m_gl->getBuffer()[pixelIndex];
      rgba8.r = AiQuantize8bit(x, y, 0, outColor.r, m_colorCorrectSettings.dither);
      rgba8.g = AiQuantize8bit(x, y, 1, outColor.g, m_colorCorrectSettings.dither);
      rgba8.b = AiQuantize8bit(x, y, 2, outColor.b, m_colorCorrectSettings.dither);
      rgba8.a = AiQuantize8bit(x, y, 2, outColor.a, m_colorCorrectSettings.dither);

   }

   void applyLUTs(AtRGBA &color)
   {
      if (m_displayID)
      {
         // basic pseudo-random color per ID.
         // I tried using AiVCellNoise3, but some geometries 
         // were displayed with the same color. 
         // For now let's use this dirty hash         
         unsigned int val;
         //val = *((unsigned int*)&rgba.r);
         val = reinterpret_type<float, unsigned int>(color.r);
         color.r = (float((val+943) % 1257))/1257.f;
         color.g = (float((val+189) % 438))/438.f;
         color.b = (float((val+789) % 939))/939.f;
         return;
      }        
      
      AtRGB &rgb = color.rgb();
     
      if (m_colorCorrectSettings.lut3d)
      {
         m_colorCorrectSettings.lut3d->applyLUT(rgb);
      }
      if (m_colorCorrectSettings.exposure != 0.f)
      {
         rgb *= m_colorCorrectSettings.exposureFactor;
      }
      if (m_colorCorrectSettings.gamma != 1.0f)
      {

         AiColorClamp(rgb, rgb, 0, 1);
         AiColorGamma(&rgb, m_colorCorrectSettings.gamma);
      }

      if (m_colorCorrectSettings.space == RV_COLOR_SPACE_SRGB)
      {
         //  (x <= 0.04045) ? x * (1.0 / 12.92) : powf((x + 0.055) * (1.0 / (1 + 0.055)), 2.4);

         rgb.r = (rgb.r <= 0.0031308f) ? rgb.r * 12.92f : (1.055) * powf(rgb.r,1.f/2.4f) - 0.055f;
         rgb.g = (rgb.g <= 0.0031308f) ? rgb.g * 12.92f : (1.055) * powf(rgb.g,1.f/2.4f) - 0.055f;
         rgb.b = (rgb.b <= 0.0031308f) ? rgb.b * 12.92f : (1.055) * powf(rgb.b,1.f/2.4f) - 0.055f;

      } else if (m_colorCorrectSettings.space == RV_COLOR_SPACE_REC709)
      {
         rgb.r = (rgb.r >= 0.018f) ? (1.099 * (powf(rgb.r, 0.45f))) - 0.099 : (4.5f * rgb.r);
         rgb.g = (rgb.g >= 0.018f) ? (1.099 * (powf(rgb.g, 0.45f))) - 0.099 : (4.5f * rgb.g);
         rgb.b = (rgb.b >= 0.018f) ? (1.099 * (powf(rgb.b, 0.45f))) - 0.099 : (4.5f * rgb.b);

         /*
            Reverse : From Rec709 To Lin
            if value >= 0.081:
            return ((value + 0.099) / 1.099) ** (1/0.45)
            else:
            return value / 4.5
         */
      }
   }
   

   AtRGBA *getDisplayedBuffer()
   {
      return (m_displayedImageIndex < 0) ? 
               ((m_displayedAovIndex < 0) ? m_buffer : m_aovBuffers[m_displayedAovIndex]) :
               m_storedImages[m_displayedImageIndex];
   }
   const std::string &getDisplayedStatus() const
   {
      return (m_displayedImageIndex < 0) ? m_status_log : m_storedImagesStatus[m_displayedImageIndex];
   }

   int m_width;
   int m_height;
   CRenderViewMainWindow *m_main_window;
   CRenderGLWidget *m_gl;
   AtDisplaySync *display_sync;
   
   bool m_show_rendering_tiles;
   bool m_progressive_refinement;
   bool m_region_crop; 

   void *m_render_thread;
   bool m_continuous_updates;


   AtRGBA *m_buffer;
   std::vector<AtRGBA *> m_storedImages;
   std::vector<AtRGBA *> m_aovBuffers;
   std::vector<std::string> m_aovNames;

   int m_displayedImageIndex;
   int m_displayedAovIndex;
   CRenderViewCCSettings m_colorCorrectSettings;

   bool m_displayID;

   int *m_picked_id;
   std::string m_status_log;
   std::vector<std::string> m_storedImagesStatus;
   bool m_status_changed;
   bool m_restore_continuous;
   bool m_status_bar_enabled;
   bool m_status_bar_pixel_info;
   CRvShadingManager  m_shading_manager;
   RenderViewDebugShading m_debug_shading;

   
};


// This structure comes from the Kick API
// it is not really adapted to Qt
// so several things here have to be changed / removed
struct AtDisplaySync
{
   // spin lock would be better here, but it's not in the API
   AtCritSec lock;
   AtCritSec event_lock;

   
   // last time a draw or interrupt was done
   AtUInt64 last_draw_time;
   AtUInt64 last_interrupt_time;

   // region to be updated
   AtBBox2 front_update_region;

   // back buffer to avoid screen tearing
   AtBBox2 back_update_region;
   bool back_buffer_ready;

   // do we need to draw or interrupt after a timeout?
   bool waiting_draw;
   bool waiting_interrupt;

   
   // for detecting full frame render completion
   int buckets_left;
   
   int previous_AA_samples;
   bool interrupted;
   
};


