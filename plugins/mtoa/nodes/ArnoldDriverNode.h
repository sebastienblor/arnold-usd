#ifndef ARNOLD_DRIVER_NODE_H
#define ARNOLD_DRIVER_NODE_H

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class CArnoldDriverNode
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
   static MObject s_mergeAOVs;
   static MObject s_driver;
   static MObject s_prefix;
   static MObject s_outputMode;

};  // class CArnoldDriverNode

#endif // ARNOLD_DRIVER_NODE_H
