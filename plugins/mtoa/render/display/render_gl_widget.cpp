/*
 * OpenGL display shader and buffer
 */

#include <ai.h>

#include <math.h>
#include <string.h>
#include <sstream>
#include <iostream>

#include "render_gl_widget.h"
#include "renderview.h"
#include "render_loop.h" 


static const AtRGBA FILL_COLOR = {0.09f, 0.09f, 0.09f, 1.0f};
static const AtRGBA8 EMPTY_FILL_COLOR = {13, 13, 13, 255};

/* Non-power-of-two texture support */

inline int NearestPow2(int num)
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

inline bool SupportNonPowerOfTwoTextures(void)
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
   m_texture = 0;
   m_frontBuffer = NULL;
   InitSize(width, height);
}

CRenderGLWidget::~CRenderGLWidget()
{
   if (m_texture)
   {
      glDeleteTextures(1, &m_texture);
      PrintGLError("delete texture");
   }

   AiFree(m_frontBuffer);
   
}

void CRenderGLWidget::InitSize(int width, int height)
{
   resize(width, height);
   // create display buffer
   
   if (SupportNonPowerOfTwoTextures())
   {
      // most cards support non-power-of-two textures now
      m_width = width;
      m_height = height;
   }
   else
   {
      // textures width/height rounded to power-of-two for old cards
      m_width = NearestPow2(width);
      m_height = NearestPow2(height);
   }

   // the texture coordinates for our quads -- we need to recompute this
   // each time we resize the texture because texture sizes are always
   // sized in multiples while the frame buffer might be an odd size.
   m_tx = (float)width / (float)m_width;
   m_ty = (float)height / (float)m_height;
   // allocate buffer
   const size_t size = m_width * m_height * sizeof(AtRGBA8);

   
   if (m_frontBuffer) AiFree(m_frontBuffer);
   m_frontBuffer = (AtRGBA8 *)AiMalloc(size);

   // fill with grey so we can see unfinished buckets
   const size_t fillsize = m_width * m_height;
   for(size_t i = 0; i < fillsize; i++)
   {
      m_frontBuffer[i] = EMPTY_FILL_COLOR;
   }

   setMouseTracking(true);
   ClearRegionCrop();
   m_pan[0] = m_pan[1] = 0;
   m_zoomFactor = 1.f;

   m_printGPUState = false;
   m_bgData = NULL;
   m_bgColor = AI_RGBA_BLACK;
}
void CRenderGLWidget::SetupTexture()
{
   if (m_texture)
   {
      glDeleteTextures(1, &m_texture);
      m_texture = 0;
   }

   // create texture
   glGenTextures(1, &m_texture);
   PrintGLError("create texture");

   glBindTexture(GL_TEXTURE_2D, m_texture);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   PrintGLError("set texture parameter");

   // describe texture layout
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_width);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
   PrintGLError("set pixel store");

   // set texture data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_frontBuffer);
   PrintGLError("set texture pixels");

   glPopClientAttrib();
}



void CRenderGLWidget::initializeGL()
{

   makeCurrent();
    // set viewport
   glViewport(0, 0, m_width, m_height); //used to be x_res/y_res
   glOrtho(0, m_width, 0, m_height, -1, 1);

   PrintGLError("setup viewport");
   
   // create texture
   SetupTexture();

}

void CRenderGLWidget::resizeGL(int width, int height)
{
}

void CRenderGLWidget::paintGL()
{
   // check time
   // if higher than low_freq -> continue

   AtDisplaySync *sync = m_renderview.DisplaySync();

//  commenting this line for now.
//  it looks like we should not block paintGL, as Qt can call it 
//  in some situations (resizing, menus appearing/disappearing).
//  We've had some buffers disappearing until waitingDraw was set back to true.
//   if (!sync->waitingDraw) return;


   if (m_printGPUState) {
      PrintGPUState();
      m_printGPUState = false;
   }

   GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
   if (depthTest == GL_TRUE)
      glDisable(GL_DEPTH_TEST);

   //check timer 
   //if (!time_out since last paint) return

   AtBBox2 region;
   AiCritSecEnter(&sync->lock);

   region = sync->frontUpdateRegion;

   sync->waitingDraw = false;
   sync->frontUpdateRegion.minx = m_width;
   sync->frontUpdateRegion.miny = m_height;
   sync->frontUpdateRegion.maxx = sync->frontUpdateRegion.maxy = -1;


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

   DisplayBuffer(m_renderview.m_regX, m_renderview.m_regY, &region, m_renderview.m_colorMode);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   if (depthTest == GL_TRUE)
      glEnable(GL_DEPTH_TEST);

   m_renderview.CheckSceneUpdates();
}


void CRenderGLWidget::ReloadBuffer(AtRvColorMode colorMode)
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

   DisplayBuffer(m_width, m_height, &region, colorMode);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   update();
}

