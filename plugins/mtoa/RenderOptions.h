#ifndef RENDER_OPTIONS_H
#define RENDER_OPTIONS_H

#include <ai_types.h>

#include <maya/MString.h>

class CMayaScene;

class CRenderOptions
{
public:

   CRenderOptions();

   void GetRenderOptions(CMayaScene* scene);
   
   void SetWidth(AtUInt width)
   {
      m_width = width;

      SetupImageOptions();
   }

   void SetHeight(AtUInt height)
   {
      m_height = height;

      SetupImageOptions();
   }

   AtUInt32 minX() const
   {
      return m_minx;
   }

   AtUInt32 minY() const
   {
      return m_miny;
   }

   AtUInt32 maxX() const
   {
      return m_maxx;
   }

   AtUInt32 maxY() const
   {
      return m_maxy;
   }

   AtUInt32 width() const
   {
      return m_width;
   }

   AtUInt32 height() const
   {
      return m_height;
   }

   float pixelAspectRatio() const
   {
      return m_pixelAspectRatio;
   }

   float outputGamma() const
   {
      return m_gamma;
   }

   bool useRenderRegion() const
   {
      return m_useRenderRegion;
   }

   bool clearBeforeRender() const
   {
      return m_clearBeforeRender;
   }

   MString outputAssFile() const
   {
      return m_outputAssFile;
   }

   bool outputAssCompressed() const
   {
      return m_outputAssCompressed;
   }

   AtUInt outputAssMask() const
   {
      return m_outputAssMask;
   }

private:

   void ProcessCommonRenderOptions();
   void ProcessArnoldRenderOptions();
   
   void SetupImageOptions();

private:

   AtUInt32 m_minx, m_miny, m_maxx, m_maxy;
   AtUInt32 m_width, m_height;
   float    m_pixelAspectRatio;
   bool     m_useRenderRegion;
   bool     m_clearBeforeRender; 
   float    m_gamma;

   MString m_outputAssFile;
   bool m_outputAssCompressed;
   AtUInt m_outputAssMask;

   CMayaScene* m_scene;
};

#endif // RENDER_OPTIONS_H
