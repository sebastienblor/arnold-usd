#ifndef ARNOLD_SKY_SHADER_H
#define ARNOLD_SKY_SHADER_H

#include <maya/MPxNode.h>

class CArnoldSkyShaderNode
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
   static MObject s_colorR;
   static MObject s_colorG;
   static MObject s_colorB;
   static MObject s_color;
   static MObject s_emission_colorR;
   static MObject s_emission_colorG;
   static MObject s_emission_colorB;
   static MObject s_emission_color;
   static MObject s_emission_intensity;
   static MObject s_intensity;
   static MObject s_opaque_alpha;
   static MObject s_visible;

   static MObject s_separate_colors;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldSkyShaderNode

#endif // ARNOLD_SKY_SHADER_H