void CRenderGLWidget::DisplayBuffer(int w, int h, const AtBBox2 *updateRegion, AtRvColorMode colorMode)
{

   glClearColor(FILL_COLOR.r, FILL_COLOR.g, FILL_COLOR.b, FILL_COLOR.a);
   //glClearColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
   glClear(GL_COLOR_BUFFER_BIT);
   PrintGLError("clear buffer");

   // bind and update texture
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

   if (m_bgData)
   {
      glBindTexture(GL_TEXTURE_2D, m_bgData->textureId);
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

      glPushMatrix();

      glTranslatef(m_bgData->offset.x, m_bgData->offset.y, 0.f);
      glScalef(m_bgData->scale.x, m_bgData->scale.y, 1.f);

      if (updateRegion)
      {
         AtRGBA8 *buffer = m_bgData->bgBuffer;

         glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_bgData->bgWidth);
         glPixelStorei(GL_UNPACK_SKIP_PIXELS, updateRegion->minx);
         glPixelStorei(GL_UNPACK_SKIP_ROWS,   updateRegion->miny);

         if (colorMode == COLOR_MODE_RGBA)
         {
            
            if (updateRegion->maxx > updateRegion->minx && updateRegion->maxy > updateRegion->miny)
            {
               glTexSubImage2D(GL_TEXTURE_2D, 0,
                  updateRegion->minx, updateRegion->miny,
                  updateRegion->maxx - updateRegion->minx,
                  updateRegion->maxy - updateRegion->miny,
                  GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            }
         }
         else
         {
            glBindTexture(GL_TEXTURE_2D, m_bgData->textureId);

            AtUInt8 *tmpBuffer = (AtUInt8 *)AiMalloc(sizeof(AtUInt8) * m_bgData->bgWidth * m_bgData->bgHeight);
            AtUInt8 *srcBuffer = (AtUInt8 *)buffer;
            
            if (colorMode == COLOR_MODE_R) srcBuffer += 0;
            else if (colorMode == COLOR_MODE_G) srcBuffer += 1;
            else if (colorMode == COLOR_MODE_B) srcBuffer += 2;
            else if (colorMode == COLOR_MODE_A) srcBuffer += 3;

            for (unsigned int i = 0; i < m_bgData->bgWidth * m_bgData->bgHeight; i++)
               tmpBuffer[i] = srcBuffer[i*4];

            glTexSubImage2D(GL_TEXTURE_2D, 0,
               updateRegion->minx, updateRegion->miny,
               updateRegion->maxx - updateRegion->minx,
               updateRegion->maxy - updateRegion->miny,
               GL_LUMINANCE, GL_UNSIGNED_BYTE, tmpBuffer);

            AiFree(tmpBuffer);
         }
         
         PrintGLError("update texture pixels");
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

   } else if (m_bgColor.r > AI_EPSILON || m_bgColor.g > AI_EPSILON || m_bgColor.b > AI_EPSILON )
   {
      glEnable(GL_BLEND);
      glDisable(GL_TEXTURE_2D);
      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
      glColor3f(m_bgColor.r, m_bgColor.g, m_bgColor.b);
      glBegin(GL_QUADS);
         glVertex2f(0, h);
         glVertex2f(0, 0);
         glVertex2f(w, 0);
         glVertex2f(w, h);
      glEnd();
   }
   glBindTexture(GL_TEXTURE_2D, m_texture);
   if (updateRegion)
   {
      AtRGBA8 *buffer = m_frontBuffer;
      
      glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_width);
      glPixelStorei(GL_UNPACK_SKIP_PIXELS, updateRegion->minx);
      glPixelStorei(GL_UNPACK_SKIP_ROWS,   updateRegion->miny);

      if (colorMode == COLOR_MODE_RGBA)
      {
         
         if (updateRegion->maxx > updateRegion->minx && updateRegion->maxy > updateRegion->miny)
         {

            glTexSubImage2D(GL_TEXTURE_2D, 0,
               updateRegion->minx, updateRegion->miny,
               updateRegion->maxx - updateRegion->minx,
               updateRegion->maxy - updateRegion->miny,
               GL_RGBA, GL_UNSIGNED_BYTE, buffer);

         }
      }
      else
      {

         glBindTexture(GL_TEXTURE_2D, m_texture);

         AtUInt8 *tmpBuffer = (AtUInt8 *)AiMalloc(sizeof(AtUInt8) * m_width * m_height);
         AtUInt8 *srcBuffer = (AtUInt8 *)buffer;

         
         if (colorMode == COLOR_MODE_R) srcBuffer += 0;
         else if (colorMode == COLOR_MODE_G) srcBuffer += 1;
         else if (colorMode == COLOR_MODE_B) srcBuffer += 2;
         else if (colorMode == COLOR_MODE_A) srcBuffer += 3;

         for (int i = 0; i < m_width * m_height; i++)
            tmpBuffer[i] = srcBuffer[i*4];

         glTexSubImage2D(GL_TEXTURE_2D, 0,
            updateRegion->minx, updateRegion->miny,
            updateRegion->maxx - updateRegion->minx,
            updateRegion->maxy - updateRegion->miny,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, tmpBuffer);

         AiFree(tmpBuffer);
      }
      PrintGLError("update texture pixels");
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
      glBegin(GL_LINE_LOOP);
      glVertex2d(m_region.minx - 2, m_height - m_region.miny - 2);
      glVertex2d(m_region.minx - 9, m_height - m_region.miny - 9);
      glEnd();
      glBegin(GL_LINE_LOOP);
      glVertex2d(m_region.minx - 9, m_height - m_region.miny - 2);
      glVertex2d(m_region.minx - 2, m_height - m_region.miny - 9);
      glEnd();
      glBegin(GL_LINE_LOOP);
      glVertex2d(m_region.minx, m_height - m_region.miny);
      glVertex2d(m_region.minx - 11, m_height - m_region.miny);
      glVertex2d(m_region.minx - 11, m_height - m_region.miny - 11);
      glVertex2d(m_region.minx, m_height - m_region.miny - 11);
      glEnd();
   }

   PrintGLError("draw texture");
}

