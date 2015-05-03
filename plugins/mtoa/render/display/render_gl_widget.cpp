/*
 * OpenGL display shader and buffer
 */

#include <ai.h>

#include <math.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include "render_gl_widget.h"

#pragma warning (disable : 4244)
 
static const AtRGBA FILL_COLOR = {0.09f, 0.09f, 0.09f, 1.0f};
static const AtRGBA8 EMPTY_FILL_COLOR = {13, 233, 13, 255};

/* Non-power-of-two texture support */

inline int nearest_pow2(int num)
{
   unsigned int n = num > 0 ? num - 1 : 0;
   n |= n >>  1;
   n |= n >>  2;
   n |= n >>  4;
   n |= n >>  8;
   n |= n >> 16;
   n++;
   return (int)n;
}

inline bool support_non_power_of_two_textures(void)
{
   // OpenGL 2.0 and up supports non-power-of-two textures
   return atof((const char*)kglGetString(GL_VERSION)) >= 2.0;
}


/***************************************************
 *
 *     QGLWidget for render display
 *
 *************************************************/
CRenderGLWidget::CRenderGLWidget(QWidget *parent, CRenderView &rv, int width, int height) : QGLWidget(parent), m_renderview(rv)
{
   resize(width, height);
   // create display buffer
   if (support_non_power_of_two_textures())
   {
      // most cards support non-power-of-two textures now
      m_width = width;
      m_height = height;
   }
   else
   {
      // textures width/height rounded to power-of-two for old cards
      m_width = nearest_pow2(width);
      m_height = nearest_pow2(height);
   }


   // the texture coordinates for our quads -- we need to recompute this
   // each time we resize the texture because texture sizes are always
   // sized in multiples while the frame buffer might be an odd size.
   m_tx = (float)width / (float)m_width;
   m_ty = (float)height / (float)m_height;

   // allocate buffer
   const size_t size = m_width * m_height * sizeof(AtRGBA8);
   m_front_buffer = (AtRGBA8 *)AiMalloc(size);

   // fill with grey so we can see unfinished buckets
   const size_t fillsize = m_width * m_height;
   for(size_t i = 0; i < fillsize; i++)
      m_front_buffer[i] = EMPTY_FILL_COLOR;

   m_back_buffer = NULL;

}
CRenderGLWidget::~CRenderGLWidget()
{
   if (m_texture)
   {
      kglDeleteTextures(1, &m_texture);
      GL_print_error("delete texture");
   }

   if (m_back_buffer)
      AiFree(m_back_buffer);

   AiFree(m_front_buffer);
}
void CRenderGLWidget::closeEvent(QCloseEvent *event)
{
   
}


void CRenderGLWidget::setupTexture()
{
   m_texture = 0;

   // create texture
   kglGenTextures(1, &m_texture);
   GL_print_error("create texture");

   kglBindTexture(GL_TEXTURE_2D, m_texture);
   kglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   kglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   kglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   kglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   kglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   GL_print_error("set texture parameter");

   // describe texture layout
   kglPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   kglPixelStorei(GL_UNPACK_ROW_LENGTH,  m_width);
   kglPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
   kglPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
   GL_print_error("set pixel store");

   // set texture data
   kglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_front_buffer);
   GL_print_error("set texture pixels");

   kglPopClientAttrib();
}



void CRenderGLWidget::initializeGL()
{
   makeCurrent();
    // set viewport
   kglViewport(0, 0, m_width, m_height); //used to be x_res/y_res
   kglOrtho(0, m_width, 0, m_height, -1, 1);
   GL_print_error("setup viewport");

   // create texture
   setupTexture();
   
}

void CRenderGLWidget::resizeGL(int width, int height)
{
}

