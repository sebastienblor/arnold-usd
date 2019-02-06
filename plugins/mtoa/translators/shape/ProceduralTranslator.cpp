#include "ProceduralTranslator.h"
#include "attributes/AttrHelper.h"
#include "../NodeTranslatorImpl.h"
#include "utils/time.h"

#include <ai_msg.h>
#include <ai_nodes.h>
#include <ai_ray.h>

#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MString.h>

void CProceduralTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "procedural");
   CShapeTranslator::MakeCommonAttributes(helper);

   CAttrData data;
   data.defaultValue.BOOL() = true;
   data.name = "overrideLightLinking";
   data.shortName = "oll";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.BOOL() = true;
   data.name = "overrideShaders";
   data.shortName = "osh";
   helper.MakeInputBoolean(data);
}

AtNode* CProceduralTranslator::CreateArnoldNodes()
{
   m_attrChanged = false;

   if (IsMasterInstance())
      return AddArnoldNode("procedural");
   else
      return AddArnoldNode("ginstance");
}

// FIXME : please remove all these hacks regarding the "override" attributes 
// once we no longer case about pre-2.0.2 compatibility
static void ArnoldStandInVisibilityOverride(AtByte &visibility, MPlug overridePlug, 
                                                MPlug visPlug, AtByte rayType)
{
   if (visPlug.isNull())
      return; // nothing to do here

   // FIXME to be removed once we get rid of these "override" attributes
   if (!overridePlug.isNull() && !overridePlug.asBool())
   {
      // the deprecated override parameter was left to False
      // need to set it to True so that we stop caring about it
      overridePlug.setBool(true);

      // if override was OFF and Visibility was OFF
      // it actually meant that my Visibility was ON
      if (!visPlug.asBool())
         visPlug.setBool(true);
   }

   // I can only remove visibility as compared to full visibility
   if (!visPlug.asBool())
      visibility &= ~rayType;
}
/**
*   Standins visibility override is a bit special  :
*   The procedural visibility will determine which rays make it to the procedural node. Then, the child node 
*   keeps its own visibility value (i.e. it doesn't inherit from the procedural parent as for other attributes).
*   This means that the resulting visibility value for the nested objects is the "intersection" of their own value
*   and the procedural's one. So the only thing we can do here is to remove bits of the visibility value.
**/
AtByte CProceduralTranslator::ComputeOverrideVisibility()
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (!IsRenderable())
      return AI_RAY_UNDEFINED;

   AtByte visibility = AI_RAY_ALL;

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideCastsShadows"), 
                                                FindMayaPlug("castsShadows"), 
                                                AI_RAY_SHADOW);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overridePrimaryVisibility"), 
                                                FindMayaPlug("primaryVisibility"), 
                                                AI_RAY_CAMERA);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInDiffuseReflection"), 
                                                FindMayaPlug("aiVisibleInDiffuseReflection"), 
                                                AI_RAY_DIFFUSE_REFLECT);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInSpecularReflection"), 
                                                FindMayaPlug("aiVisibleInSpecularReflection"), 
                                                AI_RAY_SPECULAR_REFLECT);
   
   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInDiffuseTransmission"), 
                                                FindMayaPlug("aiVisibleInDiffuseTransmission"), 
                                                AI_RAY_DIFFUSE_TRANSMIT);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInSpecularTransmission"), 
                                                FindMayaPlug("aiVisibleInSpecularTransmission"), 
                                                AI_RAY_SPECULAR_TRANSMIT);

   ArnoldStandInVisibilityOverride(visibility, FindMayaPlug("overrideVisibleInVolume"), 
                                                FindMayaPlug("aiVisibleInVolume"), 
                                                AI_RAY_VOLUME);

   return visibility;
}

