#pragma once

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
   :  public MPxSurfaceShape
{

public:

   virtual void postConstructor();

   virtual MStatus compute(const MPlug& plug, MDataBlock& data);
   virtual bool isBounded() const {return true;}
   virtual MBoundingBox boundingBox() const;
   virtual MSelectionMask getShapeSelectionMask() const;

   CAbMayaNode m_abstract;

   static void* creator();
   static MStatus initialize();

   static CAbMayaNode s_abstract;

   // Input attributes
   static MObject SAttr[];

   static std::vector<CStaticAttrHelper> s_nodeHelpers;

};  // class CArnoldShaderNode





// UI class - defines the UI part of a shape node
class CArnoldProceduralNodeUI: public MPxSurfaceShapeUI
{
public:
   CArnoldProceduralNodeUI();
   virtual ~CArnoldProceduralNodeUI();
   virtual void getDrawRequests(const MDrawInfo & info,
         bool objectAndActiveOnly, MDrawRequestQueue & requests);
   virtual void draw(const MDrawRequest & request, M3dView & view) const;
   virtual bool select(MSelectInfo &selectInfo, MSelectionList &selectionList,
         MPointArray &worldSpaceSelectPts) const;

   void getDrawRequestsWireFrame(MDrawRequest&, const MDrawInfo&);

   static void * creator();
   // Draw Tokens
   //
   enum
   {
      kDrawBoundingBox, kLastToken
   };

}; // class CArnoldProceduralNodeUI
