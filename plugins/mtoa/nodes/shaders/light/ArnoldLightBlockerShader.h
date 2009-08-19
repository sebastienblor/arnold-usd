#ifndef ARNOLD_LIGHT_BLOCKER_SHADER_H
#define ARNOLD_LIGHT_BLOCKER_SHADER_H

#include <maya/MPxNode.h>

class CArnoldLightBlockerShaderNode
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
   static MObject s_density;
   static MObject s_geometry_matrix;
   static MObject s_geometry_type;
   static MObject s_height_edge;
   static MObject s_ramp;
   static MObject s_ramp_axis;
   static MObject s_roundness;
   static MObject s_width_edge;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldLightBlockerShaderNode

#endif // ARNOLD_LIGHT_BLOCKER_SHADER_H
