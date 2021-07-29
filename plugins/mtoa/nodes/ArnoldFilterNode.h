#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class CArnoldFilterNode
   :  public MPxNode
{

public:

   MStatus compute(const MPlug& plug, MDataBlock& data) override
   {
      return MS::kSuccess;
   }

   static void* creator();
   static MStatus initialize();

   static MTypeId id;

   // Attributes
   static MObject s_filter;

};  // class CArnoldFilterNode
