#include "Lights.h"
#include "nodes/ShaderUtils.h"
#include "utils/AttrHelper.h"

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

#include <vector>
#include <cstring>

void CLightTranslator::ExportLightFilters(AtNode* light, const MObjectArray &filterNodes)
{
   std::vector<AtNode*> filters;

   for (unsigned int i=0; i<filterNodes.length(); ++i)
   {
      AtNode* filter = m_scene->ExportShader(filterNodes[i]);
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

void CLightTranslator::Update(AtNode* light, bool mayaAttrs)
{
   MPlug plug;
   AtMatrix matrix;

   // FIXME: processing parameters means setting up links if the plug has an incoming connection
   // this doesn't always make sense in the context of a light.
   plug = m_fnNode.findPlug("color");
   ProcessParameter(light, plug, "color", AI_TYPE_RGB);

   AiNodeSetStr(light, "name", m_fnNode.partialPathName().asChar());
   AiNodeSetFlt(light, "intensity", m_fnNode.findPlug("intensity").asFloat());

   if (mayaAttrs)
   {
      AiNodeSetBool(light, "cast_shadows", m_fnNode.findPlug("useRayTraceShadows").asBool());
      AiNodeSetInt(light, "samples", m_fnNode.findPlug("shadowRays").asInt());

      AiNodeSetBool(light, "affect_diffuse", m_fnNode.findPlug("emitDiffuse").asBool());
      AiNodeSetBool(light, "affect_specular", m_fnNode.findPlug("emitSpecular").asBool());
   }
   else
   {
      AiNodeSetBool(light, "cast_shadows", m_fnNode.findPlug("cast_shadows").asBool());
      AiNodeSetInt(light, "samples", m_fnNode.findPlug("samples").asInt());

      AiNodeSetBool(light, "affect_diffuse", m_fnNode.findPlug("affect_diffuse").asBool());
      AiNodeSetBool(light, "affect_specular", m_fnNode.findPlug("affect_specular").asBool());
   }

   ExportDynamicIntParameter(light, "sss_samples");
   ExportDynamicIntParameter(light, "bounces");
   ExportDynamicFloatParameter(light, "bounce_factor");

   MStatus status;
   MPlug pFilters = m_fnNode.findPlug("light_filters", &status);
   if(status == MS::kSuccess)
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
      AtArray* matrices = AiArrayAllocate(1, m_scene->GetNumMotionSteps(), AI_TYPE_MATRIX);
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
   CExtensionAttrHelper helper(nodeClassName, "point_light");
   // common attributes
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   helper.MakeInput("sss_samples");
}

void CLightTranslator::Delete()
{
   // Arnold doesn't allow use to delete nodes, so this
   // is as close as we'll get for now.
   AiNodeSetFlt(m_atNode, "intensity", 0.0f );
}

// AmbientLight
//
AtNode* CAmbientLightTranslator::Export()
{
   AtNode* light = AiNode("ambient_light");
   Update(light);
   return light;
}

void CAmbientLightTranslator::Update(AtNode* light)
{
   CLightTranslator::Update(light);
}

// DirectionalLight
//
AtNode* CDirectionalLightTranslator::Export()
{
   AtNode* light = AiNode("distant_light");
   Update(light);
   return light;
}

void CDirectionalLightTranslator::Update(AtNode* light)
{
   CLightTranslator::Update(light);
   MFnDirectionalLight fnLight(m_dagPath);
   AiNodeSetFlt(light, "angle", fnLight.shadowAngle());
}

// PointLight
//
AtNode* CPointLightTranslator::Export()
{
   AtNode* light = AiNode("point_light");
   AiNodeSetStr(light, "name", m_fnNode.partialPathName().asChar());
   Update(light);
   return light;
}

void CPointLightTranslator::Update(AtNode* light)
{
   CLightTranslator::Update(light);

   MPlug plug;
   MFnPointLight fnLight(m_dagPath);

   AiNodeSetFlt(light, "radius", fnLight.shadowRadius());

   ExportDynamicBooleanParameter(light, "affect_volumetrics");
   ExportDynamicBooleanParameter(light, "cast_volumetric_shadows");
}

void CPointLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "point_light");
   // common attributes
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   helper.MakeInput("sss_samples");
   // point light attributes
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}

