
#include "ArnoldRenderOptions.h"

#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldRenderOptionsNode::id(0x00071111);

MObject CArnoldRenderOptionsNode::s_Threads;

void* CArnoldRenderOptionsNode::creator()
{
   return new CArnoldRenderOptionsNode();
}

MStatus CArnoldRenderOptionsNode::initialize()
{
   MFnNumericAttribute  nAttr;

   s_Threads = nAttr.create("threads", "thrd", MFnNumericData::kInt, 1);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(16);
   nAttr.setMin(1);
   nAttr.setMax(16);
   addAttribute(s_Threads);

   return MS::kSuccess;
}
