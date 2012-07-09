#include "ArnoldLightBlockerNode.h"

const MTypeId CArnoldLightBlockerNode::id(ARNOLD_NODEID_LIGHT_BLOCKER);

CArnoldLightBlockerNode::CArnoldLightBlockerNode()
{
   
}

CArnoldLightBlockerNode::~CArnoldLightBlockerNode()
{
   
}

void* CArnoldLightBlockerNode::creator()
{
   return new CArnoldLightBlockerNode();
}

MStatus CArnoldLightBlockerNode::initialize()
{
   return MS::kSuccess;
}

bool CArnoldLightBlockerNode::isBounded() const
{
   return true;
}

MBoundingBox CArnoldLightBlockerNode::boundingBox() const
{
   return MBoundingBox(MPoint(0.5, 0.5, 0.5), MPoint(-0.5, -0.5, -0.5));
}

void CArnoldLightBlockerNode::draw(M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus)
{
   
}
