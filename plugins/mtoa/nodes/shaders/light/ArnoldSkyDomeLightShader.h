#ifndef ARNOLDSKYDOMELIGHTSHADER_H
#define ARNOLDSKYDOMELIGHTSHADER_H

#include "nodes/shaders/background/SphereLocator.h"
#include "utils/AttrHelper.h"

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
   virtual MStatus   compute( const MPlug&, MDataBlock& );

   static MTypeId id;

   static CStaticAttrHelper s_attributes;

   // Inputs
   static MObject s_intensity;
   static MObject s_exposure;
   static MObject s_castShadows;
   static MObject s_shadowDensity;
   static MObject s_shadowColor;
   static MObject s_samples;
   static MObject s_normalize;
   static MObject s_affectDiffuse;
   static MObject s_affectSpecular;
   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;
   // Maya specific intputs
   static MObject s_pointCamera;
   static MObject s_normalCamera;
   // Maya specific Outputs
   static MObject aLightDirection;
   static MObject aLightIntensity;
   static MObject aLightAmbient;
   static MObject aLightDiffuse;
   static MObject aLightSpecular;
   static MObject aLightShadowFraction;
   static MObject aPreShadowIntensity;
   static MObject aLightBlindData;
   static MObject aLightData;
};  // class CArnoldSkyDomeLightShaderNode

#endif // ARNOLDSKYDOMELIGHTSHADER_H
