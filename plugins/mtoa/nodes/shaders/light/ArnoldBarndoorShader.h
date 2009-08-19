#ifndef ARNOLD_BARNDOOR_SHADER_H
#define ARNOLD_BARNDOOR_SHADER_H

#include <maya/MPxNode.h>

class CArnoldBarndoorShaderNode
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
   static MObject s_barndoor_bottom_edge;
   static MObject s_barndoor_bottom_left;
   static MObject s_barndoor_bottom_right;
   static MObject s_barndoor_top_edge;
   static MObject s_barndoor_top_left;
   static MObject s_barndoor_top_right;
   static MObject s_barndoor_left_edge;
   static MObject s_barndoor_left_bottom;
   static MObject s_barndoor_left_top;
   static MObject s_barndoor_right_edge;
   static MObject s_barndoor_right_bottom;
   static MObject s_barndoor_right_top;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldBarndoorShaderNode

#endif // ARNOLD_BARNDOOR_SHADER_H
