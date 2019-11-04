#pragma once

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#include <maya/MSelectionMask.h>

class CArnoldSkyDomeLightNode
   :  public CSphereLocator
{

public:
   virtual void postConstructor();

   static void* creator();
   static MStatus initialize();
   virtual MStatus   compute( const MPlug&, MDataBlock& );
   virtual void draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status);
   virtual MSelectionMask getShapeSelectionMask() const;
   static MTypeId id;

   static CStaticAttrHelper s_attributes;

   // Inputs
   static MObject s_intensity;
//   static MObject s_exposure;
//   static MObject s_castShadows;
//   static MObject s_samples;
//   static MObject s_normalize;
   static MObject s_portalMode;
   static MObject s_visibleCamera;
   static MObject s_visibleTransmission;
   
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
   
};  // class CArnoldSkyDomeLightNode
