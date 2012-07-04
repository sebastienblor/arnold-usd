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

void CAmbientLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "ambient_light");
   MakeCommonAttributes(helper);
}

// DirectionalLight
//

void CDirectionalLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetFlt(light, "angle", FindMayaPlug("aiAngle").asFloat());

}

void CDirectionalLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "distant_light");
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

   AiNodeSetFlt(light, "radius", FindMayaPlug("aiRadius").asFloat());

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
}

void CPointLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "point_light");
   // common attributes
   MakeCommonAttributes(helper);
   // point light attributes
   helper.MakeInput("decay_type");
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

   AiNodeSetFlt(light, "radius", FindMayaPlug("aiRadius").asFloat());

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());

   AiNodeSetFlt(light, "aspect_ratio", FindMayaPlug("aiAspectRatio").asFloat());
   AiNodeSetFlt(light, "lens_radius", FindMayaPlug("aiLensRadius").asFloat());
}

void CSpotLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper = CExtensionAttrHelper(context.maya, "spot_light");
   // common attributes
   MakeCommonAttributes(helper);
   // spot light attributes
   helper.MakeInput("decay_type");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
   helper.MakeInput("aspect_ratio");
   helper.MakeInput("radius");
   helper.MakeInput("lens_radius");
}

// Quad AreaLight
//

void CQuadLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AtPoint vertices[4];

   AiV3Create(vertices[0], 1, 1, 0);
   AiV3Create(vertices[1], 1, -1, 0);
   AiV3Create(vertices[2], -1, -1, 0);
   AiV3Create(vertices[3], -1, 1, 0);

   AiNodeSetArray(light, "vertices", AiArrayConvert(4, 1, AI_TYPE_POINT, vertices));

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetInt(light, "resolution", FindMayaPlug("aiResolution").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   // This translator is used both for Maya Area light and Arnold Quad light. Maya light has the shadowColor attribute,
   // so, if this attribute is found, it has already been exported in the CLightTranslator::Export method
   // If shadowColor is not found, it is an Arnold Quad light and we will need to export the aiShadowColor attribute
   MPlug shadowColorPlug = FindMayaPlug("shadowColor");
   if (shadowColorPlug.isNull())
   {
      AiNodeSetRGB(light, "shadow_color", FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   }
}

void CQuadLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "quad_light");
   // common attributes
   MakeCommonAttributes(helper);
   // quad light attributes
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
   helper.MakeInput("resolution");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}

// Cylinder AreaLight
//
void CCylinderLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   AiNodeSetRGB(light, "shadow_color", FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());

   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   double scale[3] = {1.0f, 1.0f, 1.0f};
   tm.getScale(scale, MSpace::kPreTransform);
   AiNodeSetFlt(light, "radius", float (scale[0]+scale[2])/2.0f);
}

void CCylinderLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "cylinder_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
   helper.MakeInput("affect_volumetrics");
   helper.MakeInput("cast_volumetric_shadows");
}

// Disk AreaLight
//
void CDiskLightTranslator::Export(AtNode* light)
{
   CLightTranslator::Export(light);

   AiNodeSetInt(light,  "decay_type",      FindMayaPlug("aiDecayType").asInt());
   AiNodeSetBool(light, "affect_volumetrics", FindMayaPlug("aiAffectVolumetrics").asBool());
   AiNodeSetBool(light, "cast_volumetric_shadows", FindMayaPlug("aiCastVolumetricShadows").asBool());
   
   AiNodeSetRGB(light, "shadow_color", FindMayaPlug("aiShadowColorR").asFloat(), FindMayaPlug("aiShadowColorG").asFloat(), FindMayaPlug("aiShadowColorB").asFloat());
   
   MTransformationMatrix tm(m_dagPath.inclusiveMatrix());
   double scale[3] = {1.0f, 1.0f, 1.0f};
   tm.getScale(scale, MSpace::kPreTransform);
   AiNodeSetFlt(light, "radius", float (scale[0]+scale[1])/2.0f);
}

void CDiskLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "disk_light");
   // common attributes
   MakeCommonAttributes(helper);
   helper.MakeInput("shadow_color");
   helper.MakeInput("decay_type");
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

   AiNodeSetInt(light, "resolution", FindMayaPlug("resolution").asInt());
   AiNodeSetInt(light, "format", FindMayaPlug("format").asInt());
}

void CSkyDomeLightTranslator::NodeInitializer(CAbTranslator context)
{
   CExtensionAttrHelper helper(context.maya, "skydome_light");
   // Cannot be created both on Node and here
   MakeCommonAttributes(helper);
}

void CMeshLightTranslator::GetMatrix(AtMatrix& matrix)
{
   
}

void CMeshLightTranslator::Export(AtNode* light)
{
   
}

void CMeshLightTranslator::NodeInitializer(CAbTranslator context)
{
   
}
