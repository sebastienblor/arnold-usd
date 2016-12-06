#pragma once

#include "utils/DrawUtils.h"

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#if defined(_DARWIN)
   #include <OpenGL/gl.h>
#else
   #include <GL/gl.h>
#endif


class CArnoldPhotometricLightNode
   :  public MPxLocatorNode
{

public:
   CArnoldPhotometricLightNode();
   virtual ~CArnoldPhotometricLightNode();

#if MAYA_API_VERSION >= 201700
   virtual void            postConstructor();
   static void             attrChangedCallBack(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData);
#endif
   virtual MStatus         compute(const MPlug& plug, MDataBlock& data);
   virtual void            draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus );
   virtual bool            isBounded() const;
   virtual MBoundingBox    boundingBox() const;
   static  void *          creator();
   static  MStatus         initialize();
   virtual bool            excludeAsLocator() const;

   static MTypeId       id;
   MBoundingBox         m_boundingBox;

   static CStaticAttrHelper s_attributes;

   // Input attributes
   static  MObject s_colorR;
   static  MObject s_colorG;
   static  MObject s_colorB;
   static  MObject s_color;
   static  MObject s_intensity;
   static  MObject s_affectDiffuse;
   static  MObject s_affectSpecular;

   static  MObject s_filename;

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
#if MAYA_API_VERSION >= 201700
   // Maya spot light inputs
   static  MObject aConeAngle;
   static  MObject aPenumbraAngle;
   static  MObject aDropOff;
   static  MObject aDecayRate;
   static  MObject aUseRayTraceShadows;
   static  MObject aDepthMapResolution;
   static  MObject aShadowColor;

private:
   MCallbackId m_attrChangeId;
   bool  m_aiCastShadows;
   bool  m_aiCastVolumetricShadows;
#endif
};  // class CArnoldPhotometricLightNode
