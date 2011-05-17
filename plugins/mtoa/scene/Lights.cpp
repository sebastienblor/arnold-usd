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
#include <maya/MFnNumericAttribute.h>

#include <vector>
#include <cstring>

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

void CLightTranslator::Export(AtNode* light, bool mayaAttrs)
{
   MPlug plug;
   AtMatrix matrix;

   // FIXME: processing parameters means setting up links if the plug has an incoming connection
   // this doesn't always make sense in the context of a light.
   plug = GetFnNode().findPlug("color");
   ProcessParameter(light, plug, "color", AI_TYPE_RGB);

   AiNodeSetFlt(light, "intensity", GetFnNode().findPlug("intensity").asFloat());

   if (mayaAttrs)
   {
      AiNodeSetBool(light, "affect_diffuse", GetFnNode().findPlug("emitDiffuse").asBool());
      AiNodeSetBool(light, "affect_specular", GetFnNode().findPlug("emitSpecular").asBool());
   }
   else
   {

      AiNodeSetBool(light, "affect_diffuse", GetFnNode().findPlug("affect_diffuse").asBool());
      AiNodeSetBool(light, "affect_specular", GetFnNode().findPlug("affect_specular").asBool());
   }

   AiNodeSetBool(light, "cast_shadows", GetFnNode().findPlug("cast_shadows").asBool());
   AiNodeSetFlt(light, "exposure", GetFnNode().findPlug("exposure").asFloat());
   AiNodeSetInt(light, "samples", GetFnNode().findPlug("samples").asInt());
   AiNodeSetBool(light, "mis", GetFnNode().findPlug("mis").asBool());
   AiNodeSetBool(light, "normalize", GetFnNode().findPlug("normalize").asBool());
   if (GetFnNode().findPlug("override_sss_samples").asBool())
   {
      AiNodeSetInt(light, "sss_samples", GetFnNode().findPlug("sss_samples").asInt());
   }
   AiNodeSetInt(light, "bounces", GetFnNode().findPlug("bounces").asInt());
   AiNodeSetFlt(light, "bounce_factor", GetFnNode().findPlug("bounce_factor").asFloat());

   MStatus status;
   MPlug pFilters = GetFnNode().findPlug("light_filters", &status);
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
   CExtensionAttrHelper helper(nodeClassName, "point_light");
   // common attributes
   helper.MakeInput("cast_shadows");
   helper.MakeInput("exposure");
   helper.MakeInput("samples");
   helper.MakeInput("mis");
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   helper.MakeInput("sss_samples");
}

void CLightTranslator::Delete()
{
   // Arnold doesn't allow use to delete nodes, so this
   // is as close as we'll get for now.
   AiNodeSetFlt(GetArnoldRootNode(), "intensity", 0.0f);
}

// AmbientLight
//

void CAmbientLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);
}

// DirectionalLight
//

void CDirectionalLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetFlt(light, "angle", GetFnNode().findPlug("angle").asFloat());

}

void CDirectionalLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "distant_light");
   // common attributes
   helper.MakeInput("cast_shadows");
   helper.MakeInput("exposure");
   helper.MakeInput("angle");
   helper.MakeInput("samples");
   helper.MakeInput("mis");
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "override_sss_samples";
   data.shortName = "oss";
   helper.MakeInputBoolean(data);
   helper.MakeInput("sss_samples");
   // directional light attributes
}
// PointLight
//

void CPointLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   MPlug plug;
   MFnPointLight fnLight(m_dagPath);

   AiNodeSetFlt(light, "radius", GetFnNode().findPlug("radius").asFloat());

   AiNodeSetBool(light, "affect_volumetrics", GetFnNode().findPlug("affect_volumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", GetFnNode().findPlug("cast_volumetric_shadows").asBool());
}

void CPointLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "point_light");
   // common attributes
   helper.MakeInput("cast_shadows");
   helper.MakeInput("exposure");
   helper.MakeInput("radius");
   helper.MakeInput("samples");
   helper.MakeInput("mis");
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "override_sss_samples";
   data.shortName = "oss";
   helper.MakeInputBoolean(data);
   helper.MakeInput("sss_samples");
   // point light attributes
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}

// SpotLight
//

void CSpotLightTranslator::Export(AtNode* light)
{
   MPlug plug;
   MFnSpotLight fnLight(m_dagPath);

   CLightTranslator::Export(light);

   AiNodeSetFlt(light, "cone_angle", static_cast<float>((fnLight.coneAngle() + fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, "penumbra_angle", static_cast<float>(fabs(fnLight.penumbraAngle()) * AI_RTOD));
   AiNodeSetFlt(light, "cosine_power", static_cast<float>(fnLight.dropOff()));

   AiNodeSetFlt(light, "radius", GetFnNode().findPlug("radius").asFloat());

   AiNodeSetBool(light, "affect_volumetrics", GetFnNode().findPlug("affect_volumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", GetFnNode().findPlug("cast_volumetric_shadows").asBool());

   AiNodeSetFlt(light, "aspect_ratio", GetFnNode().findPlug("aspect_ratio").asFloat());
   AiNodeSetFlt(light, "lens_radius", GetFnNode().findPlug("lens_radius").asFloat());
}

void CSpotLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "spot_light");
   // common attributes
   helper.MakeInput("cast_shadows");
   helper.MakeInput("exposure");
   helper.MakeInput("radius");
   helper.MakeInput("samples");
   helper.MakeInput("mis");
   helper.MakeInput("normalize");
   helper.MakeInput("bounce_factor");
   helper.MakeInput("bounces");
   CAttrData data;
   data.defaultValue.BOOL = false;
   data.name = "override_sss_samples";
   data.shortName = "oss";
   helper.MakeInputBoolean(data);
   helper.MakeInput("sss_samples");
   // spot light attributes
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("aspect_ratio");
   helper.MakeInput("lens_radius");
}

// AreaLight
//

void CAreaLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AtPoint vertices[4];

   AiV3Create(vertices[0], 1, 1, 0);
   AiV3Create(vertices[1], 1, -1, 0);
   AiV3Create(vertices[2], -1, -1, 0);
   AiV3Create(vertices[3], -1, 1, 0);

   AiNodeSetArray(light, "vertices", AiArrayConvert(4, 1, AI_TYPE_POINT, vertices, true));

   AiNodeSetInt(light, "resolution", GetFnNode().findPlug("resolution").asInt());
   AiNodeSetBool(light, "affect_volumetrics", GetFnNode().findPlug("affect_volumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", GetFnNode().findPlug("cast_volumetric_shadows").asBool());
}

void CAreaLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper(nodeClassName, "quad_light");
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
   data.name = "override_sss_samples";
   data.shortName = "oss";
   helper.MakeInputBoolean(data);
   helper.MakeInput("sss_samples");
   // spot light attributes
   helper.MakeInput("resolution");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}


// SkyDomeLight
//

void CSkyDomeLightTranslator::Export(AtNode* light)
{
   // Don't use maya-style attrs
   CLightTranslator::Export(light, false);

   AiNodeSetInt(light, "resolution", GetFnNode().findPlug("resolution").asInt());
   AiNodeSetFlt(light, "exposure", GetFnNode().findPlug("exposure").asFloat());
   AiNodeSetInt(light, "format", GetFnNode().findPlug("format").asInt());
   AiNodeSetFlt(light, "shadow_density", GetFnNode().findPlug("shadow_density").asFloat());
   AiNodeSetRGB(light, "shadow_color", GetFnNode().findPlug("shadow_colorR").asFloat(), GetFnNode().findPlug("shadow_colorG").asFloat(), GetFnNode().findPlug("shadow_colorB").asFloat());
   AiNodeSetBool(light, "normalize", GetFnNode().findPlug("normalize").asBool());
   AiNodeSetBool(light, "mis", GetFnNode().findPlug("mis").asBool());
}
