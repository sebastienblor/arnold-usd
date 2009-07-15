#ifndef ARNOLD_RENDER_OPTIONS_H
#define ARNOLD_RENDER_OPTIONS_H

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class CArnoldRenderOptionsNode
   :  public MPxNode
{

public:

   virtual MStatus compute(const MPlug& plug, MDataBlock& data)
   {
      return MS::kSuccess;
   }

   static void* creator();
   static MStatus initialize();

	static MTypeId id;

   // Attributes

   static MObject s_Threads;

};  // class CArnoldRenderOptionsNode

#endif // ARNOLD_RENDER_OPTIONS_H
