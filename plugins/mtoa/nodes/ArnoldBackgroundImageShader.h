#ifndef ARNOLD_BACKGROUND_IMAGE_SHADER_H
#define ARNOLD_BACKGROUND_IMAGE_SHADER_H

#include <maya/MPxNode.h>

class CArnoldBackgroundImageShaderNode
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
   static MObject s_filename;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldBackgroundImageShaderNode

#endif // ARNOLD_BACKGROUND_IMAGE_SHADER_H