void CRenderGLWidget::Project(int windowX, int windowY, int& bufferX, int &bufferY, bool clamp) const
{
   int imageSize[2];
   imageSize[0] = int (m_width * m_zoomFactor);
   imageSize[1] = int(m_height * m_zoomFactor);

   // need to remove the margin (toolbar, menu, etc..)
   // but we need to consider what would happen if we undock the toolbar !!
   windowY -= 42;

   
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

void CRenderGLWidget::SetRegionCrop(int start_x, int start_y, int end_x, int end_y)
{
   m_region.minx = start_x;
   m_region.miny = start_y;
   m_region.maxx = end_x;
   m_region.maxy = end_y;  
   m_regionCrop = true; 
}

void CRenderGLWidget::ClearRegionCrop()
{
   m_regionCrop = false;
   m_region.minx = m_region.miny = m_region.maxx = m_region.maxy = 0;
}

void CRenderGLWidget::SetZoomFactor(float z)
{
   m_zoomFactor = z;
   // this will dirty the status_changed flag
   // and force a status bar refresh (so that zoom factor appears)
   m_renderview.DirtyStatusBar();
}

void CRenderGLWidget::SetBackgroundImage(const std::string &filename)
{

   QImage bgImg(filename.c_str());


   if (m_bgData)
   {
      delete m_bgData;
      m_bgData = NULL;
   }
   if (bgImg.isNull()) return;
   
   m_bgData = new BackgroundData();

   m_bgData->bgWidth = bgImg.width();
   m_bgData->bgHeight = bgImg.height();

   m_bgData->bgBuffer = (AtRGBA8 *)AiMalloc(m_bgData->bgWidth * m_bgData->bgHeight * sizeof(AtRGBA8));

   const uchar *pixels = bgImg.constBits();
   unsigned int size = m_bgData->bgWidth * m_bgData->bgHeight;
   int i = 0;

   for (unsigned int y = 0; y < m_bgData->bgHeight; ++y)
   {
      for (unsigned int x = 0; x < m_bgData->bgWidth; ++x, ++i)
      {
         AtRGBA8 &bufferPixel = m_bgData->bgBuffer[i];
         QRgb rgba = bgImg.pixel(x, y);

         bufferPixel.r = qRed(rgba);
         bufferPixel.g = qGreen(rgba);
         bufferPixel.b = qBlue(rgba);
         bufferPixel.a = qAlpha(rgba);
      }
   }
   // create texture
   glGenTextures(1, &m_bgData->textureId);
   PrintGLError("create texture");

   glBindTexture(GL_TEXTURE_2D, m_bgData->textureId);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   PrintGLError("set texture parameter");

   // describe texture layout
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_bgData->bgWidth);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
   PrintGLError("set pixel store");

   // set texture data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_bgData->bgWidth, m_bgData->bgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bgData->bgBuffer);
   PrintGLError("set texture pixels");

   glPopClientAttrib();

}

void CRenderGLWidget::SetBackgroundChecker()
{
   if (m_bgData)
   {
      delete m_bgData;
      m_bgData = NULL;
   }
   
   m_bgData = new BackgroundData();

   m_bgData->bgWidth = m_width;
   m_bgData->bgHeight = m_height;

   m_bgData->bgBuffer = (AtRGBA8 *)AiMalloc(m_width * m_height * sizeof(AtRGBA8));

   int pixelIndex = 0;
   int checkerSize = 20;
   int xChecker;
   int yChecker;

   for (int y = 0; y < m_height; ++y)
   {
      for (int x = 0; x < m_width; ++x, ++pixelIndex)
      {
         AtRGBA8 &bufferPixel = m_bgData->bgBuffer[pixelIndex];
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
   glGenTextures(1, &m_bgData->textureId);
   PrintGLError("create texture");

   glBindTexture(GL_TEXTURE_2D, m_bgData->textureId);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   PrintGLError("set texture parameter");

   // describe texture layout
   glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,  m_bgData->bgWidth);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
   PrintGLError("set pixel store");

   // set texture data
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_bgData->bgWidth, m_bgData->bgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bgData->bgBuffer);
   PrintGLError("set texture pixels");

   glPopClientAttrib();

}
