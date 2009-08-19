#ifndef ARNOLD_AMBIENT_OCCLUSION_SHADER_H
#define ARNOLD_AMBIENT_OCCLUSION_SHADER_H

#include <maya/MPxNode.h>

class CArnoldAmbientOcclusionShaderNode
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

   // Input attributes
   static MObject s_blackR;
   static MObject s_blackG;
   static MObject s_blackB;
   static MObject s_black;
   static MObject s_falloff;
   static MObject s_far_clip;
   static MObject s_near_clip;
   static MObject s_opacityR;
   static MObject s_opacityG;
   static MObject s_opacityB;
   static MObject s_opacity;
   static MObject s_samples;
   static MObject s_spread;
   static MObject s_whiteR;
   static MObject s_whiteG;
   static MObject s_whiteB;
   static MObject s_white;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldAmbientOcclusionShaderNode

#endif // ARNOLD_AMBIENT_OCCLUSION_SHADER_H
