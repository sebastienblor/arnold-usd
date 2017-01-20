#include "ArnoldVolume.h"
#include "nodes/ArnoldNodeIDs.h"
#include "translators/DagTranslator.h"
#include "utils/Universe.h"
#include "scene/MayaScene.h"
#include "utils/MayaUtils.h"

#include <ai_render.h>
#include <ai_dotass.h>
#include <ai_universe.h>
#include <ai_node_entry.h>
#include <ai_array.h>
#include <ai_nodes.h>
#include <ai_matrix.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MBoundingBox.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MDrawData.h>
#include <maya/MPlug.h>
#include <maya/MRenderUtil.h>
#include <maya/MMatrix.h>
#include <maya/MStringArray.h>
#include <maya/MString.h>
#include <maya/MDrawRequest.h>
#include <maya/MSelectionMask.h>
#include <maya/MSelectionList.h>
#include <maya/MFnNumericData.h>
#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
#include <maya/MViewport2Renderer.h>
#endif
#endif

#include <cstdio>
#include <fstream>
#include <string>

#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR       15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR       17 // pale blue
MTypeId CArnoldVolumeShape::id(ARNOLD_NODEID_VOLUME);

CStaticAttrHelper CArnoldVolumeShape::s_attributes(CArnoldVolumeShape::addAttribute);

MObject CArnoldVolumeShape::s_loadAtInit;
MObject CArnoldVolumeShape::s_stepSize;
MObject CArnoldVolumeShape::s_boundingBoxMin;
MObject CArnoldVolumeShape::s_boundingBoxMax;

MObject CArnoldVolumeShape::s_filename;
MObject CArnoldVolumeShape::s_filedata;
MObject CArnoldVolumeShape::s_disable_ray_extents;
MObject CArnoldVolumeShape::s_bounds_slack;
MObject CArnoldVolumeShape::s_step_scale;
MObject CArnoldVolumeShape::s_step_size;
MObject CArnoldVolumeShape::s_compress;

MObject CArnoldVolumeShape::s_grids;
MObject CArnoldVolumeShape::s_frame;
MObject CArnoldVolumeShape::s_padding;

MObject CArnoldVolumeShape::s_velocity_grids;
MObject CArnoldVolumeShape::s_velocity_scale;
MObject CArnoldVolumeShape::s_velocity_fps;
MObject CArnoldVolumeShape::s_velocity_shutter_start;
MObject CArnoldVolumeShape::s_velocity_shutter_end;
MObject CArnoldVolumeShape::s_velocity_threshold;


CArnoldVolumeShape::CArnoldVolumeShape()
{
   m_filename = "";
   m_grids = "";
   m_frame = 0;
   m_padding = 0.0f;
   
   m_bbox = MBoundingBox(MPoint(1, 1, 1), MPoint(-1, -1, -1));
}

CArnoldVolumeShape::~CArnoldVolumeShape()
{
}

/* override */
void CArnoldVolumeShape::postConstructor()
{
   // This call allows the shape to have shading groups assigned
   setRenderable(true);
}

MStatus CArnoldVolumeShape::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

bool CArnoldVolumeShape::isBounded() const
{
   return true;
}

MBoundingBox CArnoldVolumeShape::boundingBox() const
{
   // Returns the bounding box for the shape.
   CArnoldVolumeShape* nonConstThis = const_cast<CArnoldVolumeShape*> (this);
   MBoundingBox * bbox = nonConstThis->geometry();
   
   return MBoundingBox(bbox->min(), bbox->max());
}

void* CArnoldVolumeShape::creator()
{
   return new CArnoldVolumeShape();
}

