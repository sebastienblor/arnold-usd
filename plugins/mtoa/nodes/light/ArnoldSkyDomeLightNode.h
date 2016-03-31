#pragma once

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#include <ai_types.h>
#include <maya/MSelectionMask.h>

class CArnoldSkyDomeLightNode
   :  public CSphereLocator
{

public:
	virtual	~CArnoldSkyDomeLightNode();
   virtual void postConstructor();

   static void* creator();
   static MStatus initialize();
   virtual MStatus   compute( const MPlug&, MDataBlock& );
   virtual void draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status);
#ifdef ENABLE_VP2
   virtual MSelectionMask getShapeSelectionMask() const;
#endif
   static MTypeId id;

   static CStaticAttrHelper s_attributes;

   // Inputs
   static MObject s_intensity;
//   static MObject s_exposure;
//   static MObject s_castShadows;
//   static MObject s_samples;
//   static MObject s_normalize;
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

   // Color management attributes
   static MObject s_colorMgtEnabled;         // Is Color Mgt enabled or not ?
   static MObject s_colorMgtCfgFileEnabled;  // Is the OCIO Mode enabled or not ?
   static MObject s_colorMgtCfgFilePath;     // The OCIO config to use when OCIO mode is enabled
   static MObject s_workingSpace;            // The rendering space to use
   static MObject s_colorSpace;              // The input color space to use
   static MObject s_ignoreColorSpaceRules;
};  // class CArnoldSkyDomeLightNode
