#include "ShapeTranslator.h"

#include <maya/MPlugArray.h>
#include <maya/MDagPathArray.h>

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
   unsigned int nbSceneLights = session->MayaLightCount();

   const MDagPath MayaShapePath = GetMayaDagPath();
   const MString MayaShapeName = MayaShapePath.partialPathName();

   // NOTE no support for component linking and ignored lights
   if (lightLinkMode == MTOA_LIGHTLINK_MAYA)
   {
      std::vector<AtNode*> lights;
      MDagPathArray mayaLights;

      AiMsgDebug("[mtoa] Querying light linking on %s", MayaShapeName.asChar());
      status = mayaLightLinks->getLinkedLights(MayaShapePath, MObject::kNullObj, mayaLights);
      CHECK_MSTATUS(status);

      unsigned int nbLinkedLights = mayaLights.length();

      // No need for a list if linked to all lights in scene
      if (nbLinkedLights < nbSceneLights)
      {
         AiMsgDebug("[mtoa] Exporting light linking on %s (%i lights out of %i total scene lights)",
                     MayaShapeName.asChar(), nbLinkedLights, nbSceneLights );
         for (unsigned int i=0; i<nbLinkedLights; ++i)
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
      else
      {
         AiMsgDebug("[mtoa] Skipping light linking on %s (linked to all %i total scene lights)",
                     MayaShapeName.asChar(), nbSceneLights );
         AiNodeSetBool(shape, "use_light_group", false);
         AiNodeSetBool(shape, "use_shadow_group", false);
      }
   }

   // Specific shadow linking
   if (shadowLinkMode == MTOA_SHADOWLINK_MAYA)
   {
      std::vector<AtNode*> shadows;
      MDagPathArray mayaShadows;

      AiMsgDebug("[mtoa] Querying shadow linking on %s", MayaShapeName.asChar());
      status = mayaLightLinks->getShadowLinkedLights(MayaShapePath, MObject::kNullObj, mayaShadows);
      CHECK_MSTATUS(status);

      unsigned int nbLinkedShadows = mayaShadows.length();

      // No need for a list if linked to all lights in scene
      if (nbLinkedShadows < nbSceneLights)
      {
         AiMsgDebug("[mtoa] Exporting shadow linking on %s (%i lights out of %i total scene lights)",
                     MayaShapeName.asChar(), nbLinkedShadows, nbSceneLights );
         for (unsigned int i=0; i<nbLinkedShadows; ++i)
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
      else
      {
         AiMsgDebug("[mtoa] Skipping shadow linking on %s (linked to all %i total scene lights)",
                  MayaShapeName.asChar(), nbSceneLights );
         AiNodeSetBool(shape, "use_shadow_group", false);
      }
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
