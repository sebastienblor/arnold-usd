#pragma once

#include "attributes/AttrHelper.h"

#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>

#include <ai_nodes.h>

#include <vector>
#include <map>

// Shape class - defines the non-UI part of a shape node
class CArnoldVolumeShape: public MPxSurfaceShape
{

public:
   CArnoldVolumeShape();
   virtual ~CArnoldVolumeShape();

   virtual void postConstructor();
   virtual MStatus compute(const MPlug& plug, MDataBlock& data);

   virtual bool isBounded() const;
   virtual MBoundingBox boundingBox() const;

   static void* creator();
   static MStatus initialize();
   MBoundingBox* geometry();

   static MTypeId id;
private:

   // Attributes
   static CStaticAttrHelper s_attributes;
   static MObject s_type;
   static MObject s_dso;
   static MObject s_data;
   static MObject s_loadAtInit;
   static MObject s_stepSize;
   static MObject s_boundingBoxMin;
   static MObject s_boundingBoxMax;
   
   static MObject s_filename;
   static MObject s_grids;
   static MObject s_frame;
   
   int m_type;
   MString m_dso;
   MString m_data;
   MString m_filename;
   MString m_grids;
   int m_frame;
   
   MBoundingBox m_bbox;

}; // class CArnoldVolumeShape


// UI class - defines the UI part of a shape node
class CArnoldVolumeShapeUI: public MPxSurfaceShapeUI
{
public:
   CArnoldVolumeShapeUI();
   virtual ~CArnoldVolumeShapeUI();
   virtual void getDrawRequests(const MDrawInfo & info,
         bool objectAndActiveOnly, MDrawRequestQueue & requests);
   virtual void draw(const MDrawRequest & request, M3dView & view) const;
   void getDrawRequestsWireFrame(MDrawRequest&, const MDrawInfo&);
   
   virtual bool select(MSelectInfo &selectInfo, MSelectionList &selectionList,
         MPointArray &worldSpaceSelectPts) const;

   static void * creator();
   
   enum
   {
      kDrawBoundingBox, kLastToken
   };

}; // class CArnoldVolumeShapeUI
