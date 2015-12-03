/*
 * OpenGL display shader and buffer
 */

#pragma once
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QtOpenGL>

#include <ai.h>

#include <string>

#include "display_gl.h"

class CRenderView;

// QGlWidget  displaying the render buffer
enum AtRvColorMode
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

class CRenderGLWidget : public QGLWidget {
  
public:

   struct BackgroundData
   {
      BackgroundData() : bgBuffer(NULL), bgWidth(0), bgHeight(0), textureId(0) {
         scale.x = scale.y = 1.f;
         offset.x = offset.y = 0.f;
      }
      ~BackgroundData() {
         if (bgBuffer) AiFree(bgBuffer);
         if (textureId) glDeleteTextures(1, &textureId);
      }


      AtRGBA8 *bgBuffer;
      unsigned int bgWidth;
      unsigned int bgHeight;
      GLuint textureId;
      AtPoint2 scale;
      AtPoint2 offset;

   };

   CRenderGLWidget(QWidget *parent, CRenderView &rv, int width, int height);
   virtual ~CRenderGLWidget();
   
   void InitSize(int width, int height);



   AtRGBA8 *GetBuffer() { return m_frontBuffer; }
   
   int Width() { return m_width; }
   int Height() { return m_height; }

   void ReloadBuffer(AtRvColorMode colorMode);

   void SetRegionCrop(int startX, int startY, int endX, int endY);
   void ClearRegionCrop();

   void SetPan(int x, int y)
   {
      m_pan[0] = x; 
      m_pan[1] = y; 
   }
   void GetPan(int &x, int &y) const
   {
      x = m_pan[0];
      y = m_pan[1];
   }

   void SetZoomFactor(float z);
   float GetZoomFactor() const {return m_zoomFactor;}

   const AtBBox2 *GetRegion() const {return (m_regionCrop) ? &m_region : NULL;}

   void Project(int windowX, int windowY, int& bufferX, int &bufferY, bool clamp = false) const;

   void DebugGPUState() {m_printGPUState = true;}
   void SetBackgroundImage(const std::string &filename);
   void SetBackgroundChecker();
   void SetBackgroundColor (const AtRGBA &c) {m_bgColor = c;}
   const AtRGBA &GetBackgroundColor() const {return m_bgColor;}

   BackgroundData *GetBackgroundData() {return m_bgData;}

// Qt virtual functions
   virtual void initializeGL();
   virtual void resizeGL(int width, int height);
   virtual void paintGL();
   
private:
   void SetupTexture();
   void DisplayBuffer(int w, int h, const AtBBox2 *updateRegion,
             AtRvColorMode colorMode);

   CRenderView &m_renderview;

   // buffer
   AtRGBA8 *m_frontBuffer;
   
   // OpenGL textured handle
   GLuint m_texture;
   // buffer and texture dimensions
   int m_width, m_height;
   // texture coordinates for drawing
   float m_tx, m_ty;

   bool m_regionCrop;
   AtBBox2 m_region;

   int m_pan[2];
   float m_zoomFactor;
  
   BackgroundData *m_bgData;
   AtRGBA m_bgColor;

   bool m_printGPUState;
  
};

