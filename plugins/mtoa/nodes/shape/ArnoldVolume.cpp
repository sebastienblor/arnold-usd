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

MObject CArnoldVolumeShape::s_type;
MObject CArnoldVolumeShape::s_dso;
MObject CArnoldVolumeShape::s_data;
MObject CArnoldVolumeShape::s_loadAtInit;
MObject CArnoldVolumeShape::s_stepSize;
MObject CArnoldVolumeShape::s_boundingBoxMin;
MObject CArnoldVolumeShape::s_boundingBoxMax;

MObject CArnoldVolumeShape::s_filename;
MObject CArnoldVolumeShape::s_grids;
MObject CArnoldVolumeShape::s_frame;
MObject CArnoldVolumeShape::s_padding;

MObject CArnoldVolumeShape::s_velocity_grids;
MObject CArnoldVolumeShape::s_velocity_scale;
MObject CArnoldVolumeShape::s_velocity_fps;
MObject CArnoldVolumeShape::s_velocity_shutter_start;
MObject CArnoldVolumeShape::s_velocity_shutter_end;
MObject CArnoldVolumeShape::s_velocity_threshold;

enum VolumeType{
   VT_CUSTOM,
   VT_OPEN_VDB
};
   

CArnoldVolumeShape::CArnoldVolumeShape()
{
   m_type = 0;
   m_dso = "";
   m_data = "";
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

   s_attributes.SetNode("volume");

   CDagTranslator::MakeArnoldVisibilityFlags(s_attributes);

   s_type = eAttr.create("type", "type", 1);
   eAttr.addField("Custom", VT_CUSTOM);
   eAttr.addField("OpenVDB", VT_OPEN_VDB);
   addAttribute(s_type);
   
   s_dso = tAttr.create("dso", "dso", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_dso);
   
   s_data = tAttr.create("data", "data", MFnData::kString);
   tAttr.setHidden(false);
   tAttr.setStorable(true);
   addAttribute(s_data);
   
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
	}
	return MS::kSuccess;
}
#endif
#endif

