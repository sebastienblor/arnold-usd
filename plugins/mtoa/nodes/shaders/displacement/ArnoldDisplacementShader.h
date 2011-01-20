#ifndef ARNOLDDISPLACEMENTSHADER_H
#define ARNOLDDISPLACEMENTSHADER_H

#include <maya/MPxNode.h>

class CArnoldDisplacementShaderNode
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
   static MObject s_height;
   static MObject s_zero_value;
   static MObject s_mapR;
   static MObject s_mapG;
   static MObject s_mapB;
   static MObject s_map;
   static MObject s_autobump;
   static MObject s_vector_displacement;
   static MObject s_vector_displacement_scale;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;
   static MObject s_OUT_displacementR;
   static MObject s_OUT_displacementG;
   static MObject s_OUT_displacementB;
   static MObject s_OUT_displacement;

};  // class CArnoldDisplacementShaderNode

#endif // ARNOLDDISPLACEMENTSHADER_H
