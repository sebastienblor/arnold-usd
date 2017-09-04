#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class CArnoldAOVNode
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
   static MObject s_name;
   static MObject s_enabled;
   // enumerator that corresponds to arnold's data type enum
   static MObject s_type;
   static MObject s_prefix;
   static MObject s_defaultValue;

   static MObject s_imageFormat;
   static MObject s_filterType;

   static MObject s_outputs;
   static MObject s_driver;
   static MObject s_filter;

   static MObject s_lightPathExpression;
   static MObject s_lightGroups;   
   static MObject s_globalAOV;   
   static MObject s_lightGroupsList;
};  // class CArnoldAOVNode
