#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>


#include "ArnoldProceduralNode.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"
#include "attributes/Metadata.h"
#include "render/AOV.h"
#include "extension/ExtensionsManager.h"
#include "common/UnorderedContainer.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MRenderUtil.h>
#include <maya/MGlobal.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MStatus.h>
#include <maya/MFloatVector.h>
#include <maya/MSelectionList.h>
#include <maya/MDrawRequest.h>
#include <maya/M3dView.h>
#include <maya/MDrawData.h>
#include <maya/MDrawInfo.h>
#include <maya/MSelectInfo.h>
#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
#include <maya/MViewport2Renderer.h>
#endif
#endif

#include <ai_node_entry.h>
#include <ai_params.h>
#include <ai_metadata.h>
CAbMayaNode CArnoldProceduralNode::s_abstract;

std::vector<CStaticAttrHelper> CArnoldProceduralNode::s_nodeHelpers;

static unordered_map<std::string, std::vector<std::string> >  s_proceduralParameters;


#define LEAD_COLOR            18 // green
#define ACTIVE_COLOR       15 // white
#define ACTIVE_AFFECTED_COLOR 8  // purple
#define DORMANT_COLOR         4  // blue
#define HILITE_COLOR       17 // pale blue

// FIXME to be implemented properly
void CArnoldProceduralNode::postConstructor()
{
   // This call allows the shape to have shading groups assigned
   setRenderable(true);

   // TODO: use a metadata to define this
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   // No compute anyway
   setMPSafe(false);
   // Copy the abstract so that it can accessed on instances
   // (and saved before a new register overwrites it)
   m_abstract = s_abstract;

   MObject me = thisMObject();    
   MFnDependencyNode node(me);

   // First disable all Maya native parameters
   unsigned int attrs = node.attributeCount();
   for (unsigned int i = 0; i < attrs; ++i)
   {
      MObject attrObj = node.attribute(i);
      MStatus status;
      MFnAttribute attr(attrObj, &status);
      if (status != MS::kSuccess)
         continue;

      MString attrName = attr.name();
      // These are Maya attributes we want to keep for our export
      if (attrName == MString("visibility") || attrName == MString("primaryVisibility") || attrName == MString("castsShadows"))
         continue;

      // These parameters are created in CProceduralTranslator::NodeInitializer
      // and they don't have the "ai" prefix
      if (attrName == "overrideLightLinking" || attrName == "overrideShaders")
         continue;

      if (attrName.length() > 2 && attrName.substringW(0, 1) == MString("ai"))
         continue;

      attr.setHidden(true);
   }
   MString nodeType = typeName();
   
   // Now re-enable Arnold attributes 
   const std::vector<std::string> &nodeParameters = s_proceduralParameters[nodeType.asChar()];

   for (size_t i = 0; i < nodeParameters.size(); ++i)
   {
      MObject attrObj = node.attribute(MString(nodeParameters[i].c_str()));
      MStatus status;
      MFnAttribute attr(attrObj, &status);
      if (status != MS::kSuccess)
         continue;

      attr.setHidden(false);
   }
}

MStatus CArnoldProceduralNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldProceduralNode::creator()
{
   return new CArnoldProceduralNode();
}

