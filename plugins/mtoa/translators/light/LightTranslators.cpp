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

void CAmbientLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "ambient_light");
   MakeCommonAttributes(helper);
}

// DirectionalLight
//

void CDirectionalLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetFlt(light, "angle", GetFnNode().findPlug("aiAngle").asFloat());

}

void CDirectionalLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "distant_light");
   // common attributes
   MakeCommonAttributes(helper);
   // directional light attributes
   helper.MakeInput("angle");
}
// PointLight
//

void CPointLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   MPlug plug;
   MFnPointLight fnLight(m_dagPath);

   AiNodeSetFlt(light, "radius", GetFnNode().findPlug("aiRadius").asFloat());

   AiNodeSetBool(light, "affect_volumetrics", GetFnNode().findPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", GetFnNode().findPlug("aiCastVolumetricShadows").asBool());
}

void CPointLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "point_light");
   // common attributes
   MakeCommonAttributes(helper);
   // point light attributes
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("radius");
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

   AiNodeSetFlt(light, "radius", GetFnNode().findPlug("aiRadius").asFloat());

   AiNodeSetBool(light, "affect_volumetrics", GetFnNode().findPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", GetFnNode().findPlug("aiCastVolumetricShadows").asBool());

   AiNodeSetFlt(light, "aspect_ratio", GetFnNode().findPlug("aiAspectRatio").asFloat());
   AiNodeSetFlt(light, "lens_radius", GetFnNode().findPlug("aiLensRadius").asFloat());
}

void CSpotLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(nodeClassName, "spot_light");
   // common attributes
   MakeCommonAttributes(helper);
   // spot light attributes
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("aspect_ratio");
   helper.MakeInput("radius");
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

   AiNodeSetInt(light, "resolution", GetFnNode().findPlug("aiResolution").asInt());
   AiNodeSetBool(light, "affect_volumetrics", GetFnNode().findPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", GetFnNode().findPlug("aiCastVolumetricShadows").asBool());
}

void CAreaLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper(nodeClassName, "quad_light");
   // common attributes
   MakeCommonAttributes(helper);
   // spot light attributes
   helper.MakeInput("resolution");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}


// SkyDomeLight
//
void CSkyDomeLightTranslator::GetMatrix(AtMatrix& matrix)
{
   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   // Invert in Z to account for the env sphere being viewed from inside
   double scale[3] = {1.0, 1.0, -1.0};
   tm.addScale(scale, MSpace::kPreTransform);
   MMatrix m = tm.asMatrix();

   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) m[I][J];
      }
   }
}

void CSkyDomeLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetInt(light, "resolution", GetFnNode().findPlug("resolution").asInt());
   AiNodeSetInt(light, "format", GetFnNode().findPlug("format").asInt());
   AiNodeSetFlt(light, "shadow_density", GetFnNode().findPlug("shadow_density").asFloat());
   AiNodeSetRGB(light, "shadow_color", GetFnNode().findPlug("shadow_colorR").asFloat(), GetFnNode().findPlug("shadow_colorG").asFloat(), GetFnNode().findPlug("shadow_colorB").asFloat());
}

void CSkyDomeLightTranslator::NodeInitializer(MString nodeClassName)
{
   CExtensionAttrHelper helper(nodeClassName, "skydome_light");
   // Cannot be created both on Node and here
   MakeCommonAttributes(helper);
}
