/*
 * OpenGL display shader and buffer
 */

#include <ai.h>

#include <math.h>
#include <string.h>
#include <sstream>
#include <iostream>

#include <maya/MImage.h>
#include "render_gl_widget.h"
#include "renderview.h"
#include "render_loop.h" 


static const AtRGBA FILL_COLOR = {0.09f, 0.09f, 0.09f, 1.0f};
static const AtRGBA8 EMPTY_FILL_COLOR = {13, 13, 13, 255};

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
   return atof((const char*)glGetString(GL_VERSION)) >= 2.0;
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
   {
      m_front_buffer[i] = EMPTY_FILL_COLOR;
   }

   m_back_buffer = NULL;
   setMouseTracking(true);
   clearRegionCrop();
   m_pan[0] = m_pan[1] = 0;
   m_zoomFactor = 1.f;

   m_printGPUState = false;
   m_bg_data = NULL;
   m_bg_color = AI_RGBA_BLACK;
}

CRenderGLWidget::~CRenderGLWidget()
{
   if (m_texture)
   {
      glDeleteTextures(1, &m_texture);
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
   glGenTextures(1, &m_texture);
   GL_print_error("create texture");

   glBindTexture(GL_TEXTURE_2D, m_texture);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   GL_print_error("set texture parameter");

   // describe texture layout
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_width);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
   GL_print_error("set pixel store");

   // set texture data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_front_buffer);
   GL_print_error("set texture pixels");

   glPopClientAttrib();
}



void CRenderGLWidget::initializeGL()
{

   makeCurrent();
    // set viewport
   glViewport(0, 0, m_width, m_height); //used to be x_res/y_res
   glOrtho(0, m_width, 0, m_height, -1, 1);

   GL_print_error("setup viewport");
   
   // create texture
   setupTexture();

}

void CRenderGLWidget::resizeGL(int width, int height)
{
}

void CRenderGLWidget::paintGL()
{
   // check time
   // if higher than low_freq -> continue

   AtDisplaySync *sync = m_renderview.displaySync();

//  commenting this line for now.
//  it looks like we should not block paintGL, as Qt can call it 
//  in some situations (resizing, menus appearing/disappearing).
//  We've had some buffers disappearing until waiting_draw was set back to true.
//   if (!sync->waiting_draw) return;


   if (m_printGPUState) {
      printGPUState();
      m_printGPUState = false;
   }

   GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
   if (depthTest == GL_TRUE)
      glDisable(GL_DEPTH_TEST);

   //check timer 
   //if (!time_out since last paint) return

   AtBBox2 region;
   AiCritSecEnter(&sync->lock);

   region = sync->front_update_region;

   sync->waiting_draw = false;
   sync->front_update_region.minx = m_width;
   sync->front_update_region.miny = m_height;
   sync->front_update_region.maxx = sync->front_update_region.maxy = -1;


   AiCritSecLeave(&sync->lock);

   makeCurrent();


   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   int imageSize[2];
   imageSize[0] = int (m_width * m_zoomFactor);
   imageSize[1] = int(m_height * m_zoomFactor);

   
   int zoomOffset[2];
   zoomOffset[0] = int ((m_width - imageSize[0]) * 0.5);
   zoomOffset[1] = int ((m_height - imageSize[1]) * 0.5);


   int windowSizeOffset[2];
   windowSizeOffset[0] = int( (parentWidget()->width() - m_width) * 0.5 );
   windowSizeOffset[1] = int( (parentWidget()->height() - m_height) * 0.5);
   
   glViewport(m_pan[0] + zoomOffset[0] + windowSizeOffset[0],  -m_pan[1] + zoomOffset[1] + windowSizeOffset[1], imageSize[0], imageSize[1]);
   glOrtho(0, m_width, 0, m_height, -1, 1);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   displayBuffer(m_renderview.m_reg_x, m_renderview.m_reg_y, &region, m_renderview.m_color_mode, false);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   if (depthTest == GL_TRUE)
      glEnable(GL_DEPTH_TEST);



   m_renderview.checkSceneUpdates();

}



