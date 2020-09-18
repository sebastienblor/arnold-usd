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

   virtual void postConstructor();

   virtual MStatus setDependentsDirty( const MPlug& plug, MPlugArray& plugArray);

   CAbMayaNode m_abstract;

   static void* creator();
   static MStatus initialize();
   virtual void updateGeometry();
   
   static CAbMayaNode s_abstract;

   

   // Input attributes
   static MObject SAttr[];

   static std::vector<CStaticAttrHelper> s_nodeHelpers;


};  // class CArnoldShaderNode


