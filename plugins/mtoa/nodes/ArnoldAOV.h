#ifndef ARNOLDAOV_H
#define ARNOLDAOV_H

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
   static MObject s_aovs;
   static MObject s_name;
   static MObject s_type;
   static MObject s_prefix;
   static MObject s_batch_mode_only;
   
   // other attributes: filter etc

};  // class CArnoldAOVNode

#endif // ARNOLDAOV_H
