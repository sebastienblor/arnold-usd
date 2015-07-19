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

class CRenderViewMainWindow : public QMainWindow
{

Q_OBJECT
 
public:
   CRenderViewMainWindow(QWidget *parent, CRenderView &rv) : QMainWindow(parent, Qt::WindowStaysOnTopHint), m_renderView(rv){}
    ~CRenderViewMainWindow() {}
 
   void initMenus();
private:

   void populateAOVsMenu();

   CRenderView &m_renderView;
   QMenu *m_menu_file;
   QMenu *m_menu_view;
   QMenu *m_menu_render;
   QMenu *m_menu_aovs;

   QAction *m_action_show_rendering_tiles;
   QAction *m_action_auto_refresh;
   QAction *m_action_progressive_refinement;
   QAction *m_action_enable_aovs;
   QActionGroup *m_channel_action_group;
   QActionGroup *m_aovs_action_group;

 
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
   bool canRefresh() const;
   void refresh();
   
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


protected:

friend CRenderViewMainWindow;

   void init();
   void refreshGLBuffer();
   
   void copyToRGBA8(const AtRGBA &rgba, AtRGBA8 &rgba8, int x, int y)
   {
      // apply gamma
      if (m_gamma != 1.0f)
      {
         // need to copy the input RGBA for the gamma
         AtRGBA color = rgba;
         AtRGB &rgb = color.rgb();

         AiColorClamp(rgb, rgb, 0, 1);
         AiColorGamma(&rgb, m_gamma);
         rgba8.r = AiQuantize8bit(x, y, 0, color.r, m_dither);
         rgba8.g = AiQuantize8bit(x, y, 1, color.g, m_dither);
         rgba8.b = AiQuantize8bit(x, y, 2, color.b, m_dither);
         rgba8.a = AiQuantize8bit(x, y, 3, color.a, m_dither);

      } else
      {  
         rgba8.r = AiQuantize8bit(x, y, 0, rgba.r, m_dither);
         rgba8.g = AiQuantize8bit(x, y, 1, rgba.g, m_dither);
         rgba8.b = AiQuantize8bit(x, y, 2, rgba.b, m_dither);
         rgba8.a = AiQuantize8bit(x, y, 3, rgba.a, m_dither);
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
   bool m_dither;
   float m_gamma;
   bool m_show_rendering_tiles;
   bool m_progressive_refinement;
  

   void *m_render_thread;


   AtRGBA *m_buffer;
   std::vector<AtRGBA *> m_storedImages;
   std::vector<AtRGBA *> m_aovBuffers;
   std::vector<std::string> m_aovNames;

   int m_displayedImageIndex;
   int m_displayedAovIndex;
};


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


 