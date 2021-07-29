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

   void postConstructor() override;

   MStatus compute(const MPlug& plug, MDataBlock& data) override;
   SchedulingType schedulingType()const override {return kSerial;}

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
