
#include "ArnoldRenderOptions.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldRenderOptionsNode::id(0x00071000);

MObject CArnoldRenderOptionsNode::s_threads;
MObject CArnoldRenderOptionsNode::s_threads_autodetect;
MObject CArnoldRenderOptionsNode::s_bucket_scanning;
MObject CArnoldRenderOptionsNode::s_bucket_size;
MObject CArnoldRenderOptionsNode::s_clear_before_render;
MObject CArnoldRenderOptionsNode::s_abort_on_error;
MObject CArnoldRenderOptionsNode::s_AA_samples;
MObject CArnoldRenderOptionsNode::s_GI_hemi_samples;
MObject CArnoldRenderOptionsNode::s_GI_specular_samples;
MObject CArnoldRenderOptionsNode::s_use_sample_clamp;
MObject CArnoldRenderOptionsNode::s_AA_sample_clamp;
MObject CArnoldRenderOptionsNode::s_driver_gamma;
MObject CArnoldRenderOptionsNode::s_TM_lgamma;
MObject CArnoldRenderOptionsNode::s_TM_sgamma;
MObject CArnoldRenderOptionsNode::s_TM_tgamma;
MObject CArnoldRenderOptionsNode::s_GI_diffuse_depth;
MObject CArnoldRenderOptionsNode::s_GI_glossy_depth;
MObject CArnoldRenderOptionsNode::s_GI_reflection_depth;
MObject CArnoldRenderOptionsNode::s_GI_refraction_depth;
MObject CArnoldRenderOptionsNode::s_GI_total_depth;
MObject CArnoldRenderOptionsNode::s_background;

void* CArnoldRenderOptionsNode::creator()
{
   return new CArnoldRenderOptionsNode();
}

MStatus CArnoldRenderOptionsNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute eAttr;

   s_threads_autodetect = nAttr.create("threads_autodetect", "thr_auto", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_threads_autodetect);

   s_threads = nAttr.create("threads", "thrds", MFnNumericData::kInt, 1);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(16);
   addAttribute(s_threads);

   s_bucket_scanning = eAttr.create("bucket_scanning", "bktsc", 0);
   nAttr.setKeyable(false);
   eAttr.addField("Top Down", 0);
   eAttr.addField("Bottom Up", 1);
   eAttr.addField("Left To Right", 2);
   eAttr.addField("Right To Left", 3);
   eAttr.addField("Random", 4);
   eAttr.addField("Woven", 5);
   eAttr.addField("Spiral", 6);
   eAttr.addField("Hilbert", 7);
   eAttr.addField("List", 8);
   addAttribute(s_bucket_scanning);

   s_bucket_size = nAttr.create("bucket_size", "bktsz", MFnNumericData::kInt, 64);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(256);
   addAttribute(s_bucket_size);

   s_clear_before_render = nAttr.create("clear_before_render", "clear", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_clear_before_render);

   s_abort_on_error = nAttr.create("abort_on_error", "abort", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_abort_on_error);

   s_AA_samples = nAttr.create("AA_samples", "AAsmpls", MFnNumericData::kInt, 3);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(10);
   nAttr.setMin(0);
   nAttr.setMax(100);
   addAttribute(s_AA_samples);

   s_GI_hemi_samples = nAttr.create("GI_hemi_samples", "GIhemismpls", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setMin(0);
   nAttr.setMax(100);
   addAttribute(s_GI_hemi_samples);

   s_GI_specular_samples = nAttr.create("GI_specular_samples", "GIspecsmpls", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setMin(0);
   nAttr.setMax(100);
   addAttribute(s_GI_specular_samples);

   s_use_sample_clamp = nAttr.create("use_sample_clamp", "usesmpclamp", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_use_sample_clamp);

   s_AA_sample_clamp = nAttr.create("AA_sample_clamp", "smpclamp", MFnNumericData::kFloat, 10);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0.001);
   nAttr.setSoftMax(100);
   addAttribute(s_AA_sample_clamp);

   s_driver_gamma = nAttr.create("driver_gamma", "dgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_driver_gamma);

   s_TM_lgamma = nAttr.create("TM_lgamma", "lgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_TM_lgamma);

   s_TM_sgamma = nAttr.create("TM_sgamma", "sgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_TM_sgamma);

   s_TM_tgamma = nAttr.create("TM_tgamma", "tgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_TM_tgamma);

   s_GI_diffuse_depth = nAttr.create("GI_diffuse_depth", "GI_dd", MFnNumericData::kInt, 1);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(16);
   nAttr.setMin(0);
   nAttr.setMax(10000);
   addAttribute(s_GI_diffuse_depth);

   s_GI_glossy_depth = nAttr.create("GI_glossy_depth", "GI_gd", MFnNumericData::kInt, 1);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(16);
   nAttr.setMin(0);
   nAttr.setMax(10000);
   addAttribute(s_GI_glossy_depth);

   s_GI_reflection_depth = nAttr.create("GI_reflection_depth", "GI_rld", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(16);
   nAttr.setMin(0);
   nAttr.setMax(10000);
   addAttribute(s_GI_reflection_depth);

   s_GI_refraction_depth = nAttr.create("GI_refraction_depth", "GI_rrd", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(16);
   nAttr.setMin(0);
   nAttr.setMax(10000);
   addAttribute(s_GI_refraction_depth);

   s_GI_total_depth = nAttr.create("GI_total_depth", "GI_td", MFnNumericData::kInt, 10);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(16);
   nAttr.setMin(0);
   nAttr.setMax(10000);
   addAttribute(s_GI_total_depth);

   s_background = eAttr.create("background", "bkg", 0);
   nAttr.setKeyable(false);
   eAttr.addField("None", 0);
   eAttr.addField("Image", 1);
   eAttr.addField("Sky", 2);
   eAttr.addField("Sky HDRI", 3);
   addAttribute(s_background);

   return MS::kSuccess;
}