MStatus CArnoldVolumeShape::initialize()
{
   MFnTypedAttribute tAttr;
   MFnNumericAttribute nAttr;
   MFnEnumAttribute eAttr;

   s_attributes.SetNode("volume_openvdb");

   CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);

   s_loadAtInit = nAttr.create("loadAtInit", "loadAtInit", MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_loadAtInit);
   
   s_stepSize = nAttr.create("stepSize", "stepSize", MFnNumericData::kFloat, 0);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_stepSize);
   
   s_boundingBoxMin = nAttr.create("MinBoundingBox", "min", MFnNumericData::k3Float, 0.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_boundingBoxMin);

   s_boundingBoxMax = nAttr.create("MaxBoundingBox", "max", MFnNumericData::k3Float, 0.0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_boundingBoxMax);

   s_disable_ray_extents = nAttr.create("disableRayExtents", "disableRayExtents", MFnNumericData::kBoolean, 0);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_disable_ray_extents);
   
   s_bounds_slack = nAttr.create("boundsSlack", "boundsSlack", MFnNumericData::kFloat, 0);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_bounds_slack);

   s_step_size = nAttr.create("stepSize", "stepSize", MFnNumericData::kFloat, 0);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_step_size);

   s_step_scale = nAttr.create("stepScale", "stepScale", MFnNumericData::kFloat, 1);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_step_scale);

   s_compress = nAttr.create("compress", "compress", MFnNumericData::kBoolean, 1);
   nAttr.setHidden(false);
   nAttr.setKeyable(true);
   nAttr.setStorable(true);
   addAttribute(s_compress);

   s_filename = tAttr.create("filename", "filename", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_filename);
   
   s_grids = tAttr.create("grids", "grids", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_grids);
   
   s_frame = nAttr.create("frame", "frame", MFnNumericData::kInt, 0);
   nAttr.setHidden(false);
   nAttr.setStorable(true);
   addAttribute(s_frame);
   
   s_padding = nAttr.create("padding", "padd", MFnNumericData::kFloat, 0.0f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   nAttr.setSoftMin(0.0);
   nAttr.setSoftMax(1.0);
   addAttribute(s_padding);
   
   
   
   s_velocity_grids = tAttr.create("velocityGrids", "vGrids", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_velocity_grids);
   
   s_velocity_scale = nAttr.create("velocityScale", "vScale", MFnNumericData::kFloat, 1.0f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_velocity_scale);
   
   s_velocity_fps = nAttr.create("velocityFps", "vFps", MFnNumericData::kFloat, 24.0f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_velocity_fps);
   
   s_velocity_shutter_start = nAttr.create("velocityShutterStart", "vShutterStart", MFnNumericData::kFloat, -0.25f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_velocity_shutter_start);
   
   s_velocity_shutter_end = nAttr.create("velocityShutterEnd", "vShutterEnd", MFnNumericData::kFloat, 0.25f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_velocity_shutter_end);

   s_velocity_threshold = nAttr.create("velocityThreshold", "vThreshold", MFnNumericData::kFloat, 0.001f);
   nAttr.setStorable(true);
   nAttr.setKeyable(true);
   addAttribute(s_velocity_threshold);
   
   return MStatus::kSuccess;
}

#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
/* override */
MSelectionMask CArnoldVolumeShape::getShapeSelectionMask() const
//
// Description
//     This method is overriden to support interactive object selection in Viewport 2.0
//
// Returns
//
//    The selection mask of the shape
//
{
	// Assume these are categorized as meshes for now
	MSelectionMask::SelectionType selType = MSelectionMask::kSelectMeshes;
	return selType;
}

MStatus CArnoldVolumeShape::setDependentsDirty( const MPlug& plug, MPlugArray& plugArray)
{
   /*
	// If more attributes are added which require update, they
	// shoukd be added here
	if (plug == s_type ||
		plug == s_dso ||
		plug == s_data ||
		plug == s_loadAtInit ||
		plug == s_stepSize ||
		plug == s_boundingBoxMin ||
		plug == s_boundingBoxMax ||

		plug == s_filename ||
		plug == s_grids ||
		plug == s_frame ||
		plug == s_padding ||

		plug == s_velocity_grids ||
		plug == s_velocity_scale ||
		plug == s_velocity_fps ||
		plug == s_velocity_shutter_start ||
		plug == s_velocity_shutter_end || 
      plug == s_velocity_threshold  
      )
	{
		// Signal to VP2 that we require an update
		MHWRender::MRenderer::setGeometryDrawDirty(thisMObject());
	}*/
	return MS::kSuccess;
}
#endif
#endif

MBoundingBox* CArnoldVolumeShape::geometry()
{
   MString tmpFilename = m_filename;
   MString tmpGrids = m_grids;
   int tmpFrame = m_frame;
   float tmpPadding = m_padding;

   MObject this_object = thisMObject();
   MPlug plug(this_object, s_filename);
   plug.getValue(m_filename);
   
   plug.setAttribute(s_grids);
   plug.getValue(m_grids);
   
   plug.setAttribute(s_frame);
   plug.getValue(m_frame);
   
   plug.setAttribute(s_padding);
   plug.getValue(m_padding);
   
   
   if (m_filename != tmpFilename ||
       m_grids != tmpGrids || m_frame != tmpFrame || m_padding != tmpPadding)
   {
      int start = 0;
      int end = 0;
      MString expandedFilename = "";
      char frameExt[64];

      start = m_filename.index('#');
      end = m_filename.rindex('#');
      
      if(start >= 0)
      {
         sprintf(frameExt, "%0*d", end - start + 1, m_frame);
         expandedFilename = m_filename.substring(0,start-1) + frameExt + m_filename.substring(end+1,m_filename.length());
      }
      else
      {
         expandedFilename = m_filename;
      }
      expandedFilename = expandedFilename.expandEnvironmentVariablesAndTilde();
      
      MString cmd;
      cmd.format("import mtoa.volume_vdb; mtoa.volume_vdb.GetChannelBounds('^1s', '^2s')", expandedFilename, m_grids);

      MStringArray result;
      MGlobal::executePythonCommand(cmd, result);   

      if (result.length() >= 6)   
      {
         MPoint minBox(result[0].asFloat(), result[1].asFloat(), result[2].asFloat(), 0.f);
         MPoint maxBox(result[3].asFloat(), result[4].asFloat(), result[5].asFloat(), 0.f);
      } else
      {
         m_bbox = MBoundingBox (MPoint(-1,-1,-1), MPoint(1,1,1));
      }
   }
   return &m_bbox;
}