MStatus CArnoldProceduralNode::initialize()
{
   MFnAttribute fnAttr;
   MFnNumericAttribute nAttr;
 
   MString maya = s_abstract.name;

   std::vector<std::string> &nodeParameters = s_proceduralParameters[maya.asChar()];

   // Register this node as being a custom Arnold shape.
   // This way it will appear in the dedicated menu #3212
   CExtensionsManager::AddCustomShape(maya);

   MString arnold = s_abstract.arnold;
   MString classification = s_abstract.classification;
   MString provider = s_abstract.provider;
   const AtNodeEntry *nodeEntry = AiNodeEntryLookUp(arnold.asChar());

   CStaticAttrHelper helper(CArnoldProceduralNode::addAttribute, nodeEntry);

   static unordered_set<std::string> s_ignoredArnoldParams;
   if (s_ignoredArnoldParams.empty())
   {
      // skip all parameters belonging to common procedural parameters
      // so that we only keep those defined by the user
      // FIXME is there a better way to do this ? we should rather 
      // create a dummy procedural class to find out if the attribute exists in there
      s_ignoredArnoldParams.insert("name");
      s_ignoredArnoldParams.insert("receive_shadows");
      s_ignoredArnoldParams.insert("visibility");
      s_ignoredArnoldParams.insert("matrix");
      s_ignoredArnoldParams.insert("matte");
      s_ignoredArnoldParams.insert("opaque");
      s_ignoredArnoldParams.insert("sidedness");
      s_ignoredArnoldParams.insert("self_shadows");
      s_ignoredArnoldParams.insert("shader");
      s_ignoredArnoldParams.insert("light_group");
      s_ignoredArnoldParams.insert("trace_sets");
      s_ignoredArnoldParams.insert("shadow_group");
      s_ignoredArnoldParams.insert("invert_normals");
      s_ignoredArnoldParams.insert("ray_bias");
      s_ignoredArnoldParams.insert("transform_type");
      s_ignoredArnoldParams.insert("use_light_group");
      s_ignoredArnoldParams.insert("use_shadow_group");
      s_ignoredArnoldParams.insert("motion_start");
      s_ignoredArnoldParams.insert("motion_end");
      s_ignoredArnoldParams.insert("id");
      s_ignoredArnoldParams.insert("override_nodes");
      s_ignoredArnoldParams.insert("namespace");
   }

   // inputs
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(nodeEntry);
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);
      std::string paramNameStr(paramName);
      
      bool hide = false;
      if (!AiMetaDataGetBool(nodeEntry, paramName, "maya.hide", &hide) || !hide)
      {
         nodeParameters.push_back(paramNameStr);

         // check if this parameter is in the ignore list
         if (s_ignoredArnoldParams.find(paramNameStr) == s_ignoredArnoldParams.end())
         {
            CAttrData attrData;
            helper.GetAttrData(paramName, attrData);
            MObject attr = helper.MakeInput(attrData);
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);

   CAttrData data;
   data.defaultValue.BOOL() = false;
   data.name = "overrideReceiveShadows";
   data.shortName = "overrideReceiveShadows";
   helper.MakeInputBoolean(data);
   nodeParameters.push_back(data.name.asChar());
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideDoubleSided";
   data.shortName = "overrideDoubleSided";
   helper.MakeInputBoolean(data);
   nodeParameters.push_back(data.name.asChar());

   data.defaultValue.BOOL() = false;
   data.name = "overrideSelfShadows";
   data.shortName = "overrideSelfShadows";
   helper.MakeInputBoolean(data);
   nodeParameters.push_back(data.name.asChar());
   
   data.defaultValue.BOOL() = false;
   data.name = "overrideOpaque";
   data.shortName = "overrideOpaque";
   helper.MakeInputBoolean(data);
   nodeParameters.push_back(data.name.asChar());

   data.defaultValue.BOOL() = false;
   data.name = "overrideMatte";
   data.shortName = "overrideMatte";
   helper.MakeInputBoolean(data);
   nodeParameters.push_back(data.name.asChar());

   data.defaultValue.VEC() = AtVector(-1.f, -1.f, -1.f);
   data.name = "minBoundingBox";
   data.shortName = "min";
   helper.MakeInputVector(data);
   nodeParameters.push_back(data.name.asChar());

   data.defaultValue.VEC() = AtVector(1.f, 1.f, 1.f);
   data.name = "maxBoundingBox";
   data.shortName = "max";
   helper.MakeInputVector(data);
   nodeParameters.push_back(data.name.asChar());

   s_nodeHelpers.push_back(helper);

   return MS::kSuccess;
}

MBoundingBox CArnoldProceduralNode::boundingBox() const
{
   MBoundingBox box;
   MFnDependencyNode depNode(thisMObject());
   MPlug minPlug = depNode.findPlug("minBoundingBox", true);
   MPlug maxPlug = depNode.findPlug("maxBoundingBox", true);

   if (minPlug.isNull() || maxPlug.isNull())
   {
      box.expand(MPoint(1.f, 1.f, 1.f));
      box.expand(MPoint(-1.f, -1.f, -1.f));
   } else
   {
      box.expand(MPoint(minPlug.child(0).asFloat(), minPlug.child(1).asFloat(), minPlug.child(2).asFloat()));
      box.expand(MPoint(maxPlug.child(0).asFloat(), maxPlug.child(1).asFloat(), maxPlug.child(2).asFloat()));
   }   

   return box;
}

#ifdef ENABLE_VP2
#if MAYA_API_VERSION >= 201700
/* override */
MSelectionMask CArnoldProceduralNode::getShapeSelectionMask() const
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

MStatus CArnoldProceduralNode::setDependentsDirty( const MPlug& plug, MPlugArray& plugArray)
{
   MString plugName = plug.partialName();
   if (plugName == "max" || plugName == "maxx" || plugName == "maxy" || plugName == "maxz" ||
      plugName == "min" || plugName == "minx" || plugName == "miny" || plugName == "minz" )
   {
      // Signal to VP2 that we require an update
      MHWRender::MRenderer::setGeometryDrawDirty(thisMObject());
   }

   return MS::kSuccess;
}
#endif
#endif

MBoundingBox* CArnoldProceduralNode::geometry()
{
   m_bbox = boundingBox();      
   return &m_bbox;
}

// FIXME to be implemented properly
CArnoldProceduralNodeUI::CArnoldProceduralNodeUI()
{
}
CArnoldProceduralNodeUI::~CArnoldProceduralNodeUI()
{
}

void* CArnoldProceduralNodeUI::creator()
{
   return new CArnoldProceduralNodeUI();
}

void CArnoldProceduralNodeUI::getDrawRequests(const MDrawInfo & info, bool /*objectAndActiveOnly*/,
      MDrawRequestQueue & queue)
{
   
}

void CArnoldProceduralNodeUI::draw(const MDrawRequest & request, M3dView & view) const
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

void CArnoldProceduralNodeUI::getDrawRequestsWireFrame(MDrawRequest& request, const MDrawInfo& info)
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
bool CArnoldProceduralNodeUI::select(MSelectInfo &selectInfo, MSelectionList &selectionList,
      MPointArray &worldSpaceSelectPts) const
{

   MSelectionMask priorityMask(MSelectionMask::kSelectObjectsMask);
   MSelectionList item;
   item.add(selectInfo.selectPath());
   MPoint xformedPt;
   selectInfo.addSelection(item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, false);
   return true;

}

