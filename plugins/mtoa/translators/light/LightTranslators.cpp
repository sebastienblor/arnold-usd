#include "LightTranslators.h"

#include <maya/MFnAmbientLight.h>
#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>

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
