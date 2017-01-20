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
#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
   virtual MStatus setDependentsDirty( const MPlug& plug, MPlugArray& plugArray);
#endif
#endif

   virtual bool isBounded() const;
   virtual MBoundingBox boundingBox() const;

   static void* creator();
   static MStatus initialize();
   MBoundingBox* geometry();

#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
   virtual MSelectionMask getShapeSelectionMask() const;
#endif
#endif

   static MTypeId id;
private:

   // Attributes
   static CStaticAttrHelper s_attributes;
   static MObject s_loadAtInit;
   
   static MObject s_stepSize;
   static MObject s_boundingBoxMin;
   static MObject s_boundingBoxMax;
   static MObject s_autoStepSize;
   static MObject s_stepScale;
   
   static MObject s_filename;
   static MObject s_filedata;
   static MObject s_disable_ray_extents;
   static MObject s_bounds_slack;
   
   static MObject s_step_scale;
   static MObject s_step_size;
   static MObject s_compress;
   
   static MObject s_min;
   static MObject s_max;
   
   static MObject s_grids;
   static MObject s_frame;
   static MObject s_padding;
   
   static MObject s_velocity_grids;
   static MObject s_velocity_scale;
   static MObject s_velocity_fps;
   static MObject s_velocity_shutter_start;
   static MObject s_velocity_shutter_end;
   static MObject s_velocity_threshold;
   

   MString m_filename;
   MString m_grids;
   float m_padding;
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
