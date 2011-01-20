#ifndef ARNOLDLIGHTDECAYSHADER_H
#define ARNOLDLIGHTDECAYSHADER_H

#include <maya/MPxNode.h>

class CArnoldLightDecayShaderNode
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
   static MObject s_decay_clamp;
   static MObject s_decay_radius;
   static MObject s_decay_type;
   static MObject s_far_end;
   static MObject s_far_start;
   static MObject s_near_end;
   static MObject s_near_start;
   static MObject s_use_far_atten;
   static MObject s_use_near_atten;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldLightDecayShaderNode

#endif // ARNOLDLIGHTDECAYSHADER_H
