#pragma once

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#include <ai_types.h>
#include <maya/MSelectionMask.h>

class CArnoldLightPortalNode
   :  public MPxLocatorNode
{

public:
   CArnoldLightPortalNode();
   virtual void postConstructor();

   static CStaticAttrHelper s_attributes;

   static void* creator();
   static MStatus initialize();
   virtual MStatus   compute( const MPlug&, MDataBlock& );
   virtual void            draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus displayStatus );
   virtual bool            isBounded() const;
   virtual MBoundingBox    boundingBox() const;
   virtual bool            excludeAsLocator() const;

   static MTypeId id;
   MBoundingBox         m_boundingBox;
//   static MObject s_update;
}; 
