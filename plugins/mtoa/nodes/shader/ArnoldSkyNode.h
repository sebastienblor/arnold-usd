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

   bool excludeAsLocator() const override;
   void draw(M3dView& view, const MDagPath& DGpath, M3dView::DisplayStyle style, M3dView::DisplayStatus status) override;

   static void* creator();
   static MStatus initialize();

   void postConstructor() override;
   SchedulingType schedulingType()const override {return kParallel;}

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
