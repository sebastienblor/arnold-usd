
#include "ArnoldRenderOptions.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

MTypeId CArnoldRenderOptionsNode::id(0x00071111);

MObject CArnoldRenderOptionsNode::s_threads;
MObject CArnoldRenderOptionsNode::s_threads_autodetect;
MObject CArnoldRenderOptionsNode::s_bucket_scanning;
MObject CArnoldRenderOptionsNode::s_bucket_size;
MObject CArnoldRenderOptionsNode::s_abort_on_errors;
MObject CArnoldRenderOptionsNode::s_AA_samples;
MObject CArnoldRenderOptionsNode::s_GI_hemi_samples;
MObject CArnoldRenderOptionsNode::s_GI_specular_samples;
MObject CArnoldRenderOptionsNode::s_use_sample_clamp;
MObject CArnoldRenderOptionsNode::s_AA_sample_clamp;

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

   s_abort_on_errors = nAttr.create("abort_on_errors", "abort", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_abort_on_errors);

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

   return MS::kSuccess;
}
