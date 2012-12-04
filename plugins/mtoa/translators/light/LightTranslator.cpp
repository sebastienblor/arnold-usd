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

void CLightTranslator::ExportLightFilters(AtNode* light, const MPlugArray &filterNodes)
{
   std::vector<AtNode*> filters;

   for (unsigned int i=0; i<filterNodes.length(); ++i)
   {
      AtNode* filter = ExportNode(filterNodes[i]);
      filters.push_back(filter);
   }

   if (filters.size() > 0)
   {
      AiNodeSetArray(light, "filters", AiArrayConvert((int)filters.size(), 1, AI_TYPE_NODE, &filters[0]));
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

   // Early out, light isn't visible so no point exporting anything else.
   if (false == m_session->IsRenderablePath(m_dagPath))
   {
      // Light can't be hidden.
      AiNodeSetFlt(GetArnoldRootNode(), "intensity",  0.0f);
      return;
   }
   
   // FIXME: processing parameters means setting up links if the plug has an incoming connection
   // this doesn't always make sense in the context of a light.
   ProcessParameter(light, "color", AI_TYPE_RGB);
   ProcessParameter(light, "intensity", AI_TYPE_FLOAT);
   ProcessParameter(light, "shadow_color", AI_TYPE_RGB);

   AiNodeSetBool(light, "affect_diffuse",  FindMayaPlug("emitDiffuse").asBool());
   AiNodeSetBool(light, "affect_specular", FindMayaPlug("emitSpecular").asBool());
   AiNodeSetBool(light, "cast_shadows",    FindMayaPlug("aiCastShadows").asBool());
   AiNodeSetFlt(light,  "shadow_density",  FindMayaObjectPlug("aiShadowDensity").asFloat());
   AiNodeSetFlt(light,  "exposure",        FindMayaPlug("aiExposure").asFloat());
   AiNodeSetInt(light,  "samples",         FindMayaPlug("aiSamples").asInt());
   AiNodeSetBool(light, "normalize",       FindMayaPlug("aiNormalize").asBool());
   AiNodeSetInt(light,  "bounces",         FindMayaPlug("aiBounces").asInt());
   AiNodeSetFlt(light,  "bounce_factor",   FindMayaPlug("aiBounceFactor").asFloat());
 
   MStatus status;
   MPlug pFilters = FindMayaPlug("aiFilters");
   if (!pFilters.isNull())
   {
      MPlugArray filters;
      MPlugArray pSources;

      for (unsigned int i=0; i<pFilters.numElements(); ++i)
      {
         MPlug pFilter = pFilters[i];
         pFilter.connectedTo(pSources, true, false);
         if (pSources.length() == 1)
         {
            filters.append(pSources[0]);
         }
      }

      ExportLightFilters(light, filters);
   }

   GetMatrix(matrix);

   if (RequiresMotionData())
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

void CLightTranslator::ExportMotion(AtNode* light, unsigned int step)
{
   AtMatrix matrix;
   GetMatrix(matrix);

   AtArray* matrices = AiNodeGetArray(light, "matrix");
   AiArraySetMtx(matrices, step, matrix);
}

void CLightTranslator::MakeCommonAttributes(CBaseAttrHelper& helper)
{
   helper.MakeInput("cast_shadows");
   helper.MakeInput("shadow_density"); 
   helper.MakeInput("exposure");
   helper.MakeInput("samples");
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   helper.MakeInput("filters");
   
   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "aiUseColorTemperature";
   data.shortName = "ai_use_color_temperature";
   helper.MakeInputBoolean(data);
   
   data.defaultValue.FLT = 6500;
   data.name = "aiColorTemperature";
   data.shortName = "ai_color_temperature";
   helper.MakeInputFloat(data);
}

void CLightTranslator::Delete()
{
   // AiNodeDestroy(GetArnoldRootNode());
   // Arnold doesn't allow us to create nodes in between to calls to AiRender
   // for the moment. For IPR we still need to rely on setting the intensity to 0.0.
   AiNodeSetFlt(GetArnoldRootNode(), "intensity",  0.0f);
}

AtRGB CLightTranslator::ConvertKelvinToRGB(float kelvin)
{
   return AI_RGB_BLACK;
}
