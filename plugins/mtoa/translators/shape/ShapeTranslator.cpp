#include "ShapeTranslator.h"

#include <maya/MPlugArray.h>
#include <maya/MDagPathArray.h>

#include <algorithm>

#include <maya/MPlugArray.h>

// computes and sets the visibility mask as well as other shape attributes related to ray visibility
// (self_shadows, opaque)
void CShapeTranslator::ProcessRenderFlags(AtNode* node)
{
   AiNodeSetInt(node, "visibility", ComputeVisibility());

   MPlug plug;
   plug = FindMayaObjectPlug("aiSelfShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "self_shadows", plug.asBool());

   plug = FindMayaObjectPlug("aiOpaque");
   if (!plug.isNull()) AiNodeSetBool(node, "opaque", plug.asBool());

   plug = FindMayaObjectPlug("receiveShadows");
   if (!plug.isNull()) AiNodeSetBool(node, "receive_shadows", plug.asBool());

   // Sub-Surface Scattering
   plug = FindMayaObjectPlug("aiSssSampleDistribution");
   if (!plug.isNull()) AiNodeSetInt(node, "sss_sample_distribution", plug.asInt());

   plug = FindMayaObjectPlug("aiSssSampleSpacing");
   if (!plug.isNull()) AiNodeSetFlt(node, "sss_sample_spacing", plug.asFloat());

}


void CShapeTranslator::ExportLightLinking(AtNode* shape)
{
   MStatus status;

   CArnoldSession* session = GetSession();
   ArnoldLightLinkMode lightLinkMode = session->GetLightLinkMode();
   ArnoldShadowLinkMode shadowLinkMode = session->GetShadowLinkMode();
   MLightLinks* mayaLightLinks = session->MayaLightLinks();

   const MDagPath MayaShapePath = GetMayaDagPath();
   const MString MayaShapeName = MayaShapePath.partialPathName();

   std::vector<AtNode*> lights;
   MDagPathArray mayaLights;
   // NOTE no support for component linking and ignored lights
   if (lightLinkMode == MTOA_LIGHTLINK_MAYA)
   {
      AiMsgDebug("[mtoa] Exporting light linking on %s", MayaShapeName.asChar());
      status = mayaLightLinks->getLinkedLights(MayaShapePath, MObject::kNullObj, mayaLights);
      CHECK_MSTATUS(status);

      for (unsigned int i=0; i<mayaLights.length(); ++i)
      {
         MDagPath lightPath = mayaLights[i];
         // TODO : handle multiple shapes cases
         lightPath.extendToShape();
         if (lightPath.isValid() && (session->FilteredStatus(lightPath) == MTOA_EXPORT_ACCEPTED))
         {
            // TODO: shoud this respect current selection for render / export selection
            // and or export filters? In that case use CArnoldSession::ExportSelection and
            // CArnoldSession::FileredStatus instead
            AtNode* light = ExportDagPath(lightPath);
            // TODO : might be safer to check it's indeed a light that has been exported
            if (light != NULL)
            {
               lights.push_back(light);
            }
         }
      }

      if (lights.size() > 0)
      {
         AiNodeSetArray(shape, "light_group", AiArrayConvert((int)lights.size(), 1, AI_TYPE_NODE, &lights[0]));
      }
      else
      {
         AiNodeSetArray(shape, "light_group", AiArray(0, 1, AI_TYPE_NODE));
      }

      AiNodeSetBool(shape, "use_light_group", true);

      // Shadow linking obeys light linking
      if (shadowLinkMode == MTOA_SHADOWLINK_LIGHT)
      {
         if (lights.size() > 0)
         {
            AiNodeSetArray(shape, "shadow_group", AiArrayConvert((int)lights.size(), 1, AI_TYPE_NODE, &lights[0]));
         }
         else
         {
            AiNodeSetArray(shape, "shadow_group", AiArray(0, 1, AI_TYPE_NODE));
         }

         AiNodeSetBool(shape, "use_shadow_group", true);
      }
      
   }
   else if (lightLinkMode == MTOA_LIGHTLINK_NONE)
   {
      AiNodeSetBool(shape, "use_light_group", false);
      
      if (shadowLinkMode == MTOA_SHADOWLINK_LIGHT)
      {
         AiNodeSetBool(shape, "use_shadow_group", false);
      }
   }

   // Specific shadow linking
   std::vector<AtNode*> shadows;
   MDagPathArray mayaShadows;
   if (shadowLinkMode == MTOA_SHADOWLINK_MAYA)
   {
      AiMsgDebug("[mtoa] Exporting shadow linking on %s", MayaShapeName.asChar());
      status = mayaLightLinks->getShadowLinkedLights(MayaShapePath, MObject::kNullObj, mayaShadows);
      CHECK_MSTATUS(status);

      for (unsigned int i=0; i<mayaShadows.length(); ++i)
      {
         MDagPath shadowPath = mayaShadows[i];
         // TODO : handle multiple shapes cases
         shadowPath.extendToShape();
         if (shadowPath.isValid() && (session->FilteredStatus(shadowPath) == MTOA_EXPORT_ACCEPTED))
         {
            // TODO: shoud this respect current selection for render / export selection
            // and or export filters? In that case use CArnoldSession::ExportSelection and
            // CArnoldSession::FileredStatus instead
            AtNode* shadow = ExportDagPath(shadowPath);
            // TODO : might be safer to check it's indeed a light that has been exported
            if (shadow != NULL)
            {
               shadows.push_back(shadow);
            }
         }
      }

      if (shadows.size() > 0)
      {
         AiNodeSetArray(shape, "shadow_group", AiArrayConvert((int)shadows.size(), 1, AI_TYPE_NODE, &shadows[0]));
      }
      else
      {
         AiNodeSetArray(shape, "shadow_group", AiArray(0, 1, AI_TYPE_NODE));
      }

      AiNodeSetBool(shape, "use_shadow_group", true);
   }
   else if (shadowLinkMode == MTOA_SHADOWLINK_NONE)
   {
      AiNodeSetBool(shape, "use_shadow_group", false);
   }
}

// create attributes common to arnold shape nodes
//
void CShapeTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   MString nodeType = helper.GetMayaNodeTypeName();
   AiMsgDebug("[mtoa] Creating common Arnold shape attributes on Maya \"%s\" nodes", nodeType.asChar());

   helper.MakeInput("sss_sample_distribution");
   helper.MakeInput("sss_sample_spacing");

   helper.MakeInput("self_shadows");
   helper.MakeInput("opaque");

   MakeArnoldVisibilityFlags(helper);
}

