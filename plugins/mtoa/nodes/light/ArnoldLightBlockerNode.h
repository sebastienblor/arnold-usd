#ifndef ARNOLDLIGHTBLOCKERNODE_H
#define ARNOLDLIGHTBLOCKERNODE_H

#include "attributes/AttrHelper.h"
#include "nodes/ArnoldNodeIDs.h"

#include <maya/MPxLocatorNode.h>
#include <maya/MTypeId.h>

class CArnoldLightBlockerNode : public MPxLocatorNode{
public:
   
   CArnoldLightBlockerNode();
   ~CArnoldLightBlockerNode();
   
   static void* creator();
   static MStatus initialize();
   
   virtual void draw(M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus);
   virtual bool isBounded() const;
   virtual MBoundingBox boundingBox() const;   
   
   static const MTypeId id;      
private:
   
   static MObject s_geometryType;
   static MObject s_density;
   static MObject s_roundness;
   static MObject s_widthEdge;
   static MObject s_heightEdge;
   static MObject s_ramp;
   static MObject s_rampAxis;
};

#endif // ARNOLDLIGHTBLOCKERNODE_H