/// overrides CShapeTranslator::ProcessRenderFlags to ensure that we don't set aiOpaque unless overrideOpaque is enabled
void CProceduralTranslator::ProcessRenderFlags(AtNode* node)
{
   // Once we can get rid of pre-2.0.2 compatibility, we can simply invoke ComputeVisibility here...
   // For now we're testing if one of the legacy attributes exists
   if (FindMayaPlug("overridePrimaryVisibility").isNull())
      AiNodeSetByte(node, "visibility", ComputeVisibility());   
   else
      AiNodeSetByte(node, "visibility", ComputeOverrideVisibility());

   MPlug plug;
   
   plug = FindProceduralPlug("overrideSelfShadows");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("aiSelfShadows");
      if (!plug.isNull()) AiNodeSetBool(node, "self_shadows", plug.asBool());
   }

   // for standins, we check
   plug = FindProceduralPlug("overrideOpaque");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("aiOpaque");
      if (!plug.isNull()) AiNodeSetBool(node, "opaque", plug.asBool());
   }
   
   plug = FindProceduralPlug("overrideReceiveShadows");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("receiveShadows");
      if (!plug.isNull()) AiNodeSetBool(node, "receive_shadows", plug.asBool());
   }
   
   plug = FindProceduralPlug("overrideDoubleSided");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("doubleSided");
      
      if (!plug.isNull() && plug.asBool())
         AiNodeSetByte(node, "sidedness", AI_RAY_ALL);
      else
         AiNodeSetByte(node, "sidedness", 0);
   }
   
   // for standins, we check
   plug = FindProceduralPlug("overrideMatte");
   if (plug.isNull() || plug.asBool())
   {
      plug = FindMayaPlug("aiMatte");
      if (!plug.isNull()) AiNodeSetBool(node, "matte", plug.asBool());
   }
   if (RequiresMotionData())
   {
      double motionStart, motionEnd;
      GetSessionOptions().GetMotionRange(motionStart, motionEnd);
      AiNodeSetFlt(node, "motion_start", (float)motionStart);
      AiNodeSetFlt(node, "motion_end", (float)motionEnd);
   }

   // eventually export dcc_name user attribute
   // FIXME this could be moved to a separate function but this might be temporary
   if (GetSessionOptions().GetExportFullPath() || GetSessionOptions().GetExportPrefix().length() > 0)
   {
      if (AiNodeLookUpUserParameter(node, "dcc_name") == NULL)
         AiNodeDeclare(node, "dcc_name", "constant STRING");
   
      MString partialName = m_dagPath.partialPathName();
      AiNodeSetStr(node, "dcc_name", AtString(partialName.asChar()));
   }

   if (!GetSessionOptions().GetExportFullPath() || GetSessionOptions().GetExportPrefix().length() > 0)
   {
      if (AiNodeLookUpUserParameter(node, "maya_full_name") == NULL)
         AiNodeDeclare(node, "maya_full_name", "constant STRING");
   
      MString fullName = m_dagPath.fullPathName();
      AiNodeSetStr(node, "maya_full_name", AtString(fullName.asChar()));
   }

}

void CProceduralTranslator::Export(AtNode* anode)
{
   m_attrChanged = false;
   const char* nodeType = AiNodeEntryGetName(AiNodeGetNodeEntry(anode));
   if (strcmp(nodeType, "ginstance") == 0)
   {
      ExportInstance(anode, GetMasterInstance());
   }
   else
   {
      ExportProcedural(anode);
   }
}

void CProceduralTranslator::ExportMotion(AtNode* anode)
{
   ExportMatrix(anode);
}

// Deprecated : Arnold support procedural instance, but it's not safe.
//
AtNode* CProceduralTranslator::ExportInstance(AtNode *instance, const MDagPath& masterInstance)
{
   MFnDependencyNode masterDepNode(masterInstance.node());
   MPlug dummyPlug = masterDepNode.findPlug("matrix", true);
   // in case master instance wasn't exported (#648)
   // and also to create the reference between both translators
   AtNode *masterNode = (dummyPlug.isNull()) ? NULL : ExportConnectedNode(dummyPlug);
   

   //AiNodeSetStr(instance, "name", CDagTranslator::GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(instance);

   AiNodeSetPtr(instance, "node", masterNode);
   AiNodeSetBool(instance, "inherit_xform", false);
  
   // #2858 we must recompute the visibility instead of getting the master's one
   // since we're not sure the master has already been exported 
   //AtByte visibility = AiNodeGetByte(masterNode, "visibility");
   //AiNodeSetByte(instance, "visibility", visibility);
   ProcessRenderFlags(instance);

   m_DagNode.setObject(masterInstance);
   
   if (FindProceduralPlug("overrideShaders").asBool() &&
      RequiresShaderExport())
   {
      ExportShaders();
   }
   if (FindProceduralPlug("overrideLightLinking").asBool())
   {
      ExportLightLinking(instance);
   }

   return instance;
}

void CProceduralTranslator::ExportShaders()
{
   AtNode *node = GetArnoldNode();
   if (node == NULL)
      return;

   int instanceNum = m_dagPath.isInstanced() ? m_dagPath.instanceNumber() : 0;

   MPlug shadingGroupPlug = GetNodeShadingGroup(m_dagPath.node(), instanceNum);
   if (!shadingGroupPlug.isNull())
   {
      AtNode *shader = ExportConnectedNode(shadingGroupPlug);
      if (shader != NULL)
      {
         AiNodeSetPtr(node, "shader", shader);
      }
      else
      {         
         AiNodeSetPtr(node, "shader", NULL);
      }
   }
}

