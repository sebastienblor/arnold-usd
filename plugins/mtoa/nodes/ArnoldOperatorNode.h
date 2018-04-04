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

   virtual void postConstructor();

   virtual MStatus compute(const MPlug& plug, MDataBlock& data)
   {
      return MS::kSuccess;
   }

   CAbMayaNode m_abstract;

   static void* creator();
   static MStatus initialize();
   static CAbMayaNode s_abstract;
   // Input attributes
   static MObject SAttr[];

   static std::vector<CStaticAttrHelper> s_nodeHelpers;
};  // class CArnoldOperatorNode
