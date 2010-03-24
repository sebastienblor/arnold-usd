
#include "ArnoldRenderOptions.h"

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>

MTypeId CArnoldRenderOptionsNode::id(0x00071000);

MObject CArnoldRenderOptionsNode::s_arnoldRenderImageFormat;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageCompression;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageHalfPrecision;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageGamma;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageOutputPadded;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageQuality;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageOutputFormat;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageTiled;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageUnpremultAlpha;
MObject CArnoldRenderOptionsNode::s_threads;
MObject CArnoldRenderOptionsNode::s_threads_autodetect;
MObject CArnoldRenderOptionsNode::s_bucket_scanning;
MObject CArnoldRenderOptionsNode::s_bucket_size;
MObject CArnoldRenderOptionsNode::s_clear_before_render;
MObject CArnoldRenderOptionsNode::s_abort_on_error;
MObject CArnoldRenderOptionsNode::s_plugins_path;
MObject CArnoldRenderOptionsNode::s_AA_samples;
MObject CArnoldRenderOptionsNode::s_GI_hemi_samples;
MObject CArnoldRenderOptionsNode::s_GI_specular_samples;
MObject CArnoldRenderOptionsNode::s_use_sample_clamp;
MObject CArnoldRenderOptionsNode::s_AA_sample_clamp;
MObject CArnoldRenderOptionsNode::s_filter_type;
MObject CArnoldRenderOptionsNode::s_filter_width;
MObject CArnoldRenderOptionsNode::s_filter_domain;
MObject CArnoldRenderOptionsNode::s_filter_scalar_mode;
MObject CArnoldRenderOptionsNode::s_filter_maximum;
MObject CArnoldRenderOptionsNode::s_filter_minimum;
MObject CArnoldRenderOptionsNode::s_driver_gamma;
MObject CArnoldRenderOptionsNode::s_TM_lgamma;
MObject CArnoldRenderOptionsNode::s_TM_sgamma;
MObject CArnoldRenderOptionsNode::s_TM_tgamma;
MObject CArnoldRenderOptionsNode::s_GI_diffuse_depth;
MObject CArnoldRenderOptionsNode::s_GI_glossy_depth;
MObject CArnoldRenderOptionsNode::s_GI_reflection_depth;
MObject CArnoldRenderOptionsNode::s_GI_refraction_depth;
MObject CArnoldRenderOptionsNode::s_GI_total_depth;
MObject CArnoldRenderOptionsNode::s_motion_blur_enable;
MObject CArnoldRenderOptionsNode::s_mb_lights_enable;
MObject CArnoldRenderOptionsNode::s_mb_camera_enable;
MObject CArnoldRenderOptionsNode::s_mb_objects_enable;
MObject CArnoldRenderOptionsNode::s_mb_object_deform_enable;
MObject CArnoldRenderOptionsNode::s_shutter_size;
MObject CArnoldRenderOptionsNode::s_shutter_offset;
MObject CArnoldRenderOptionsNode::s_shutter_type;
MObject CArnoldRenderOptionsNode::s_motion_steps;
MObject CArnoldRenderOptionsNode::s_motion_frames;
MObject CArnoldRenderOptionsNode::s_max_subdivisions;
MObject CArnoldRenderOptionsNode::s_output_ass_filename;
MObject CArnoldRenderOptionsNode::s_output_ass_compressed;
MObject CArnoldRenderOptionsNode::s_output_ass_mask;
MObject CArnoldRenderOptionsNode::s_log_filename;
MObject CArnoldRenderOptionsNode::s_log_max_warnings;
MObject CArnoldRenderOptionsNode::s_log_console_verbosity;
MObject CArnoldRenderOptionsNode::s_log_file_verbosity;
MObject CArnoldRenderOptionsNode::s_background;
MObject CArnoldRenderOptionsNode::s_atmosphere;

void* CArnoldRenderOptionsNode::creator()
{
   return new CArnoldRenderOptionsNode();
}

MStatus CArnoldRenderOptionsNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute eAttr;
   MFnStringData sData;
   MFnTypedAttribute tAttr;

   s_arnoldRenderImageFormat = eAttr.create("arnoldRenderImageFormat", "arnif", 0);
   nAttr.setKeyable(false);
   eAttr.addField("OpenEXR", 0);
   eAttr.addField("Tiff", 1);
   eAttr.addField("Jpg", 2);
   eAttr.addField("Png", 3);
   addAttribute(s_arnoldRenderImageFormat);

   s_arnoldRenderImageCompression = eAttr.create("compression","arnic",0);
   nAttr.setKeyable(false);
   eAttr.addField("none", 0);
   eAttr.addField("rle", 1);
   eAttr.addField("zip", 2);
   eAttr.addField("piz", 3);
   eAttr.addField("pxr24", 4);
   addAttribute(s_arnoldRenderImageCompression);

   s_arnoldRenderImageHalfPrecision = nAttr.create("half_precision", "arnihp", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_arnoldRenderImageHalfPrecision);

   s_arnoldRenderImageGamma = nAttr.create("gamma", "arnig", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0.001);
   nAttr.setSoftMax(5);
   addAttribute(s_arnoldRenderImageGamma);

   s_arnoldRenderImageOutputPadded = nAttr.create("output_padded", "arniop", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_arnoldRenderImageOutputPadded);

   s_arnoldRenderImageQuality = nAttr.create("quality", "arniq", MFnNumericData::kInt, 100);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(100);
   addAttribute(s_arnoldRenderImageQuality);

   s_arnoldRenderImageOutputFormat = eAttr.create("format","arniof",0);
   nAttr.setKeyable(false);
   eAttr.addField("int8", 0);
   eAttr.addField("int16", 1);
   eAttr.addField("int32", 2);
   eAttr.addField("float32", 3);
   addAttribute(s_arnoldRenderImageOutputFormat);

   s_arnoldRenderImageTiled = nAttr.create("tiled", "arnitld", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_arnoldRenderImageTiled);

   s_arnoldRenderImageUnpremultAlpha = nAttr.create("unpremult_alpha", "arniua", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_arnoldRenderImageUnpremultAlpha);

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

   s_clear_before_render = nAttr.create("clear_before_render", "clear", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_clear_before_render);

   s_abort_on_error = nAttr.create("abort_on_error", "abort", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_abort_on_error);

   s_plugins_path = tAttr.create("plugins_path", "ppath", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("$MTOA_PLUGINS_PATH"));
   addAttribute(s_plugins_path);

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

   s_filter_type = eAttr.create("filter_type", "flttyp", 9);
   nAttr.setKeyable(false);
   eAttr.addField("box_filter",0);
   eAttr.addField("catrom2d_filter",1);
   eAttr.addField("catrom_filter",2);
   eAttr.addField("closest_filter",3);
   eAttr.addField("cone_filter",4);
   eAttr.addField("cook_filter",5);
   eAttr.addField("cubic_filter",6);
   eAttr.addField("disk_filter",7);
   eAttr.addField("farthest_filter",8);
   eAttr.addField("gaussian_filter",9);
   eAttr.addField("heatmap_filter",10);
   eAttr.addField("mitnet_filter",11);
   eAttr.addField("sinc_filter",12);
   eAttr.addField("triangle_filter",13);
   eAttr.addField("variance_filter",14);
   eAttr.addField("video_filter",15);
   addAttribute(s_filter_type);

   s_filter_width = nAttr.create("filter_width", "fltwdth", MFnNumericData::kFloat, 2.0f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(4);
   addAttribute(s_filter_width);

   s_filter_domain = eAttr.create("filter_domain", "fltdomn", 0);
   nAttr.setKeyable(false);
   eAttr.addField("first_hit",0);
   eAttr.addField("all_hits",1);
   addAttribute(s_filter_domain);

   s_filter_scalar_mode = nAttr.create("filter_scalar_mode", "fltscmd", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_filter_scalar_mode);

   s_filter_maximum = nAttr.create("filter_maximum", "fltmax", MFnNumericData::kFloat, 1.0f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   addAttribute(s_filter_maximum);

   s_filter_minimum = nAttr.create("filter_minimum", "fltmin", MFnNumericData::kFloat, 0.0f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   addAttribute(s_filter_minimum);

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

   s_motion_blur_enable = nAttr.create("motion_blur_enable", "mb_en", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_motion_blur_enable);

   s_mb_lights_enable = nAttr.create("mb_lights_enable", "mb_len", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_mb_lights_enable);

   s_mb_camera_enable = nAttr.create("mb_camera_enable", "mb_cen", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_mb_camera_enable);

   s_mb_objects_enable = nAttr.create("mb_objects_enable", "mb_oen", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_mb_objects_enable);

   s_mb_object_deform_enable = nAttr.create("mb_object_deform_enable", "mb_den", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_mb_object_deform_enable);

   s_shutter_size = nAttr.create("shutter_size", "shuts", MFnNumericData::kFloat, 0.5f);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(1);
   addAttribute(s_shutter_size);

   s_shutter_offset = nAttr.create("shutter_offset", "shuto", MFnNumericData::kFloat, 0);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(-0.5f);
   nAttr.setSoftMax(0.5f);
   nAttr.setMin(-1);
   nAttr.setMax(1);
   addAttribute(s_shutter_offset);

   s_shutter_type = eAttr.create("shutter_type", "shutt", 0);
   nAttr.setKeyable(false);
   eAttr.addField("Box", 0);
   eAttr.addField("Triangle", 1);
   addAttribute(s_shutter_type);

   s_motion_steps = nAttr.create("motion_steps", "mots", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setMin(2);
   nAttr.setMax(30);
   addAttribute(s_motion_steps);

   s_motion_frames = nAttr.create("motion_frames", "motf", MFnNumericData::kFloat, 1);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(2);
   nAttr.setMin(0);
   nAttr.setMax(20);
   addAttribute(s_motion_frames);

   s_max_subdivisions = nAttr.create("max_subdivisions", "maxs", MFnNumericData::kInt, 999);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(999);
   addAttribute(s_max_subdivisions);

   s_output_ass_filename = tAttr.create("output_ass_filename", "file", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("$MTOA_ASS_PATH/mtoa.ass"));
   addAttribute(s_output_ass_filename);

   s_output_ass_compressed = nAttr.create("output_ass_compressed", "oasc", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_output_ass_compressed);

   s_output_ass_mask = nAttr.create("output_ass_mask", "oamask", MFnNumericData::kInt, 0xFFFF);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(0xFFFF);
   addAttribute(s_output_ass_mask);

   s_log_filename = tAttr.create("log_filename", "logf", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("$MTOA_LOG_PATH/arnold.log"));
   addAttribute(s_log_filename);

   s_log_max_warnings = nAttr.create("log_max_warnings", "logw", MFnNumericData::kInt, 100);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(100000);
   addAttribute(s_log_max_warnings);

   s_log_console_verbosity = nAttr.create("log_console_verbosity", "logcv", MFnNumericData::kInt, 6);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(6);
   addAttribute(s_log_console_verbosity);

   s_log_file_verbosity = nAttr.create("log_file_verbosity", "logfv", MFnNumericData::kInt, 6);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(6);
   addAttribute(s_log_file_verbosity);

   s_background = eAttr.create("background", "bkg", 0);
   nAttr.setKeyable(false);
   eAttr.addField("None", 0);
   eAttr.addField("Sky", 1);
   addAttribute(s_background);

   s_atmosphere = eAttr.create("atmosphere", "atm", 0);
   nAttr.setKeyable(false);
   eAttr.addField("None", 0);
   eAttr.addField("Fog", 1);
   eAttr.addField("Volume Scattering", 2);
   addAttribute(s_atmosphere);

   return MS::kSuccess;
}
