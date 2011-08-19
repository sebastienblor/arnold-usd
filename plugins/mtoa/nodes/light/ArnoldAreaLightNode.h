#ifndef ARNOLDAREALIGHTNODE_H
#define ARNOLDAREALIGHTNODE_H

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
   #include <OpenGL/glu.h>
#else
   #include <GL/gl.h>
   #include <GL/glu.h>
#endif

#include <ai_types.h>
#include "attributes/AttrHelper.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MPxLocatorNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MFnPlugin.h>
#include <maya/MDistance.h>
#include <maya/MFnEnumAttribute.h>

class CArnoldAreaLightNode : public MPxLocatorNode
{
public:
   CArnoldAreaLightNode();
   virtual ~CArnoldAreaLightNode();

   virtual MStatus        compute( const MPlug& plug, MDataBlock& data );
   virtual void            draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus );
   virtual bool            isBounded() const;
   virtual MBoundingBox    boundingBox() const;
   static  void *          creator();
   static  MStatus         initialize();

   static CStaticAttrHelper s_attributes;

   // The type of the areaLight
   static  MObject s_type;
   // Input attributes
   static  MObject s_colorR;
   static  MObject s_colorG;
   static  MObject s_colorB;
   static  MObject s_color;
   static  MObject s_intensity;
   static  MObject s_shadowDensity;
   static  MObject s_shadowColor;
   static  MObject s_affectDiffuse;
   static  MObject s_affectSpecular;

   // Arnold outputs
   static  MObject s_OUT_colorR;
   static  MObject s_OUT_colorG;
   static  MObject s_OUT_colorB;
   static  MObject s_OUT_color;
   static  MObject s_OUT_transparencyR;
   static  MObject s_OUT_transparencyG;
   static  MObject s_OUT_transparencyB;
   static  MObject s_OUT_transparency;
   // Maya specific intputs
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

public:
   static   MTypeId     id;
};  // class CArnoldAreaLightNode

#endif // ARNOLDAREALIGHTNODE_H
