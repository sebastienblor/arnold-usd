
#include "ArnoldStandardShader.h"
#include "ShaderUtils.h"

#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldStandardShaderNode::id(0x00072000);

MObject CArnoldStandardShaderNode::s_Fresnel;
MObject CArnoldStandardShaderNode::s_Fresnel_affect_diff;
MObject CArnoldStandardShaderNode::s_IOR;
MObject CArnoldStandardShaderNode::s_KattR;
MObject CArnoldStandardShaderNode::s_KattG;
MObject CArnoldStandardShaderNode::s_KattB;
MObject CArnoldStandardShaderNode::s_Katt;
MObject CArnoldStandardShaderNode::s_Kb;
MObject CArnoldStandardShaderNode::s_Kd;
MObject CArnoldStandardShaderNode::s_Kd_colorR;
MObject CArnoldStandardShaderNode::s_Kd_colorG;
MObject CArnoldStandardShaderNode::s_Kd_colorB;
MObject CArnoldStandardShaderNode::s_Kd_color;
MObject CArnoldStandardShaderNode::s_Kr;
MObject CArnoldStandardShaderNode::s_Kr_colorR;
MObject CArnoldStandardShaderNode::s_Kr_colorG;
MObject CArnoldStandardShaderNode::s_Kr_colorB;
MObject CArnoldStandardShaderNode::s_Kr_color;
MObject CArnoldStandardShaderNode::s_Krn;
MObject CArnoldStandardShaderNode::s_Ks;
MObject CArnoldStandardShaderNode::s_Ks_colorR;
MObject CArnoldStandardShaderNode::s_Ks_colorG;
MObject CArnoldStandardShaderNode::s_Ks_colorB;
MObject CArnoldStandardShaderNode::s_Ks_color;
MObject CArnoldStandardShaderNode::s_Ksn;
MObject CArnoldStandardShaderNode::s_Ksss;
MObject CArnoldStandardShaderNode::s_Ksss_colorR;
MObject CArnoldStandardShaderNode::s_Ksss_colorG;
MObject CArnoldStandardShaderNode::s_Ksss_colorB;
MObject CArnoldStandardShaderNode::s_Ksss_color;
MObject CArnoldStandardShaderNode::s_Kt;
MObject CArnoldStandardShaderNode::s_Phong_exponent;
MObject CArnoldStandardShaderNode::s_bounce_factor;
MObject CArnoldStandardShaderNode::s_caustics;
MObject CArnoldStandardShaderNode::s_direct_diffuse;
MObject CArnoldStandardShaderNode::s_direct_specular;
MObject CArnoldStandardShaderNode::s_emission;
MObject CArnoldStandardShaderNode::s_emission_colorR;
MObject CArnoldStandardShaderNode::s_emission_colorG;
MObject CArnoldStandardShaderNode::s_emission_colorB;
MObject CArnoldStandardShaderNode::s_emission_color;
MObject CArnoldStandardShaderNode::s_indirect_diffuse;
MObject CArnoldStandardShaderNode::s_indirect_specular;
MObject CArnoldStandardShaderNode::s_opacityR;
MObject CArnoldStandardShaderNode::s_opacityG;
MObject CArnoldStandardShaderNode::s_opacityB;
MObject CArnoldStandardShaderNode::s_opacity;
MObject CArnoldStandardShaderNode::s_retro_reflector;
MObject CArnoldStandardShaderNode::s_specular_Fresnel;
MObject CArnoldStandardShaderNode::s_sss_radius;
MObject CArnoldStandardShaderNode::s_OUT_colorR;
MObject CArnoldStandardShaderNode::s_OUT_colorG;
MObject CArnoldStandardShaderNode::s_OUT_colorB;
MObject CArnoldStandardShaderNode::s_OUT_color;
MObject CArnoldStandardShaderNode::s_OUT_transparencyR;
MObject CArnoldStandardShaderNode::s_OUT_transparencyG;
MObject CArnoldStandardShaderNode::s_OUT_transparencyB;
MObject CArnoldStandardShaderNode::s_OUT_transparency;

MStatus CArnoldStandardShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldStandardShaderNode::creator()
{
   return new CArnoldStandardShaderNode();
}