// UI IMPLEMENTATION

CArnoldVolumeShapeUI::CArnoldVolumeShapeUI()
{
}
CArnoldVolumeShapeUI::~CArnoldVolumeShapeUI()
{
}

void* CArnoldVolumeShapeUI::creator()
{
   return new CArnoldVolumeShapeUI();
}

void CArnoldVolumeShapeUI::getDrawRequests(const MDrawInfo & info, bool /*objectAndActiveOnly*/,
      MDrawRequestQueue & queue)
{   
   // The draw data is used to pass geometry through the
   // draw queue. The data should hold all the information
   // needed to draw the shape.
   //
   MDrawData data;
   MDrawRequest request = info.getPrototype(*this);
   CArnoldVolumeShape* shapeNode = reinterpret_cast<CArnoldVolumeShape*>(surfaceShape());
   if(shapeNode != NULL)
   {
      MBoundingBox * bbox = shapeNode->geometry();
      getDrawData(bbox, data);
      request.setDrawData(data);
   }
   
   // Use mode status to determine how to display object
   // why was there a switch if everything executed the same code??
   getDrawRequestsWireFrame(request, info);
   queue.add(request);
}

void CArnoldVolumeShapeUI::draw(const MDrawRequest & request, M3dView & view) const
{
   MDrawData data = request.drawData();
   MBoundingBox * bbox = (MBoundingBox*) data.geometry();
   view.beginGL();
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

   
   {
      MPoint bbMin = bbox->min();
      MPoint bbMax = bbox->max();

      float minCoords[4];
      float maxCoords[4];

      bbMin.get(minCoords);
      bbMax.get(maxCoords);
   
      MBoundingBox m_bbox = MBoundingBox (minCoords, maxCoords);
      float minPt[4];
      float maxPt[4];
      m_bbox.min().get(minPt);
      m_bbox.max().get(maxPt);
      const float bottomLeftFront[3] =
      { minPt[0], minPt[1], minPt[2] };
      const float topLeftFront[3] =
      { minPt[0], maxPt[1], minPt[2] };
      const float bottomRightFront[3] =
      { maxPt[0], minPt[1], minPt[2] };
      const float topRightFront[3] =
      { maxPt[0], maxPt[1], minPt[2] };
      const float bottomLeftBack[3] =
      { minPt[0], minPt[1], maxPt[2] };
      const float topLeftBack[3] =
      { minPt[0], maxPt[1], maxPt[2] };
      const float bottomRightBack[3] =
      { maxPt[0], minPt[1], maxPt[2] };
      const float topRightBack[3] =
      { maxPt[0], maxPt[1], maxPt[2] };

      glBegin(GL_LINE_STRIP);
      glVertex3fv(bottomLeftFront);
      glVertex3fv(bottomLeftBack);
      glVertex3fv(topLeftBack);
      glVertex3fv(topLeftFront);
      glVertex3fv(bottomLeftFront);
      glVertex3fv(bottomRightFront);
      glVertex3fv(bottomRightBack);
      glVertex3fv(topRightBack);
      glVertex3fv(topRightFront);
      glVertex3fv(bottomRightFront);
      glEnd();

      glBegin(GL_LINES);
      glVertex3fv(bottomLeftBack);
      glVertex3fv(bottomRightBack);

      glVertex3fv(topLeftBack);
      glVertex3fv(topRightBack);

      glVertex3fv(topLeftFront);
      glVertex3fv(topRightFront);
      glEnd();
   }
   
   glPopAttrib();
   view.endGL();
}

void CArnoldVolumeShapeUI::getDrawRequestsWireFrame(MDrawRequest& request, const MDrawInfo& info)
{
   request.setToken(kDrawBoundingBox);
   M3dView::DisplayStatus displayStatus = info.displayStatus();
   M3dView::ColorTable activeColorTable = M3dView::kActiveColors;
   M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;
   switch (displayStatus)
   {
   case M3dView::kLead:
      request.setColor(LEAD_COLOR, activeColorTable);
      break;
   case M3dView::kActive:
      request.setColor(ACTIVE_COLOR, activeColorTable);
      break;
   case M3dView::kActiveAffected:
      request.setColor(ACTIVE_AFFECTED_COLOR, activeColorTable);
      break;
   case M3dView::kDormant:
      request.setColor(DORMANT_COLOR, dormantColorTable);
      break;
   case M3dView::kHilite:
      request.setColor(HILITE_COLOR, activeColorTable);
      break;
   default:
      break;
   }

}

bool CArnoldVolumeShapeUI::select(MSelectInfo &selectInfo, MSelectionList &selectionList,
      MPointArray &worldSpaceSelectPts) const
//
// Select function. Gets called when the bbox for the object is selected.
// This function just selects the object without doing any intersection tests.
//
{
   MSelectionMask priorityMask(MSelectionMask::kSelectObjectsMask);
   MSelectionList item;
   item.add(selectInfo.selectPath());
   MPoint xformedPt;
   selectInfo.addSelection(item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, false);
   return true;
}

