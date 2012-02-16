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

// called for shaders connected directly to shapes
AtNode* CShapeTranslator::ExportRootShader(const MPlug& plug)
{
   AtNode* beauty = ExportNode(plug);
   return AddAOVDefaults(beauty);
}

AtNode* CShapeTranslator::AddAOVDefaults(AtNode *beauty)
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
            AtNode* write = NULL;
            int type = fnNode.findPlug("type").asInt();
            switch(type)
            {
               case AI_TYPE_RGB:
               case AI_TYPE_RGBA:
               {
                  write = AddArnoldNode("writeColor", aov.GetName().asChar());
                  break;
               }
               case AI_TYPE_FLOAT:
               {
                  write = AddArnoldNode("writeFloat", aov.GetName().asChar());
                  break;
               }
//               case AI_TYPE_INT:
//               {
//                  write = AddArnoldNode("writeFloat", aov.GetName().asChar());
//                  break;
//               }
            }
            if (write != NULL)
            {
               AiNodeLink(beauty, "beauty", write);
               AiNodeSetStr(write, "aov_name", aov.GetName().asChar());
               // process connections
               // use m_session->ExportNode to avoid processing aovs for this node
               AtNode* linkedNode = m_session->ExportNode(connections[0]);
               if (linkedNode != NULL)
               {
                  AiNodeLink(linkedNode, "input", write);
                  beauty = write;
               }
               else
                  AiMsgWarning("[mtoa] [aov] invalid input on default value for \"%s\"", aov.GetName().asChar());
            }
         }
         //ProcessParameter(shader, plug, "input", AI_TYPE_RGB);
      }
   }
   return beauty;
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
