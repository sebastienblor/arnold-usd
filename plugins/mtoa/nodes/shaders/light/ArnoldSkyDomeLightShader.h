#ifndef ARNOLDSKYDOMELIGHTSHADER_H
#define ARNOLDSKYDOMELIGHTSHADER_H

#include "nodes/shaders/background/SphereLocator.h"
#include "nodes/ArnoldNodeHelper.h"

#include <ai_types.h>


class CArnoldSkyDomeLightShaderNode
   :  public CSphereLocator
{

public:
   virtual void postConstructor()
   {
      // Initialize colorData
      m_colorData   = NULL;
      m_UData       = NULL;
      m_VData       = NULL;
      m_goSample    = true;
      m_goUVSample  = true;

      setMPSafe(true);
   }

   static void* creator();
   static MStatus initialize();

   static MTypeId id;

   static CStaticAttrHelper s_attributes;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;
};  // class CArnoldSkyDomeLightShaderNode

#endif // ARNOLDSKYDOMELIGHTSHADER_H
