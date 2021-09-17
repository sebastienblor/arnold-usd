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

   void postConstructor() override;
   MStatus compute(const MPlug& plug, MDataBlock& data) override;
   MStatus setDependentsDirty( const MPlug& plug, MPlugArray& plugArray) override;

   bool isBounded() const override;
   MBoundingBox boundingBox() const override;

   static void* creator();
   static MStatus initialize();
   MBoundingBox* geometry();

   MSelectionMask getShapeSelectionMask() const override;

   static MTypeId id;
private:

   // Attributes
   static CStaticAttrHelper s_attributes;
//   static MObject s_loadAtInit;
   
   static MObject s_stepSize;
   static MObject s_boundingBoxMin;
   static MObject s_boundingBoxMax;
   static MObject s_autoStepSize;
   static MObject s_stepScale;
   
   static MObject s_filename;
   static MObject s_filedata;
   static MObject s_disable_ray_extents;
   static MObject s_volume_padding;
   
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
   static MObject s_motion_start;
   static MObject s_motion_end;
   static MObject s_velocity_threshold;
   
   static MObject s_type;
   static MObject s_threshold;
   static MObject s_samples;
   static MObject s_solver;
   static MObject s_field_channel;
   static MObject s_field;

   static MObject s_useFrameExtension;

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
   void getDrawRequests(const MDrawInfo & info,
         bool objectAndActiveOnly, MDrawRequestQueue & requests) override;
   void draw(const MDrawRequest & request, M3dView & view) const override;
   void getDrawRequestsWireFrame(MDrawRequest&, const MDrawInfo&);
    
   bool select(MSelectInfo &selectInfo, MSelectionList &selectionList,
         MPointArray &worldSpaceSelectPts) const override;

   static void * creator();
   
   enum
   {
      kDrawBoundingBox, kLastToken
   };

}; // class CArnoldVolumeShapeUI
