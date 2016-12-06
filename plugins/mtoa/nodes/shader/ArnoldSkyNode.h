#pragma once

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#include <maya/MPlugArray.h>
#include <maya/MDGModifier.h>
#include <maya/MGlobal.h>

class CArnoldSkyNode
   : public CSphereLocator
{

public:

   ~CArnoldSkyNode()
   {
   }

   virtual void postConstructor()
   {
      // Call parent postConstructor as it is not done automatically as the parent constructor
      CSphereLocator::postConstructor();

      setMPSafe(true);
   }

   virtual bool excludeAsLocator() const;
   virtual void draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status);

   static void* creator();
   static MStatus initialize();

   static MTypeId id;

   static CStaticAttrHelper s_attributes;

   // Input attributes
   static MObject s_XX;
   static MObject s_XY;
   static MObject s_XZ;
   static MObject s_X;
   static MObject s_X_angle;
   static MObject s_YX;
   static MObject s_YY;
   static MObject s_YZ;
   static MObject s_Y;
   static MObject s_Y_angle;
   static MObject s_ZX;
   static MObject s_ZY;
   static MObject s_ZZ;
   static MObject s_Z;
   static MObject s_Z_angle;

   // Output attributes
   static MObject s_OUT_colorR;
   static MObject s_OUT_colorG;
   static MObject s_OUT_colorB;
   static MObject s_OUT_color;
   static MObject s_OUT_transparencyR;
   static MObject s_OUT_transparencyG;
   static MObject s_OUT_transparencyB;
   static MObject s_OUT_transparency;
};  // class CArnoldSkyNode