MBoundingBox* CArnoldVolumeShape::geometry()
{
   int tmpType = m_type;
   MString tmpDso = m_dso;
   MString tmpData = m_data;
   MString tmpFilename = m_filename;
   MString tmpGrids = m_grids;
   int tmpFrame = m_frame;
   float tmpPadding = m_padding;

   MObject this_object = thisMObject();
   MPlug plug(this_object, s_type);
   plug.getValue(m_type);
   
   plug.setAttribute(s_dso);
   plug.getValue(m_dso);

   plug.setAttribute(s_data);
   plug.getValue(m_data);
   
   plug.setAttribute(s_filename);
   plug.getValue(m_filename);
   
   plug.setAttribute(s_grids);
   plug.getValue(m_grids);
   
   plug.setAttribute(s_frame);
   plug.getValue(m_frame);
   
   plug.setAttribute(s_padding);
   plug.getValue(m_padding);
   
   
   if (m_type != tmpType || m_dso != tmpDso || m_data != tmpData || m_filename != tmpFilename ||
       m_grids != tmpGrids || m_frame != tmpFrame || m_padding != tmpPadding)
   {
      if (AiUniverseIsActive())
         return &m_bbox;
      
      bool AiUniverseCreated = false;
      AiUniverseCreated = ArnoldUniverseBegin();
      
      AtNode* options = AiUniverseGetOptions();
      AiNodeSetBool(options, "preserve_scene_data", true);
      AiNodeSetBool(options, "skip_license_check", true);
      
      
      AtNode* volume = AiNode("volume");;
      AiNodeSetFlt(volume, "step_size", 0.0f);
      AiNodeSetBool(volume, "load_at_init", true);
      
      AiNodeSetStr(volume, "name", "myvolume");

      AiNodeSetMatrix(volume, "matrix", AiM4Identity());

	  MString dso;
      if(m_type == VT_CUSTOM)
      {
         dso = m_dso.expandEnvironmentVariablesAndTilde();
	  }
	  else // openvdb
	  {
         dso = MString(getenv("MTOA_PATH")) + MString("/procedurals/volume_openvdb.so");
	  }
	  unsigned int nchars = dso.numChars();
	  if (nchars > 3 && dso.substringW(nchars-3, nchars) == ".so")
	  {
		  dso = dso.substringW(0, nchars-4)+LIBEXT;
	  }
	  else if (nchars > 4 && dso.substringW(nchars-4, nchars) == ".dll")
	  {
		  dso = dso.substringW(0, nchars-5)+LIBEXT;
	  }
	  else if (nchars > 6 && dso.substringW(nchars-6, nchars) == ".dylib")
	  {
		  dso = dso.substringW(0, nchars-7)+LIBEXT;
	  }

      if(m_type == VT_CUSTOM)
      {
         AiNodeSetStr(volume, "dso", dso.asChar());
      
         int sizeData = strlen(m_data.asChar());
         if (sizeData != 0)
         {
            AiNodeSetStr(volume, "data", m_data.expandEnvironmentVariablesAndTilde().asChar());
         }
      }
      else // openvdb
      {
		 AiNodeSetStr(volume, "dso", dso.expandEnvironmentVariablesAndTilde().asChar());
         
         int start = 0;
         int end = 0;
         MString newFilename = "";
         char frameExt[64];

         start = m_filename.index('#');
         end = m_filename.rindex('#');
         
         if(start >= 0)
         {
            sprintf(frameExt, "%0*d", end - start + 1, m_frame);
            newFilename = m_filename.substring(0,start-1) + frameExt + m_filename.substring(end+1,m_filename.length());
         }
         else
         {
            newFilename = m_filename;
         }
         
         AiNodeDeclare( volume, "filename", "constant STRING" );
         AiNodeSetStr( volume, "filename", newFilename.expandEnvironmentVariablesAndTilde().asChar() );
         
         MStringArray gridList;
         m_grids.split(' ',gridList);
         
         if (gridList.length() > 0)
         {
            AiNodeDeclare( volume, "grids", "constant ARRAY STRING" );
            AtArray *ary = AiArrayAllocate(gridList.length(), 1, AI_TYPE_STRING);
            for(unsigned int i = 0; i < gridList.length(); i++)
            {
               AiArraySetStr(ary, i, gridList[i].asChar());
            }
            AiNodeSetArray( volume, "grids", ary);
         }
      }

      // create a lambert shader
      //AtNode *shader = AiNode("density");
      //AiNodeSetStr(shader, "name", "mydensity");

      // assign the sphere's shader
      //AiNodeSetPtr(volume, "shader", shader);
      
      // create a perspective camera
      AtNode *camera = AiNode("persp_camera");
      AiNodeSetStr(camera, "name", "mycamera");
      
      if (AiRender(AI_RENDER_MODE_FREE) != AI_SUCCESS)
      {
         if (AiUniverseCreated) ArnoldUniverseEnd();
         m_bbox = MBoundingBox (MPoint(-1,-1,-1), MPoint(1,1,1));
         return &m_bbox;
      }
      
      AtBBox bbox;
      bbox = AiUniverseGetSceneBounds();
      
      if (AiUniverseCreated) ArnoldUniverseEnd();

	  if (bbox.isEmpty())
	  {
		 m_bbox = MBoundingBox (MVector(-1,-1,-1), MVector(1,1,1));
         return &m_bbox;
	  }

      float minCoords[4];
      float maxCoords[4];
      
      minCoords[0] = bbox.min.x;
      minCoords[1] = bbox.min.y;
      minCoords[2] = bbox.min.z;
      minCoords[3] = 0.0f;

      maxCoords[0] = bbox.max.x;
      maxCoords[1] = bbox.max.y;
      maxCoords[2] = bbox.max.z;
      maxCoords[3] = 0.0f;
      
      m_bbox = MBoundingBox (minCoords, maxCoords);
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

