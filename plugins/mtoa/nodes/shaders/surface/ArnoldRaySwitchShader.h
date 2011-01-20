#ifndef ARNOLDRAYSWITCHSHADER_H
#define ARNOLDRAYSWITCHSHADER_H

#include <maya/MPxNode.h>

class CArnoldRaySwitchShaderNode
   :  public MPxNode
{

public:

   virtual void postConstructor()
   {
      setMPSafe(true);
   }

   virtual MStatus compute(const MPlug& plug, MDataBlock& data);

   static void* creator();
   static MStatus initialize();

	static MTypeId id;

   // Input attribute
   static MObject s_IN_camera;
   static MObject s_IN_cameraR;
   static MObject s_IN_cameraG;
   static MObject s_IN_cameraB;
   static MObject s_IN_shadow;
   static MObject s_IN_shadowR;
   static MObject s_IN_shadowG;
   static MObject s_IN_shadowB;
   static MObject s_IN_reflection;
   static MObject s_IN_reflectionR;
   static MObject s_IN_reflectionG;
   static MObject s_IN_reflectionB;
   static MObject s_IN_refraction;
   static MObject s_IN_refractionR;
   static MObject s_IN_refractionG;
   static MObject s_IN_refractionB;
   static MObject s_IN_diffuse;
   static MObject s_IN_diffuseR;
   static MObject s_IN_diffuseG;
   static MObject s_IN_diffuseB;
   static MObject s_IN_glossy;
   static MObject s_IN_glossyR;
   static MObject s_IN_glossyG;
   static MObject s_IN_glossyB;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldRaySwicthShaderNode

#endif // ARNOLDRAYSWITCHSHADER_H