void CRenderGLWidget::copyToBackBuffer()
{
   const size_t size = m_width * m_height * sizeof(AtRGBA8);

   // allocate buffer on demand
   if (!m_back_buffer)
      m_back_buffer = (AtRGBA8 *)AiMalloc(size);

   memcpy(m_back_buffer, m_front_buffer, size);
}


void CRenderGLWidget::reloadBuffer(AtRvColorMode color_mode)
{
   makeCurrent();

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();


   glViewport(0, 0, m_width, m_height);
   glOrtho(0, m_width, 0, m_height, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   AtBBox2 region;
   region.minx = region.miny = 0;
   region.maxx = m_width;
   region.maxy = m_height;

   displayBuffer(m_width, m_height, &region, color_mode, false);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   update();
}


void CRenderGLWidget::displayBuffer(int w, int h, const AtBBox2 *update_region, AtRvColorMode color_mode, bool back_buffer)
{
   glClearColor(FILL_COLOR.r, FILL_COLOR.g, FILL_COLOR.b, FILL_COLOR.a);
   //glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
   glClear(GL_COLOR_BUFFER_BIT);
   GL_print_error("clear buffer");

   // bind and update texture
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

   if (m_bg_data)
   {
      glBindTexture(GL_TEXTURE_2D, m_bg_data->textureId);
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      glPushMatrix();

      glTranslatef(m_bg_data->offset.x, m_bg_data->offset.y, 0.f);
      glScalef(m_bg_data->scale.x, m_bg_data->scale.y, 1.f);

      if (update_region)
      {
         AtRGBA8 *buffer = m_bg_data->bgBuffer;

         glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_bg_data->bgWidth);
         glPixelStorei(GL_UNPACK_SKIP_PIXELS, update_region->minx);
         glPixelStorei(GL_UNPACK_SKIP_ROWS,   update_region->miny);

         if (color_mode == COLOR_MODE_RGBA)
         {
            
            if (update_region->maxx > update_region->minx && update_region->maxy > update_region->miny)
            {
               glTexSubImage2D(GL_TEXTURE_2D, 0,
                  update_region->minx, update_region->miny,
                  update_region->maxx - update_region->minx,
                  update_region->maxy - update_region->miny,
                  GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            }
         }
         else
         {
            glBindTexture(GL_TEXTURE_2D, m_bg_data->textureId);

            AtUInt8 *tmp_buffer = (AtUInt8 *)AiMalloc(sizeof(AtUInt8) * m_bg_data->bgWidth * m_bg_data->bgHeight);
            AtUInt8 *src_buffer = (AtUInt8 *)buffer;
            
            if (color_mode == COLOR_MODE_R) src_buffer += 0;
            else if (color_mode == COLOR_MODE_G) src_buffer += 1;
            else if (color_mode == COLOR_MODE_B) src_buffer += 2;
            else if (color_mode == COLOR_MODE_A) src_buffer += 3;

            for (unsigned int i = 0; i < m_bg_data->bgWidth * m_bg_data->bgHeight; i++)
               tmp_buffer[i] = src_buffer[i*4];

            glTexSubImage2D(GL_TEXTURE_2D, 0,
               update_region->minx, update_region->miny,
               update_region->maxx - update_region->minx,
               update_region->maxy - update_region->miny,
               GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp_buffer);

            AiFree(tmp_buffer);
         }
         
         GL_print_error("update texture pixels");
      }



      // draw texture mapped quad with fixed function pipeline
      glEnable(GL_TEXTURE_2D);
      glColor3f(1, 1, 1);

      glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f);
         glVertex2f(0, h);

         glTexCoord2f(0.0f, m_ty);
         glVertex2f(0, 0);

         glTexCoord2f(m_tx, m_ty);
         glVertex2f(w, 0);

         glTexCoord2f(m_tx, 0.0f);
         glVertex2f(w, h);
      glEnd();


      glPopMatrix();

      glDisable(GL_TEXTURE_2D);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

   } else if (m_bg_color.r > AI_EPSILON || m_bg_color.g > AI_EPSILON || m_bg_color.b > AI_EPSILON )
   {
      glEnable(GL_BLEND);
      glDisable(GL_TEXTURE_2D);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
      glColor3f(m_bg_color.r, m_bg_color.g, m_bg_color.b);
      glBegin(GL_QUADS);
         glVertex2f(0, h);
         glVertex2f(0, 0);
         glVertex2f(w, 0);
         glVertex2f(w, h);
      glEnd();
   }
   glBindTexture(GL_TEXTURE_2D, m_texture);
   if (update_region)
   {
      AtRGBA8 *buffer = (back_buffer && m_back_buffer) ? m_back_buffer : m_front_buffer;
      
      glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_width);
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, update_region->minx);
      glPixelStorei(GL_UNPACK_SKIP_ROWS,   update_region->miny);

      if (color_mode == COLOR_MODE_RGBA)
      {
         
         if (update_region->maxx > update_region->minx && update_region->maxy > update_region->miny)
         {

            glTexSubImage2D(GL_TEXTURE_2D, 0,
               update_region->minx, update_region->miny,
               update_region->maxx - update_region->minx,
               update_region->maxy - update_region->miny,
               GL_RGBA, GL_UNSIGNED_BYTE, buffer);

         }
      }
      else
      {

         glBindTexture(GL_TEXTURE_2D, m_texture);

         AtUInt8 *tmp_buffer = (AtUInt8 *)AiMalloc(sizeof(AtUInt8) * m_width * m_height);
         AtUInt8 *src_buffer = (AtUInt8 *)buffer;

         
         if (color_mode == COLOR_MODE_R) src_buffer += 0;
         else if (color_mode == COLOR_MODE_G) src_buffer += 1;
         else if (color_mode == COLOR_MODE_B) src_buffer += 2;
         else if (color_mode == COLOR_MODE_A) src_buffer += 3;

         for (int i = 0; i < m_width * m_height; i++)
            tmp_buffer[i] = src_buffer[i*4];

         glTexSubImage2D(GL_TEXTURE_2D, 0,
            update_region->minx, update_region->miny,
            update_region->maxx - update_region->minx,
            update_region->maxy - update_region->miny,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, tmp_buffer);

         AiFree(tmp_buffer);
      }
      GL_print_error("update texture pixels");
   }
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);  
   // draw texture mapped quad with fixed function pipeline
   glEnable(GL_TEXTURE_2D);
   glColor3f(1, 1, 1);

   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0, h);

      glTexCoord2f(0.0f, m_ty);
      glVertex2f(0, 0);

      glTexCoord2f(m_tx, m_ty);
      glVertex2f(w, 0);

      glTexCoord2f(m_tx, 0.0f);
      glVertex2f(w, h);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   glPopClientAttrib();

    // Draw a border to the buffer
   if (m_regionCrop)
   {
      glColor3f(1.0f, 0.1f, 0.1f);
      glBegin(GL_LINE_LOOP);
      glVertex2d(m_region.minx, m_height - m_region.maxy);
      glVertex2d(m_region.maxx, m_height - m_region.maxy);
      glVertex2d(m_region.maxx, m_height - m_region.miny);
      glVertex2d(m_region.minx, m_height - m_region.miny);
      glEnd();
   }

   GL_print_error("draw texture");
}

