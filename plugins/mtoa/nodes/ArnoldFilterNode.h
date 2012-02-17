#ifndef ARNOLD_FILTER_NODE_H
#define ARNOLD_FILTER_NODE_H

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class CArnoldFilterNode
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
   static MObject s_filter;

};  // class CArnoldFilterNode

#endif // ARNOLD_DRIVER_NODE_H
