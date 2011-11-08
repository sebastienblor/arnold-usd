#include "ShapeTranslator.h"

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
   MObjectArray mayaLights;

   if (FindMayaObjectPlug("aiUseLightGroup").asBool())
   {
      AiNodeSetBool(shape, "use_light_group", FindMayaObjectPlug("aiUseLightGroup").asBool());
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
                mayaLights.append(pSources[0].node());
             }
         }
      }
   }

   for (unsigned int i=0; i<mayaLights.length(); ++i)
   {
      MDagPath lightPath;
      MDagPath::getAPathTo(mayaLights[i], lightPath);
      if (lightPath.isValid())
      {
         AtNode* light = ExportDagPath(lightPath);
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

