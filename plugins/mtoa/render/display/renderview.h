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

struct AtDisplaySync;

class CRenderGLWidget;
class QMenu;





class CRenderView;

struct CRenderViewCCSettings
{
   CRenderViewCCSettings() :  gamma(1.0),
                              brightness(1.f),
                              srgb(true),
                              dither(true)
                              {}
   float gamma;
   float brightness;
   bool  srgb;
   bool  dither;
};

class CRenderViewCCWindow : public QMainWindow
{
Q_OBJECT

public:
   CRenderViewCCWindow(QWidget *parent, CRenderView &rv, CRenderViewCCSettings &ccSettings) : 
      QMainWindow(parent), 
      m_renderView(rv),
      m_colorCorrectSettings(ccSettings)
      {}

   virtual ~CRenderViewCCWindow() {}


   void init();

private:
   CRenderView &m_renderView;
   CRenderViewCCSettings &m_colorCorrectSettings;

   QLineEdit *m_gamma_edit;
   QSlider *m_gamma_slider;

   QLineEdit *m_brightness_edit;
   QSlider *m_brightness_slider;

   QCheckBox *m_dither_box;
   QCheckBox *m_srgb_box;



private slots:
   void gammaSliderChanged();
   void gammaTextChanged();
   void brightnessSliderChanged();
   void brightnessTextChanged();
   void ditherChanged();
   void srgbChanged();
};


class CRenderViewMainWindow : public QMainWindow
{

Q_OBJECT
 
public:
   CRenderViewMainWindow(QWidget *parent, CRenderView &rv) : QMainWindow(parent, Qt::WindowStaysOnTopHint), m_renderView(rv), m_cc_window(NULL) {}
    ~CRenderViewMainWindow() {}
 
   void initMenus();
private:

   void populateAOVsMenu();
   void populateCamerasMenu();


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
   QActionGroup *m_channel_action_group;
   QActionGroup *m_aovs_action_group;
   QActionGroup *m_cameras_action_group;

   CRenderViewCCWindow *m_cc_window;

   bool m_leftButtonDown;
   int  m_pickPoint[2];
   int  m_previousPan[2];
   float m_previousZoom;

protected:
   void mouseMoveEvent ( QMouseEvent * event );
   void mousePressEvent ( QMouseEvent * event );
   void mouseReleaseEvent ( QMouseEvent * event );
   void resizeEvent(QResizeEvent *event);
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

   void displaySyncCreate(int width, int height);
   AtDisplaySync *displaySync() {return display_sync;}

   // image parameters
   int x_res, y_res;        // window and image size
   int bucket_size;
   int min_x, min_y;        // region window offset
   int reg_x, reg_y;        // region window size

   // OS window handle
   AtCritSec window_close_lock;
   bool canRestartRender() const;
   void restartRender();
   void updateRender();
   
   void saveImage(const std::string &filename);
   void storeImage();
   void showPreviousStoredImage();
   void showNextStoredImage();
   void deleteStoredImage();

   
   // this method doesn't check for boundaries
   void setPixelColor(int x, int y, const AtRGBA &rgba)
   {
      int pixel_index = y * m_width + x;

      m_buffer[pixel_index] = rgba;

      if (m_displayedImageIndex < 0 && m_displayedAovIndex < 0)
      {
         // Now let'sfill the GLWidget's RGBA8 buffer
         AtRGBA8 &rgba8 = m_gl->getBuffer()[pixel_index];
         copyToRGBA8(rgba, rgba8, x, y);
      }
   }
   // this method doesn't check for boundaries
   void setAOVPixelColor(int aovIndex, int x, int y, const AtRGBA &rgba)
   {
      int pixel_index = y * m_width + x;

      m_aovBuffers[aovIndex][pixel_index] = rgba;

      if (m_displayedAovIndex == aovIndex && m_displayedImageIndex < 0)
      {
         // Now let'sfill the GLWidget's RGBA8 buffer
         AtRGBA8 &rgba8 = m_gl->getBuffer()[pixel_index];
         copyToRGBA8(rgba, rgba8, x, y);
      }
   }

   void setShowRenderingTiles(bool b) {m_show_rendering_tiles = b;}
   bool getShowRenderingTiles() const {return m_show_rendering_tiles;}
   
   AtRvColorMode m_color_mode;

   bool getRegionCrop(bool b){m_region_crop = b;}
   void refreshGLBuffer();
   void checkSceneUpdates();

   void setStatus (const std::string &status)
   {
      m_main_window->statusBar()->showMessage(QString(status.c_str()));
   }

protected:

friend CRenderViewMainWindow;

   void init();
   
   
   void copyToRGBA8(const AtRGBA &rgba, AtRGBA8 &rgba8, int x, int y)
   {
      const bool &dither = m_colorCorrectSettings.dither;
      // apply gamma
      if (true /*m_colorCorrectSettings.gamma != 1.0f*/)
      {
         // need to copy the input RGBA for the gamma
         AtRGBA color = rgba;
         AtRGB &rgb = color.rgb();
         
         if (m_colorCorrectSettings.brightness != 1.0f)
         {
            rgb *= m_colorCorrectSettings.brightness;
         }
         if (m_colorCorrectSettings.gamma != 1.0f)
         {

            AiColorClamp(rgb, rgb, 0, 1);
            AiColorGamma(&rgb, m_colorCorrectSettings.gamma);
         }

         if (m_colorCorrectSettings.srgb)
         {
            //  (x <= 0.04045) ? x * (1.0 / 12.92) : powf((x + 0.055) * (1.0 / (1 + 0.055)), 2.4);

            rgb.r = (rgb.r <= 0.0031308f) ? rgb.r * 12.92f : (1.055) * powf(rgb.r,1.f/2.4f) - 0.055f;
            rgb.g = (rgb.g <= 0.0031308f) ? rgb.g * 12.92f : (1.055) * powf(rgb.g,1.f/2.4f) - 0.055f;
            rgb.b = (rgb.b <= 0.0031308f) ? rgb.b * 12.92f : (1.055) * powf(rgb.b,1.f/2.4f) - 0.055f;

         }
         rgba8.r = AiQuantize8bit(x, y, 0, color.r, dither);
         rgba8.g = AiQuantize8bit(x, y, 1, color.g, dither);
         rgba8.b = AiQuantize8bit(x, y, 2, color.b, dither);
         rgba8.a = AiQuantize8bit(x, y, 3, color.a, dither);

      } else
      {  
         rgba8.r = AiQuantize8bit(x, y, 0, rgba.r, dither);
         rgba8.g = AiQuantize8bit(x, y, 1, rgba.g, dither);
         rgba8.b = AiQuantize8bit(x, y, 2, rgba.b, dither);
         rgba8.a = AiQuantize8bit(x, y, 3, rgba.a, dither);
      }
   }
   AtRGBA *getDisplayedBuffer()
   {
      return (m_displayedImageIndex < 0) ? 
               ((m_displayedAovIndex < 0) ? m_buffer : m_aovBuffers[m_displayedAovIndex]) :
               m_storedImages[m_displayedImageIndex];
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




 