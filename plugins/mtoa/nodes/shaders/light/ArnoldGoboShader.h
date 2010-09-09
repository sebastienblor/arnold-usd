#ifndef ARNOLD_GOBO_SHADER_H
#define ARNOLD_GOBO_SHADER_H

#include <maya/MPxNode.h>

class CArnoldGoboShaderNode
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
   static MObject s_filter_mode;
   static MObject s_offsetX;
   static MObject s_offsetY;
   static MObject s_offset;
   static MObject s_rotate;
   static MObject s_scale_s;
   static MObject s_scale_t;
   static MObject s_slidemapR;
   static MObject s_slidemapG;
   static MObject s_slidemapB;
   static MObject s_slidemap;
   static MObject s_wrap_s;
   static MObject s_wrap_t;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldGoboShaderNode

#endif // ARNOLD_GOBO_SHADER_H