AtNode* CShapeTranslator::CreateShadingGroupShader(AtNode *rootShader, std::vector<AtNode*> &aovShaders)
{
   int numCustom = aovShaders.size();
   // Do we want to always create a shadingEngine shader for consistency?
   if (numCustom)
   {
      // insert shading group shader to evaluate extra AOV inputs
      AtNode* shadingEngine = AiNode("MayaShadingEngine");
      AiNodeSetArray(shadingEngine, "aov_inputs", AiArrayConvert(numCustom, 1, AI_TYPE_NODE, &aovShaders[0]));
      AiNodeSetStr(shadingEngine, "name", (GetMayaNodeName() + "@SG").asChar());
      AiNodeLink(rootShader, "beauty", shadingEngine);
      return shadingEngine;
   }
   return rootShader;
}

// called for shaders connected directly to shapes
AtNode* CShapeTranslator::ExportRootShader(const MPlug& plug)
{
   return ExportRootShader(ExportNode(plug));
}

// called for root shaders that have already been created
AtNode* CShapeTranslator::ExportRootShader(AtNode *rootShader)
{
   std::vector<AtNode*> aovShaders;
   AddAOVDefaults(aovShaders);
   return CreateShadingGroupShader(rootShader, aovShaders);
}

/// Find and export the surfaceShader for the passed shadingGroup and add the AOV defaults.
AtNode* CShapeTranslator::ExportRootShader(const MObject& shadingGroup)
{
   AtNode *rootShader = NULL;
   std::vector<AtNode*> aovShaders;

   MPlugArray        connections;
   MFnDependencyNode fnDGNode(shadingGroup);
   MPlug shaderPlug = fnDGNode.findPlug("surfaceShader");
   shaderPlug.connectedTo(connections, true, false);
   if (connections.length() > 0)
   {
      // export the root shading network, this fills m_shaders
      rootShader = ExportNode(connections[0]);

      // loop through and export custom AOV networks
      MPlug arrayPlug = fnDGNode.findPlug("aiCustomAOVs");
      for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
      {
         MPlug msgPlug = arrayPlug[i].child(1);
         msgPlug.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            // by using m_session->ExportNode we avoid tracking aovs.
            AtNode* writeNode = m_session->ExportNode(connections[0]);
            // since we know this maya node is connected to aiCustomAOVs it will have a write node
            // inserted after it by CShaderTranslator::ProcessAOVOutput (assuming the node is translated by
            // CShaderTranslator)
            // TODO: check shader type: rootShader should always be an aov write node, unless it is a conversion node

            // if the node is not yet in the shading network for this shape, then branch it in.
            // m_shaders contains all the arnold nodes in a shape's shading network, as tracked by ExportRootShader.
            if (!m_shaders->count(writeNode))
            {
               aovShaders.push_back(writeNode);
            }
         }
      }
   }
   else
      AiMsgWarning("[mtoa] [translator %s] ShadingGroup %s has no surfaceShader input",
            GetTranslatorName().asChar(), fnDGNode.name().asChar());

   AddAOVDefaults(aovShaders); // modifies aovShaders list
   return CreateShadingGroupShader(rootShader, aovShaders);
}