AtNode* CProceduralTranslator::ExportProcedural(AtNode* procedural)
{
   m_DagNode.setObject(m_dagPath.node());

   // Why did we use to set the name here ? this is already done in AddArnoldNode
   //AiNodeSetStr(procedural, "name", GetArnoldNaming(m_dagPath).asChar());

   ExportMatrix(procedural);
   ProcessRenderFlags(procedural);
   if (FindProceduralPlug("overrideShaders").asBool())
      ExportShaders();
   
   if (FindProceduralPlug("overrideLightLinking").asBool())
      ExportLightLinking(procedural);
   
   AiNodeSetBool(procedural, "override_nodes", FindProceduralPlug("overrideNodes").asBool());

   MString nsName = m_DagNode.findPlug("aiNamespace", true).asString();
   if (nsName.length() > 0)
      AiNodeSetStr(procedural, "namespace", nsName.asChar());
   else
      AiNodeResetParameter(procedural, "namespace");


   // Now check the "operators" input. In Arnold it's a single node pointer, whereas MtoA provides an array.
   // If there are multiple inputs in the maya node we'll insert a merge op in the middle
   std::vector<AtNode *> inputOps;
   MPlug ops = FindMayaPlug("operators");
   unsigned nelems = ops.numElements();
   MPlug elemPlug;
   // FIXME this is a tempory workaround to get the operators translating in the correct order
   // To be transferred to operators instead
   for (int i = nelems; i > -1; --i)
   // for (unsigned int i = 0; i < nelems; ++i)
   {
      elemPlug = ops[i];       
      MPlugArray connections;
      elemPlug.connectedTo(connections, true, false);
      if (connections.length() > 0)
      {
         AtNode *op = ExportConnectedNode(connections[0]); // ensure this operator is exported
         if (op)
            inputOps.push_back(op); // append the operator node to the list 
      }
   }
   if (inputOps.empty())
      AiNodeResetParameter(procedural, "operator"); // don't forget to clear this attribute for IPR updates
   else if (inputOps.size() == 1)
      AiNodeSetPtr(procedural, "operator", (void*)inputOps[0]); // single input, basic translation
   else
   {
      // Multiple ops, let's insert a merge operator
      AtNode *mergeOp = AddArnoldNode("merge", "input_merge_op");
      AtArray* opArray = AiArrayAllocate(inputOps.size(), 1, AI_TYPE_NODE);
      for (unsigned int i = 0; i < inputOps.size(); ++i)
         AiArraySetPtr(opArray, i, (void*)inputOps[i]);
      
      AiNodeSetArray(mergeOp, "inputs", opArray);
      AiNodeSetPtr(procedural, "operator", mergeOp);
   }
   return procedural;
}


void CProceduralTranslator::NodeChanged(MObject& node, MPlug& plug)
{
   m_attrChanged = true; // this flag tells me that I've been through a NodeChanged call

   if (!IsTransformPlug(plug))
      SetUpdateMode(AI_RECREATE_NODE);
   
   CShapeTranslator::NodeChanged(node, plug);
}
   
void CProceduralTranslator::RequestUpdate()
{  
   // if no attribute has changed, it means that RequestUpdate is invoked explicitely
   // and in that case we want to fully regenerate the node (#3240)
   if (!m_attrChanged)
      SetUpdateMode(AI_RECREATE_NODE);

   CShapeTranslator::RequestUpdate();
   m_attrChanged = false;
   // this should propagate a request update on all other procedurals, standins, referencing me
}

MPlug CProceduralTranslator::FindProceduralPlug(const char *name)
{
   MString attrName(name);
   if (attrName.length() == 0)
      return MPlug();

   MPlug plug = m_DagNode.findPlug(MString(name), true);
   if (plug.isNull())
   {
      MString prefix = attrName.substringW(0, 0);
      attrName = MString ("ai") + prefix.toUpperCase() + attrName.substringW(1, attrName.length() - 1);

      plug = m_DagNode.findPlug(attrName, true);
   }

   MStatus overstat;
   MString attrLongName = plug.partialName(false, true, true, false, true, true, &overstat);
   MPlug overridePlug = m_impl->FindMayaOverridePlug(attrLongName, &overstat);
   if ((MStatus::kSuccess == overstat) && !overridePlug.isNull())
   {
      plug = overridePlug;
   }

   return plug;
}