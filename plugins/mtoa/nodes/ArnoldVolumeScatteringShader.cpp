
#include "ArnoldVolumeScatteringShader.h"
#include "ShaderUtils.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldVolumeScatteringShaderNode::id(0x00072007);

MObject CArnoldVolumeScatteringShaderNode::s_affect_diffuse;
MObject CArnoldVolumeScatteringShaderNode::s_affect_reflection;
MObject CArnoldVolumeScatteringShaderNode::s_attenuation;
MObject CArnoldVolumeScatteringShaderNode::s_density;
MObject CArnoldVolumeScatteringShaderNode::s_eccentricity;
MObject CArnoldVolumeScatteringShaderNode::s_importance_sampling;
MObject CArnoldVolumeScatteringShaderNode::s_mscattering_depth;
MObject CArnoldVolumeScatteringShaderNode::s_mscattering_samples;
MObject CArnoldVolumeScatteringShaderNode::s_phase_function;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_attenuationR;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_attenuationG;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_attenuationB;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_attenuation;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_densityR;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_densityG;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_densityB;
MObject CArnoldVolumeScatteringShaderNode::s_rgb_density;
MObject CArnoldVolumeScatteringShaderNode::s_samples;
MObject CArnoldVolumeScatteringShaderNode::s_sampling_pattern;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_colorR;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_colorG;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_colorB;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_color;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_transparencyR;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_transparencyG;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_transparencyB;
MObject CArnoldVolumeScatteringShaderNode::s_OUT_transparency;

MStatus CArnoldVolumeScatteringShaderNode::compute(const MPlug& plug, MDataBlock& data)
{
   return MS::kUnknownParameter;
}

void* CArnoldVolumeScatteringShaderNode::creator()
{
   return new CArnoldVolumeScatteringShaderNode();
}

MStatus CArnoldVolumeScatteringShaderNode::initialize()
{
   MFnEnumAttribute eAttr;
   MFnNumericAttribute nAttr;

   s_affect_diffuse = nAttr.create("affect_diffuse", "ad", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_affect_diffuse);

   s_affect_reflection = nAttr.create("affect_reflection", "ar", MFnNumericData::kFloat, 1);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_affect_reflection);

   s_attenuation = nAttr.create("attenuation", "att", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_attenuation);

   s_density = nAttr.create("density", "d", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_density);

   s_eccentricity = nAttr.create("eccentricity", "ecc", MFnNumericData::kFloat, 0);
   nAttr.setMin(0);
   nAttr.setMax(1);
   MAKE_INPUT(nAttr, s_eccentricity);

   s_importance_sampling = nAttr.create("importance_sampling", "is", MFnNumericData::kBoolean, 1);
   MAKE_INPUT(nAttr, s_importance_sampling);

   s_mscattering_depth = nAttr.create("mscattering_depth", "msd", MFnNumericData::kInt, 0);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_mscattering_depth);

   s_mscattering_samples = nAttr.create("mscattering_samples", "mss", MFnNumericData::kInt, 0);
   nAttr.setMin(0);
   nAttr.setMax(10);
   MAKE_INPUT(nAttr, s_mscattering_samples);

   s_phase_function = eAttr.create("phase_function", "pf", 0);
   eAttr.addField("isotropic", 0);
   eAttr.addField("rayleigh", 1);
   eAttr.addField("mie_hazy", 2);
   eAttr.addField("mie_murky", 3);
   eAttr.addField("hg", 4);
   addAttribute(s_phase_function);

   MAKE_COLOR(s_rgb_attenuation, "rgb_attenuation", "rgbatt", 1, 1, 1);
   MAKE_INPUT(nAttr, s_rgb_attenuation);

   MAKE_COLOR(s_rgb_density, "rgb_density", "rgbd", 1, 1, 1);
   MAKE_INPUT(nAttr, s_rgb_density);

   s_samples = nAttr.create("samples", "s", MFnNumericData::kInt, 5);
   nAttr.setMin(0);
   nAttr.setMax(64);
   MAKE_INPUT(nAttr, s_samples);

   s_sampling_pattern = eAttr.create("sampling_pattern", "sp", 11);
   eAttr.addField("regular", 0);
   eAttr.addField("random", 1);
   eAttr.addField("jittered", 2);
   eAttr.addField("multi_jittered", 3);
   eAttr.addField("halton", 4);
   eAttr.addField("hammersley", 5);
   eAttr.addField("zaremba", 6);
   eAttr.addField("larcher", 7);
   eAttr.addField("poisson_bc", 8);
   eAttr.addField("poisson_dr", 9);
   eAttr.addField("dithered", 10);
   eAttr.addField("non_uniform", 11);
   eAttr.addField("nrooks", 12);
   addAttribute(s_sampling_pattern);

   // OUTPUT ATTRIBUTES

   MAKE_COLOR(s_OUT_color, "outColor", "oc", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_color);

   MAKE_COLOR(s_OUT_transparency, "outTransparency", "ot", 0, 0, 0);
   MAKE_OUTPUT(nAttr, s_OUT_transparency);

   // DEPENDENCIES

   attributeAffects(s_affect_diffuse, s_OUT_color);
   attributeAffects(s_affect_reflection, s_OUT_color);
   attributeAffects(s_attenuation, s_OUT_color);
   attributeAffects(s_density, s_OUT_color);
   attributeAffects(s_eccentricity, s_OUT_color);
   attributeAffects(s_importance_sampling, s_OUT_color);
   attributeAffects(s_mscattering_depth, s_OUT_color);
   attributeAffects(s_mscattering_samples, s_OUT_color);
   attributeAffects(s_phase_function, s_OUT_color);
   attributeAffects(s_rgb_attenuation, s_OUT_color);
   attributeAffects(s_rgb_density, s_OUT_color);
   attributeAffects(s_samples, s_OUT_color);
   attributeAffects(s_sampling_pattern, s_OUT_color);

   return MS::kSuccess;
}
