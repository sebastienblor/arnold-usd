#ifndef ARNOLD_WIREFRAME_SHADER_H
#define ARNOLD_WIREFRAME_SHADER_H

#include <maya/MPxNode.h>

class CArnoldWireframeShaderNode
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
   static MObject s_edge_type;
   static MObject s_fill_colorR;
   static MObject s_fill_colorG;
   static MObject s_fill_colorB;
   static MObject s_fill_color;
   static MObject s_line_colorR;
   static MObject s_line_colorG;
   static MObject s_line_colorB;
   static MObject s_line_color;
   static MObject s_line_width;
   static MObject s_raster_space;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldWireframeShaderNode

#endif // ARNOLD_WIREFRAME_SHADER_H