MStatus CArnoldStandardShaderNode::initialize()
{
   MFnNumericAttribute  nAttr;

   s_Fresnel = nAttr.create("Fresnel", "frn", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_Fresnel);

   s_Fresnel_affect_diff = nAttr.create("Fresnel_affect_diff", "frndiff", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_Fresnel_affect_diff);

   s_IOR = nAttr.create("IOR", "ior", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_IOR);

   MAKE_COLOR(s_Katt, "Katt", "katt", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Katt);

   s_Kb = nAttr.create("Kb", "kb", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kb);

   s_Kd = nAttr.create("Kd", "kd", MFnNumericData::kFloat, 0.7f);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kd);

   MAKE_COLOR(s_Kd_color, "Kd_color", "kdc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Kd_color);

   s_Kr = nAttr.create("Kr", "kr", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kr);

   MAKE_COLOR(s_Kr_color, "Kr_color", "krc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Kr_color);

   s_Krn = nAttr.create("Krn", "krn", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Krn);

   s_Ks = nAttr.create("Ks", "ks", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ks);

   MAKE_COLOR(s_Ks_color, "Ks_color", "ksc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Ks_color);

   s_Ksn = nAttr.create("Ksn", "ksn", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ksn);

   s_Ksss = nAttr.create("Ksss", "ksss", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Ksss);

   MAKE_COLOR(s_Ksss_color, "Ksss_color", "ksssc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_Ksss_color);

   s_Kt = nAttr.create("Kt", "kt", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_Kt);

   s_Phong_exponent = nAttr.create("Phong_exponent", "phonge", MFnNumericData::kFloat, 10);
   nAttr.setMin(0);
   nAttr.setMax(60);
   MAKE_INPUT(nAttr, s_Phong_exponent);

   s_bounce_factor = nAttr.create("bounce_factor", "bouncef", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(4);
   MAKE_INPUT(nAttr, s_bounce_factor);

   s_caustics = nAttr.create("caustics", "caust", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_caustics);

   s_direct_diffuse = nAttr.create("direct_diffuse", "directd", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_direct_diffuse);

   s_direct_specular = nAttr.create("direct_specular", "directs", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_direct_specular);

   s_emission = nAttr.create("emission", "emiss", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_emission);

   MAKE_COLOR(s_emission_color, "emission_color", "emissc", 1, 1, 1);
   MAKE_INPUT(nAttr, s_emission_color);

   s_indirect_diffuse = nAttr.create("indirect_diffuse", "indirectd", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_indirect_diffuse);

   s_indirect_specular = nAttr.create("indirect_specular", "indirects", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_indirect_specular);

   MAKE_COLOR(s_opacity, "opacity", "opac", 1, 1, 1);
   MAKE_INPUT(nAttr, s_opacity);

   s_retro_reflector = nAttr.create("retro_reflector", "retror", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_retro_reflector);

   s_specular_Fresnel = nAttr.create("specular_Fresnel", "specf", MFnNumericData::kBoolean, 0);
   MAKE_INPUT(nAttr, s_specular_Fresnel);

   s_sss_radius = nAttr.create("sss_radius", "sssr", MFnNumericData::kFloat, 0.1f);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_sss_radius);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_Fresnel, s_OUT_color);
   attributeAffects(s_Fresnel_affect_diff, s_OUT_color);
   attributeAffects(s_IOR, s_OUT_color);
   attributeAffects(s_Katt, s_OUT_color);
   attributeAffects(s_Kb, s_OUT_color);
   attributeAffects(s_Kd, s_OUT_color);
   attributeAffects(s_Kd_color, s_OUT_color);
   attributeAffects(s_Kr, s_OUT_color);
   attributeAffects(s_Kr_color, s_OUT_color);
   attributeAffects(s_Krn, s_OUT_color);
   attributeAffects(s_Ks, s_OUT_color);
   attributeAffects(s_Ks_color, s_OUT_color);
   attributeAffects(s_Ksn, s_OUT_color);
   attributeAffects(s_Ksss, s_OUT_color);
   attributeAffects(s_Ksss_color, s_OUT_color);
   attributeAffects(s_Kt, s_OUT_color);
   attributeAffects(s_Phong_exponent, s_OUT_color);
   attributeAffects(s_bounce_factor, s_OUT_color);
   attributeAffects(s_caustics, s_OUT_color);
   attributeAffects(s_direct_diffuse, s_OUT_color);
   attributeAffects(s_direct_specular, s_OUT_color);
   attributeAffects(s_emission, s_OUT_color);
   attributeAffects(s_emission_color, s_OUT_color);
   attributeAffects(s_indirect_diffuse, s_OUT_color);
   attributeAffects(s_indirect_specular, s_OUT_color);
   attributeAffects(s_opacity, s_OUT_color);
   attributeAffects(s_retro_reflector, s_OUT_color);
   attributeAffects(s_specular_Fresnel, s_OUT_color);
   attributeAffects(s_sss_radius, s_OUT_color);

   attributeAffects(s_opacity, s_OUT_transparency);

   return MS::kSuccess;
}
