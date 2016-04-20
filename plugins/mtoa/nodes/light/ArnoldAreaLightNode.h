#pragma once

#include "utils/DrawUtils.h" // so glew is going to be included before gl.h

#include "attributes/AttrHelper.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MStatus.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MBoundingBox.h>
#include <maya/MObject.h>
#include <maya/MTypeId.h>
#include <maya/M3dView.h>
#include <maya/MPlug.h>
#include <maya/MDagPath.h>
#include <maya/MDataBlock.h>

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else 
   #include <GL/gl.h>
#endif   

class CArnoldAreaLightNode : public MPxLocatorNode
{
public:
   CArnoldAreaLightNode();
   virtual ~CArnoldAreaLightNode();

   virtual MStatus         compute(const MPlug& plug, MDataBlock& data);
   virtual void            draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus );
   virtual bool            isBounded() const;
   virtual MBoundingBox    boundingBox() const;
   static  void *          creator();
   static  MStatus         initialize();
   virtual bool            excludeAsLocator() const;

   static CStaticAttrHelper s_attributes;

   // The type of the areaLight
   static  MObject s_type;
   // Input attributes
   static  MObject s_colorR;
   static  MObject s_colorG;
   static  MObject s_colorB;
   static  MObject s_color;
   static  MObject s_intensity;
   static  MObject s_affectDiffuse;
   static  MObject s_affectSpecular;
   static  MObject s_update;

   // Arnold outputs
   static  MObject s_OUT_colorR;
   static  MObject s_OUT_colorG;
   static  MObject s_OUT_colorB;
   static  MObject s_OUT_color;
   static  MObject s_OUT_transparencyR;
   static  MObject s_OUT_transparencyG;
   static  MObject s_OUT_transparencyB;
   static  MObject s_OUT_transparency;
   // Maya specific inputs
   static  MObject s_pointCamera;
   static  MObject s_normalCamera;
   // Maya specific Outputs
   static  MObject aLightDirection;
   static  MObject aLightIntensity;
   static  MObject aLightAmbient;
   static  MObject aLightDiffuse;
   static  MObject aLightSpecular;
   static  MObject aLightShadowFraction;
   static  MObject aPreShadowIntensity;
   static  MObject aLightBlindData;
   static  MObject aLightData;
   // Maya shadowing inputs
   static  MObject aDropOff;
   static  MObject aDecayRate;
   static  MObject aUseRayTraceShadows;
   static  MObject aDepthMapResolution;

public:
   MBoundingBox         m_boundingBox;
   static   MTypeId     id;
};  // class CArnoldAreaLightNode
