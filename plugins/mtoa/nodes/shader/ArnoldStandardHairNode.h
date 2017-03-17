#pragma once

#include "extension/ExtensionsManager.h"
#include <maya/MPxHwShaderNode.h>
#include <maya/MObjectArray.h>

#include <extension/AbMayaNode.h>

class CArnoldStandardHairNode
   :  public MPxNode
{

private:

   static MObjectArray s_PlugsAffecting;

public:

   virtual void postConstructor();

   virtual MStatus compute(const MPlug& plug, MDataBlock& data);

   static void* creator();
   static MStatus initialize();

   static MTypeId id;
   
   CAbMayaNode m_abstract;
   static CAbMayaNode s_abstract;
   
   static CStaticAttrHelper* s_nodeHelper;

   // Input attributes
   static MObject s_OUT_color;
   static MObject s_normal_camera;
   
};  // class CArnoldStandardHairNode
