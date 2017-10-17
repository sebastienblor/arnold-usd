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

      attr.setHidden(true);
   }

   // Now re-enable Arnold attributes 
   const std::vector<std::string> &nodeParameters = s_proceduralParameters[m_abstract.name.asChar()];

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

         // skip the special "name" parameter
         // Also skip the parameters existing natively in maya shapes (receive_shadows, visibility, matrix)
         if (paramNameStr != "name" && paramNameStr != "receive_shadows" && paramNameStr != "visibility" && paramNameStr != "matrix")
         {
            CAttrData attrData;
            helper.GetAttrData(paramName, attrData);
            MObject attr = helper.MakeInput(attrData);
         }
      }
   }
   AiParamIteratorDestroy(nodeParam);
   s_nodeHelpers.push_back(helper);

   return MS::kSuccess;
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

