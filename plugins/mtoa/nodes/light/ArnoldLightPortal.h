#pragma once

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#include <maya/MSelectionMask.h>

class CArnoldLightPortalNode
   :  public MPxLocatorNode
{

public:
   CArnoldLightPortalNode();
   void postConstructor() override;

   static CStaticAttrHelper s_attributes;

   static void* creator();
   static MStatus initialize();
   MStatus   compute( const MPlug&, MDataBlock& ) override;
   void            draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus ) override;
   bool            isBounded() const override;
   MBoundingBox    boundingBox() const override;
   bool            excludeAsLocator() const override;
   SchedulingType schedulingType()const override {return kParallel;}

   static MTypeId id;
   MBoundingBox         m_boundingBox;
//   static MObject s_update;
}; 