void CRenderGLWidget::project(int windowX, int windowY, int& bufferX, int &bufferY, bool clamp) const
{
   int imageSize[2];
   imageSize[0] = int (m_width * m_zoomFactor);
   imageSize[1] = int(m_height * m_zoomFactor);

   
   int zoomOffset[2];
   zoomOffset[0] = int ((m_width - imageSize[0]) * 0.5);
   zoomOffset[1] = int ((m_height - imageSize[1]) * 0.5);

   int windowSizeOffset[2];
   windowSizeOffset[0] = int( (parentWidget()->width() - m_width) * 0.5 );
   windowSizeOffset[1] = int( (parentWidget()->height() - m_height) * 0.5);


   int startBuffer[2];
   startBuffer[0] =   m_pan[0] + zoomOffset[0] + windowSizeOffset[0];
   startBuffer[1] =  m_pan[1] + zoomOffset[1] + windowSizeOffset[1];
   
   bufferX = int((windowX - startBuffer[0])/ m_zoomFactor);
   bufferY = int((windowY - startBuffer[1])/ m_zoomFactor);

   if (clamp)
   {
      bufferX = MIN(MAX(bufferX, 0), m_width);
      bufferY = MIN(MAX(bufferY, 0), m_height);
   }

}

void CRenderGLWidget::setRegionCrop(int start_x, int start_y, int end_x, int end_y)
{
   m_region.minx = start_x;
   m_region.miny = start_y;
   m_region.maxx = end_x;
   m_region.maxy = end_y;  
   m_regionCrop = true; 
}