// SpotLight
//
AtNode* CSpotLightTranslator::Export()
{
   AtNode* light = AiNode("spot_light");
   Update(light);
   return light;
}

void CSpotLightTranslator::Update(AtNode* light)
{
   MPlug plug;
   MFnSpotLight fnLight(m_dagPath);

   CLightTranslator::Update(light);

   AiNodeSetFlt(light, "radius", fnLight.shadowRadius());
   AiNodeSetFlt(light, "cone_angle", static_cast<float>((fnLight.coneAngle() + fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, "penumbra_angle", static_cast<float>(fabs(fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, "cosine_power", static_cast<float>(fnLight.dropOff()));

   ExportDynamicBooleanParameter(light, "affect_volumetrics");
   ExportDynamicBooleanParameter(light, "cast_volumetric_shadows");

   EXPORT_DYN_PARAM_FLOAT(light, "aspect_ratio", fnLight);
   EXPORT_DYN_PARAM_FLOAT(light, "lens_radius", fnLight);
}

void CSpotLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "spot_light");
   // common attributes
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   helper.MakeInput("sss_samples");
   // spot light attributes
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("aspect_ratio");
   helper.MakeInput("lens_radius");
}

// AreaLight
//
AtNode* CAreaLightTranslator::Export()
{
   AtNode* light = AiNode("quad_light");
   AiNodeSetStr(light, "name", m_fnNode.partialPathName().asChar());
   Update(light);
   return light;
}

void CAreaLightTranslator::Update(AtNode* light)
{
   CLightTranslator::Update(light);

   AtPoint vertices[4];

   AiV3Create(vertices[0], 1, 1, 0);
   AiV3Create(vertices[1], 1, -1, 0);
   AiV3Create(vertices[2], -1, -1, 0);
   AiV3Create(vertices[3], -1, 1, 0);

   AiNodeSetArray(light, "vertices", AiArrayConvert(4, 1, AI_TYPE_POINT, vertices, true));

   ExportDynamicIntParameter(light, "resolution");
   ExportDynamicBooleanParameter(light, "affect_volumetrics");
   ExportDynamicBooleanParameter(light, "cast_volumetric_shadows");
   ExportDynamicBooleanParameter(light, "solid_angle");
}

void CAreaLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper(nodeClassName, "quad_light");

   // common attributes
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   helper.MakeInput("sss_samples");
   // spot light attributes
   helper.MakeInput("resolution");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("solid_angle");
}


// SkyDomeLight
//
AtNode* CSkyDomeLightTranslator::Export()
{
   AtNode* light = AiNode("skydome_light");
   Update(light);
   return light;
}


void CSkyDomeLightTranslator::Update(AtNode* light)
{
   // Don't use maya-style attrs
   CLightTranslator::Update(light, false);

   AiNodeSetInt(light, "resolution", m_fnNode.findPlug("resolution").asInt());
   AiNodeSetFlt(light, "exposure", m_fnNode.findPlug("exposure").asFloat());
   AiNodeSetInt(light, "format", m_fnNode.findPlug("format").asInt());
   AiNodeSetFlt(light, "shadow_density", m_fnNode.findPlug("shadow_density").asFloat());
   AiNodeSetRGB(light, "shadow_color", m_fnNode.findPlug("shadow_colorR").asFloat(), m_fnNode.findPlug("shadow_colorG").asFloat(), m_fnNode.findPlug("shadow_colorB").asFloat());
   AiNodeSetBool(light, "normalize", m_fnNode.findPlug("normalize").asBool());
   AiNodeSetBool(light, "mis", m_fnNode.findPlug("mis").asBool());
}
