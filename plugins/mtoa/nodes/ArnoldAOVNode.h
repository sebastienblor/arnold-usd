#ifndef ARNOLDAOVNODE_H
#define ARNOLDAOVNODE_H

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
};  // class CArnoldAOVNode

#endif // ARNOLDAOVNODE_H
