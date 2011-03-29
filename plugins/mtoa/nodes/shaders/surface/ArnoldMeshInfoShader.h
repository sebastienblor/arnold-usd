#ifndef ARNOLDMESHINFOSHADER_H
#define ARNOLDMESHINFOSHADER_H

#include <maya/MPxNode.h>

class CArnoldMeshInfoShaderNode
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
   static MObject s_what;
   static MObject s_colorSetName;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;

};  // class CArnoldUtility2ShaderNode

#endif // ARNOLDMESHINFOSHADER_H
