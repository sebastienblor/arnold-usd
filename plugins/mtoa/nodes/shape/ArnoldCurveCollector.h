#pragma once

#include "nodes/SphereLocator.h"
#include "attributes/AttrHelper.h"

#include <maya/MSelectionMask.h>

class CArnoldCurveCollector
   :  public MPxLocatorNode
{

public:
   virtual void postConstructor();

   static void* creator();
   static MStatus initialize();
   virtual MStatus   compute( const MPlug&, MDataBlock& );
   
   static MTypeId id;

   static CStaticAttrHelper s_attributes;
   static MObject s_width;
   static MObject s_sampleRate;
   static MObject s_curveShader;
   static MObject s_curveShaderR;
   static MObject s_curveShaderG;
   static MObject s_curveShaderB;
   static MObject s_exportRefPoints;
   static MObject s_minPixelWidth;
   static MObject s_mode;

}; 
