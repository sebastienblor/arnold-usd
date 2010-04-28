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
   static MObject s_XX;
   static MObject s_XY;
   static MObject s_XZ;
   static MObject s_X;
   static MObject s_X_angle;
   static MObject s_YX;
   static MObject s_YY;
   static MObject s_YZ;
   static MObject s_Y;
   static MObject s_Y_angle;
   static MObject s_ZX;
   static MObject s_ZY;
   static MObject s_ZZ;
   static MObject s_Z;
   static MObject s_Z_angle;
   static MObject s_colorR;
   static MObject s_colorG;
   static MObject s_colorB;
   static MObject s_color;
   static MObject s_intensity;
   static MObject s_format;
   static MObject s_opaque_alpha;
   static MObject s_visibility;

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