/// Adds new AOV write nodes to aovShaders for any AOVs with defaults not present in this shading network
void CShapeTranslator::AddAOVDefaults(std::vector<AtNode*> &aovShaders)
{
   AOVSet active = m_session->GetActiveAOVs();

   // get the active AOVs not in the exported list
   AOVSet unused;
   std::set_difference(active.begin(), active.end(),
                       m_upstreamAOVs.begin(), m_upstreamAOVs.end(),
                       std::inserter(unused, unused.begin()));

   MFnDependencyNode fnNode;
   for (AOVSet::iterator it=unused.begin(); it!=unused.end(); ++it)
   {
      CAOV aov = *it;
      MObject oAOV = aov.GetNode();
      if (oAOV != MObject::kNullObj)
      {
         fnNode.setObject(oAOV);
         MPlug plug = fnNode.findPlug("defaultValue");
         MPlugArray connections;
         plug.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            int outType = fnNode.findPlug("type").asInt();
            MString nodeType = "";
            switch (outType)
            {
            case AI_TYPE_FLOAT:
               nodeType = "writeFloatInline";
               break;
            case AI_TYPE_RGB:
            case AI_TYPE_RGBA:
               nodeType = "writeColorInline";
               break;
            case AI_TYPE_VECTOR:
               nodeType = "writeVectorInline";
               break;
            case AI_TYPE_POINT:
               nodeType = "writePointInline";
               break;
            case AI_TYPE_POINT2:
               nodeType = "writePoint2Inline";
               break;
            default:
               {
                  AiMsgWarning("[mtoa] Not of a supported AOV data type: %s",
                               GetMayaNodeName().asChar());
                  continue;
               }
            }
            // process connections
            // use m_session->ExportNode to avoid processing aovs for this node
            AtNode* linkedNode = m_session->ExportNode(connections[0]);
            if (linkedNode != NULL)
            {
               AtNode* writeNode = AiNode(nodeType.asChar());;
               AiNodeSetStr(writeNode, "aov_name", aov.GetName().asChar());
               AiNodeLink(linkedNode, "input", writeNode);
               aovShaders.push_back(writeNode);
            }
            else
               AiMsgWarning("[mtoa] [aov] invalid input on default value for \"%s\"", aov.GetName().asChar());
         }
         //ProcessParameter(shader, plug, "input", AI_TYPE_RGB);
      }
   }
}
MObject CShapeTranslator::GetNodeShadingGroup(MObject dagNode, int instanceNum)
{
   MPlugArray        connections;
   MFnDependencyNode fnDGNode(dagNode);

   MPlug plug(dagNode, fnDGNode.attribute("instObjGroups"));

   plug.elementByLogicalIndex(instanceNum).connectedTo(connections, false, true);

   for (unsigned int k=0; k<connections.length(); ++k)
   {
      MObject shadingGroup(connections[k].node());
      if (shadingGroup.apiType() == MFn::kShadingEngine)
      {
         return shadingGroup;
      }
   }
   return MObject::kNullObj;
}
