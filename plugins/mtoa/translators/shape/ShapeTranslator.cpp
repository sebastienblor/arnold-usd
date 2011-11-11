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
   if (!plug.isNull())
   {
      int m_enum = plug.asInt();
      switch (m_enum)
      {
      case 0:
         AiNodeSetStr(node, "sss_sample_distribution", "blue_noise");
         break;
      case 1:
         AiNodeSetStr(node, "sss_sample_distribution", "blue_noise_Pref");
         break;
      case 2:
         AiNodeSetStr(node, "sss_sample_distribution", "triangle_midpoint");
         break;

      case 3:
         AiNodeSetStr(node, "sss_sample_distribution", "polygon_midpoint");
         break;
      }
   }

   plug = FindMayaObjectPlug("aiSssSampleSpacing");
   if (!plug.isNull()) AiNodeSetFlt(node, "sss_sample_spacing", plug.asFloat());

}


void CShapeTranslator::ExportLightLinking(AtNode* shape)
{
   std::vector<AtNode*> lights;
   MDagPathArray mayaLights;
   MStatus status;


   if (FindMayaObjectPlug("aiUseLightGroup").asBool())
   {
      AiNodeSetBool(shape, "use_light_group", true);
      MPlug pLights = FindMayaObjectPlug("aiLightGroup");
      if (!pLights.isNull())
      {
         MPlugArray pSources;

         for (unsigned int i=0; i<pLights.numElements(); ++i)
         {
             MPlug pLight = pLights[i];
             pLight.connectedTo(pSources, true, false);
             if (pSources.length() == 1)
             {
                MDagPath lightPath;
                MDagPath::getAPathTo(pSources[0].node(), lightPath);
                // TODO : handle multiple shapes cases
                lightPath.extendToShape();
                mayaLights.append(lightPath);
             }
         }
      }
   }
   else
   {
      AiNodeSetBool(shape, "use_light_group", true);

      // Get linked lights (use transform to support linking of instance lights)
      MString linkcmd = MString("maya.cmds.lightlink(query=True, object='")+GetMayaNodeName()
                        +MString("', sets=False, hierarchy=False, transforms=True, shapes=False)");
      MStringArray lightLinks;
      status = MGlobal::executePythonCommand(MString("import maya.cmds;")+linkcmd,
                                             lightLinks,
                                             true, false);
      CHECK_MSTATUS(status);

      MSelectionList list;
      for (unsigned int i=0; i<lightLinks.length(); ++i)
      {
         MGlobal::displayInfo(lightLinks[i]);
         list.add(lightLinks[i]);
      }
      for (unsigned int i=0; i<list.length(); ++i)
      {
         MDagPath lightPath;
         list.getDagPath(i, lightPath);
         // TODO : handle multiple shapes cases
         lightPath.extendToShape();
         mayaLights.append(lightPath);
      }
   }

   for (unsigned int i=0; i<mayaLights.length(); ++i)
   {
      MDagPath lightPath = mayaLights[i];
      if (lightPath.isValid())
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
      AiNodeSetArray(shape, "light_group", AiArrayConvert((AtInt)lights.size(), 1, AI_TYPE_NODE, &lights[0], TRUE));
   }
   else
   {
      AiNodeSetArray(shape, "light_group", NULL);
   }
}

// create attributes common to arnold shape nodes
//
void CShapeTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   helper.MakeInput("sss_sample_distribution");
   helper.MakeInput("sss_sample_spacing");

   helper.MakeInput("self_shadows");
   helper.MakeInput("opaque");

   helper.MakeInput("use_light_group");
   helper.MakeInput("light_group");

   MakeArnoldVisibilityFlags(helper);
}

