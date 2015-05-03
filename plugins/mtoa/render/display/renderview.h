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
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QtOpenGL>


#include <QtCore/qthread.h>
#include <iostream>

struct AtDisplaySync;

class CRenderGLWidget;



enum AtKickColorMode
{
   COLOR_MODE_RGBA = 0,
   COLOR_MODE_R,
   COLOR_MODE_G,
   COLOR_MODE_B,
   COLOR_MODE_A,
};

struct AtRGBA8
{
   AtUInt8 r, g, b, a;
};

extern AtNodeMethods *kick_driver_mtd;

class CRenderView
{
public:

   // create and destroy window
   CRenderView(int w, int h);

   ~CRenderView();

   void init(int w, int h);

   // request to close the window
   void close();
   void show();

   void interruptRender();
   // properties
   int width() { return m_width; }
   int height() { return m_height; }

   AtRGBA8 *getBuffer(); 
   int getBufferWidth() const;
   int getBufferHeight() const;

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
   // OpenGL display
   

   // display options
   bool dither;
   float gamma;
   AtKickColorMode color_mode;

   // render syncing
   

protected:
   int m_width;
   int m_height;
   QMainWindow *m_main_window;
   CRenderGLWidget *m_gl;
   AtDisplaySync *display_sync;

   void *m_render_thread;


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


 