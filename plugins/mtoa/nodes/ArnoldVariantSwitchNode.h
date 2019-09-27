#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class CArnoldVariantSwitchNode
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
   static MObject s_enable;
   // enumerator that corresponds to arnold's data type enum
   static MObject s_index;

   static MObject s_variants;
   static MObject s_name;
   static MObject s_inputs;

   static MObject s_out;

};  // class CArnoldVariantSwitchNode
