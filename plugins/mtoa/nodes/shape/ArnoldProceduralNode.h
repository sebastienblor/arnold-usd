#pragma once

#include "ArnoldBaseProcedural.h"
#include "attributes/AttrHelper.h"
#include <maya/MPxHwShaderNode.h>
#include <maya/MObjectArray.h>

#include <extension/AbMayaNode.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>

#include <vector>

/**
 *    This class is meant for procedural nodes created automatically by arnold procedural dlls
 **/

class CArnoldProceduralNode
   :  public CArnoldBaseProcedural
{

public:
   CArnoldProceduralNode();
   virtual ~CArnoldProceduralNode() {}

   void postConstructor() override;

   CAbMayaNode m_abstract;

   static void* creator();
   static MStatus initialize();
   void updateGeometry() override;
   SchedulingType schedulingType()const override {return kSerial;}
   
   static CAbMayaNode s_abstract;

   

   // Input attributes
   static MObject SAttr[];

   static std::vector<CStaticAttrHelper> s_nodeHelpers;


};  // class CArnoldShaderNode


