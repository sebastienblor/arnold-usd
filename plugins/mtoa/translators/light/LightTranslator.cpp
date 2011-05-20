#include "LightTranslator.h"
#include "nodes/ShaderUtils.h"
#include "attributes/AttrHelper.h"

#include <ai_constants.h>
#include <ai_msg.h>
#include <ai_nodes.h>

#include <maya/MColor.h>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>
#include <maya/MPlugArray.h>
#include <maya/MFnNumericAttribute.h>

#include <vector>
#include <string>

void CLightTranslator::ExportLightFilters(AtNode* light, const MObjectArray &filterNodes)
{
   std::vector<AtNode*> filters;

   for (unsigned int i=0; i<filterNodes.length(); ++i)
   {
      AtNode* filter = ExportShader(filterNodes[i]);
      filters.push_back(filter);
   }

   if (filters.size() > 0)
   {
      AiNodeSetArray(light, "filters", AiArrayConvert((AtInt)filters.size(), 1, AI_TYPE_NODE, &filters[0], TRUE));
   }
   else
   {
      // TODO: Change this to: AiNodeSetArray(light, "filters", NULL);
      // when the arnold bug causing a crash (reported on 16-Jan-2011) is fixed.
      AiNodeSetArray(light, "filters", AiArrayAllocate(0,0, AI_TYPE_NODE));
   }
}

void CLightTranslator::Export(AtNode* light)
{
   MPlug plug;
   AtMatrix matrix;

   // FIXME: processing parameters means setting up links if the plug has an incoming connection
   // this doesn't always make sense in the context of a light.
   plug = GetFnNode().findPlug("color");
   ProcessParameter(light, plug, "color", AI_TYPE_RGB);

   // AiNodeSetFlt(light, "intensity", GetFnNode().findPlug("intensity").asFloat());
   plug = GetFnNode().findPlug("intensity");
   ProcessParameter(light, plug, "intensity", AI_TYPE_FLOAT);

   AiNodeSetBool(light, "affect_diffuse", GetFnNode().findPlug("emitDiffuse").asBool());
   AiNodeSetBool(light, "affect_specular", GetFnNode().findPlug("emitSpecular").asBool());

   AiNodeSetBool(light, "cast_shadows", GetFnNode().findPlug("aiCastShadows").asBool());
   AiNodeSetFlt(light, "exposure", GetFnNode().findPlug("aiExposure").asFloat());
   AiNodeSetInt(light, "samples", GetFnNode().findPlug("aiSamples").asInt());
   AiNodeSetBool(light, "mis", GetFnNode().findPlug("aiMis").asBool());
   AiNodeSetBool(light, "normalize", GetFnNode().findPlug("aiNormalize").asBool());
   if (GetFnNode().findPlug("aiOverrideSssSamples").asBool())
   {
      AiNodeSetInt(light, "sss_samples", GetFnNode().findPlug("aiSssSamples").asInt());
   }
   AiNodeSetInt(light, "bounces", GetFnNode().findPlug("aiBounces").asInt());
   AiNodeSetFlt(light, "bounce_factor", GetFnNode().findPlug("aiBounceFactor").asFloat());

   MStatus status;
   MPlug pFilters = GetFnNode().findPlug("aiLightFilters", &status);
   if (status == MS::kSuccess)
   {
      MObjectArray filters;
      MPlugArray pSources;

      for (unsigned int i=0; i<pFilters.numElements(); ++i)
      {
         MPlug pFilter = pFilters[i];
         pFilter.connectedTo(pSources, true, false);
         if (pSources.length() == 1)
         {
            filters.append(pSources[0].node());
         }
      }

      ExportLightFilters(light, filters);
   }

   GetMatrix(matrix);

   if (m_motion)
   {
      AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
      AiArraySetMtx(matrices, 0, matrix);
      AiNodeSetArray(light, "matrix", matrices);
   }
   else
   {
      AiNodeSetMatrix(light, "matrix", matrix);
   }
}

void CLightTranslator::ExportMotion(AtNode* light, AtUInt step)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(light, "matrix");
   AiArraySetMtx(matrices, step, matrix);
}

void CLightTranslator::NodeInitializer(MString nodeClassName)
{
   // use point light as a generic light...
   // TODO : define a generic ArnoldLightNode
   CExtensionAttrHelper helper(nodeClassName, "point_light");
   // common attributes
   helper.MakeInput("cast_shadows");
   helper.MakeInput("exposure");
   helper.MakeInput("samples");
   helper.MakeInput("mis");
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "aiOverrideSssSamples";
   data.shortName = "ai_oss";
   helper.MakeInputBoolean(data);
   helper.MakeInput("sss_samples");
}

void CLightTranslator::Delete()
{
   // Arnold doesn't allow use to delete nodes, so this
   // is as close as we'll get for now.
   AiNodeSetFlt(GetArnoldRootNode(), "intensity", 0.0f);
}