void CRenderGLWidget::clearRegionCrop()
{
   m_regionCrop = false;
   m_region.minx = m_region.miny = m_region.maxx = m_region.maxy = 0;
}


void CRenderGLWidget::setBackgroundImage(const std::string &filename)
{
   MImage bgImg;
   MStatus status = bgImg.readFromFile(MString(filename.c_str()));
   if (m_bg_data)
   {
      delete m_bg_data;
      m_bg_data = NULL;
   }

   if (status != MS::kSuccess) return;
   
   m_bg_data = new BackgroundData();

   bgImg.getSize(m_bg_data->bgWidth, m_bg_data->bgHeight);
   bgImg.verticalFlip();
   m_bg_data->bgBuffer = (AtRGBA8 *)AiMalloc(m_bg_data->bgWidth * m_bg_data->bgHeight * sizeof(AtRGBA8));

   unsigned char *pixels = bgImg.pixels();
   unsigned int size = m_bg_data->bgWidth * m_bg_data->bgHeight;

   for (unsigned int i = 0; i < size; ++i)
   {
      
      AtRGBA8 &bufferPixel = m_bg_data->bgBuffer[i];
      bufferPixel.r = *pixels;pixels++;
      bufferPixel.g = *pixels;pixels++;
      bufferPixel.b = *pixels;pixels++;
      bufferPixel.a = *pixels;pixels++;
   }
   // create texture
   glGenTextures(1, &m_bg_data->textureId);
   GL_print_error("create texture");

   glBindTexture(GL_TEXTURE_2D, m_bg_data->textureId);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   GL_print_error("set texture parameter");

   // describe texture layout
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_bg_data->bgWidth);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
   GL_print_error("set pixel store");

   // set texture data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_bg_data->bgWidth, m_bg_data->bgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bg_data->bgBuffer);
   GL_print_error("set texture pixels");

   glPopClientAttrib();

}

void CRenderGLWidget::setBackgroundChecker()
{
   if (m_bg_data)
   {
      delete m_bg_data;
      m_bg_data = NULL;
   }
   
   m_bg_data = new BackgroundData();

   m_bg_data->bgWidth = m_width;
   m_bg_data->bgHeight = m_height;

   m_bg_data->bgBuffer = (AtRGBA8 *)AiMalloc(m_width * m_height * sizeof(AtRGBA8));

   int pixelIndex = 0;
   int checkerSize = 20;
   int xChecker;
   int yChecker;

   for (int y = 0; y < m_height; ++y)
   {
      for (int x = 0; x < m_width; ++x, ++pixelIndex)
      {
         AtRGBA8 &bufferPixel = m_bg_data->bgBuffer[pixelIndex];
         xChecker = x/checkerSize;
         yChecker = y/checkerSize;

         if (((xChecker)%2) == ((yChecker)%2))
         {
            bufferPixel.r = bufferPixel.g = bufferPixel.b = 250;
         } else
         {
            bufferPixel.r = bufferPixel.g = bufferPixel.b = 190;
         }

         bufferPixel.a = 0;
      }
   }
      
   // create texture
   glGenTextures(1, &m_bg_data->textureId);
   GL_print_error("create texture");

   glBindTexture(GL_TEXTURE_2D, m_bg_data->textureId);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   GL_print_error("set texture parameter");

   // describe texture layout
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_bg_data->bgWidth);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
   GL_print_error("set pixel store");

   // set texture data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_bg_data->bgWidth, m_bg_data->bgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bg_data->bgBuffer);
   GL_print_error("set texture pixels");

   glPopClientAttrib();

}