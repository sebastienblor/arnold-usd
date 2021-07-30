#pragma once

#include "attributes/AttrHelper.h"
#include <maya/MObjectArray.h>

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <extension/AbMayaNode.h>
#include <vector>

class CArnoldOperatorNode
   :  public MPxNode
{

public:

   void postConstructor() override;

   MStatus compute(const MPlug& plug, MDataBlock& data) override
   {
      return MS::kSuccess;
   }

   CAbMayaNode m_abstract;

   static void* creator();
   static MStatus initialize();
   static CAbMayaNode s_abstract;
   // Input attributes
   static MObject SAttr[];
   SchedulingType schedulingType()const override {return kSerial;}
   
   static std::vector<CStaticAttrHelper> s_nodeHelpers;
};  // class CArnoldOperatorNode
