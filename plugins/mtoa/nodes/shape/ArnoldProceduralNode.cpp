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

#include <ai_node_entry.h>
#include <ai_params.h>
#include <ai_metadata.h>
CAbMayaNode CArnoldProceduralNode::s_abstract;

std::vector<CStaticAttrHelper> CArnoldProceduralNode::s_nodeHelpers;

static unordered_map<std::string, std::vector<std::string> >  s_proceduralParameters;

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

   s_nodeHelpers.push_back(helper);

   return MS::kSuccess;
}

MBoundingBox CArnoldProceduralNode::boundingBox() const
{
   MBoundingBox box;
   box.expand(MPoint(1.f, 1.f, 1.f));
   box.expand(MPoint(-1.f, -1.f, -1.f));
   return box;
}
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
   MSelectionMask::SelectionType selType = MSelectionMask::kSelectMeshes;
    return MSelectionMask( selType );
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
  

}

bool CArnoldProceduralNodeUI::select(MSelectInfo &selectInfo, MSelectionList &selectionList,
      MPointArray &worldSpaceSelectPts) const
{

   MSelectionMask priorityMask(MSelectionMask::kSelectMeshes);
   MSelectionList item;
   item.add(selectInfo.selectPath());
   MPoint xformedPt;

   selectInfo.addSelection(item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, false);
   return true;

}