void CRenderGLWidget::paintGL()
{
   
   AtDisplaySync *sync = m_renderview.displaySync();

   if (!sync->waiting_draw) return;

   //check timer 
   //if (!time_out since last paint) return

   AtBBox2 region;
   AiCritSecEnter(&sync->lock);

   region = sync->front_update_region;

   sync->waiting_draw = false;
   sync->front_update_region.minx = sync->front_update_region.miny = std::numeric_limits<int>::max();
   sync->front_update_region.maxx = sync->front_update_region.maxy = -std::numeric_limits<int>::max();

   AiCritSecLeave(&sync->lock);

   makeCurrent();

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();


   kglViewport(0, 0, m_width, m_height);
   kglOrtho(0, m_width, 0, m_height, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   
   displayBuffer(m_renderview.reg_x, m_renderview.reg_y, &region, m_renderview.color_mode, false);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

}



void CRenderGLWidget::copyToBackBuffer()
{
   const size_t size = m_width * m_height * sizeof(AtRGBA8);

   // allocate buffer on demand
   if (!m_back_buffer)
      m_back_buffer = (AtRGBA8 *)AiMalloc(size);

   memcpy(m_back_buffer, m_front_buffer, size);
}



void CRenderGLWidget::displayBuffer(int w, int h, const AtBBox2 *update_region, AtKickColorMode color_mode, bool back_buffer)
{

   kglClearColor(FILL_COLOR.r, FILL_COLOR.g, FILL_COLOR.b, FILL_COLOR.a);
   kglClear(GL_COLOR_BUFFER_BIT);
   GL_print_error("clear buffer");

   // bind and update texture
   kglPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   kglBindTexture(GL_TEXTURE_2D, m_texture);

   if (update_region)
   {
      AtRGBA8 *buffer = (back_buffer && m_back_buffer) ? m_back_buffer : m_front_buffer;

      kglPixelStorei(GL_UNPACK_ROW_LENGTH,  m_width);
      kglPixelStorei(GL_UNPACK_SKIP_PIXELS, update_region->minx);
      kglPixelStorei(GL_UNPACK_SKIP_ROWS,   update_region->miny);

      if (color_mode == COLOR_MODE_RGBA)
      {
         kglTexSubImage2D(GL_TEXTURE_2D, 0,
            update_region->minx, update_region->miny,
            update_region->maxx - update_region->minx,
            update_region->maxy - update_region->miny,
            GL_RGBA, GL_UNSIGNED_BYTE, buffer);
      }
      else
      {
         AtUInt8 *tmp_buffer = (AtUInt8 *)AiMalloc(sizeof(AtUInt8) * m_width * m_height);
         AtUInt8 *src_buffer = (AtUInt8 *)buffer;

         if (color_mode == COLOR_MODE_R) src_buffer += 0;
         else if (color_mode == COLOR_MODE_G) src_buffer += 1;
         else if (color_mode == COLOR_MODE_B) src_buffer += 2;
         else if (color_mode == COLOR_MODE_A) src_buffer += 3;

         for (int i = 0; i < m_width * m_height; i++)
            tmp_buffer[i] = src_buffer[i*4];

         kglTexSubImage2D(GL_TEXTURE_2D, 0,
            update_region->minx, update_region->miny,
            update_region->maxx - update_region->minx,
            update_region->maxy - update_region->miny,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp_buffer);

         AiFree(tmp_buffer);
      }

      GL_print_error("update texture pixels");
   }

   // draw texture mapped quad with fixed function pipeline
   kglEnable(GL_TEXTURE_2D);
   kglColor3f(1, 1, 1);

   kglBegin(GL_QUADS);
      kglTexCoord2f(0.0f, 0.0f);
      kglVertex2f(0, h);

      kglTexCoord2f(0.0f, m_ty);
      kglVertex2f(0, 0);

      kglTexCoord2f(m_tx, m_ty);
      kglVertex2f(w, 0);

      kglTexCoord2f(m_tx, 0.0f);
      kglVertex2f(w, h);
   kglEnd();

   kglDisable(GL_TEXTURE_2D);

   // unset texture attributes
   kglPopClientAttrib();

   GL_print_error("draw texture");
}

