#ifndef ARNOLD_FOG_SHADER_H
#define ARNOLD_FOG_SHADER_H

#include <maya/MPxNode.h>

class CArnoldFogShaderNode
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
   static MObject s_distance;
   static MObject s_ground_normalX;
   static MObject s_ground_normalY;
   static MObject s_ground_normalZ;
   static MObject s_ground_normal;
   static MObject s_ground_pointX;
   static MObject s_ground_pointY;
   static MObject s_ground_pointZ;
   static MObject s_ground_point;
   static MObject s_height;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldFogShaderNode

#endif // ARNOLD_FOG_SHADER_H
