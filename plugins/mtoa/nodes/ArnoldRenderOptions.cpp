
#include "ArnoldRenderOptions.h"
#include "nodes/ShaderUtils.h"
#include "nodes/ArnoldNodeIDs.h"

#include <ai_universe.h>

#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDGModifier.h>
#include <maya/MDGMessage.h>

MTypeId CArnoldRenderOptionsNode::id(ARNOLD_NODEID_RENDER_OPTIONS);

MCallbackId CArnoldRenderOptionsNode::sId;

MObject CArnoldRenderOptionsNode::s_arnoldRenderImageFormat;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageCompression;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageHalfPrecision;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageGamma;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageOutputPadded;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageQuality;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageOutputFormat;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageTiled;
MObject CArnoldRenderOptionsNode::s_arnoldRenderImageUnpremultAlpha;
MObject CArnoldRenderOptionsNode::s_aovs;
MObject CArnoldRenderOptionsNode::s_renderType;
MObject CArnoldRenderOptionsNode::s_outputAssBoundingBox;
MObject CArnoldRenderOptionsNode::s_progressive_rendering;
MObject CArnoldRenderOptionsNode::s_physically_based;
MObject CArnoldRenderOptionsNode::s_threads;
MObject CArnoldRenderOptionsNode::s_threads_autodetect;
MObject CArnoldRenderOptionsNode::s_bucket_scanning;
MObject CArnoldRenderOptionsNode::s_bucket_size;
MObject CArnoldRenderOptionsNode::s_clear_before_render;
MObject CArnoldRenderOptionsNode::s_abort_on_error;
MObject CArnoldRenderOptionsNode::s_abort_on_license_fail;
MObject CArnoldRenderOptionsNode::s_skip_license_check;
MObject CArnoldRenderOptionsNode::s_plugins_path;
MObject CArnoldRenderOptionsNode::s_AA_samples;
MObject CArnoldRenderOptionsNode::s_GI_diffuse_samples;
MObject CArnoldRenderOptionsNode::s_GI_glossy_samples;
MObject CArnoldRenderOptionsNode::s_GI_sss_hemi_samples;
MObject CArnoldRenderOptionsNode::s_use_sample_clamp;
MObject CArnoldRenderOptionsNode::s_AA_sample_clamp;
MObject CArnoldRenderOptionsNode::s_lock_sampling_noise;
MObject CArnoldRenderOptionsNode::s_aa_seed;
MObject CArnoldRenderOptionsNode::s_filter_type;
MObject CArnoldRenderOptionsNode::s_filter_width;
MObject CArnoldRenderOptionsNode::s_filter_domain;
MObject CArnoldRenderOptionsNode::s_filter_scalar_mode;
MObject CArnoldRenderOptionsNode::s_filter_maximum;
MObject CArnoldRenderOptionsNode::s_filter_minimum;
MObject CArnoldRenderOptionsNode::s_driver_gamma;
MObject CArnoldRenderOptionsNode::s_light_gamma;
MObject CArnoldRenderOptionsNode::s_shader_gamma;
MObject CArnoldRenderOptionsNode::s_texture_gamma;
MObject CArnoldRenderOptionsNode::s_GI_diffuse_depth;
MObject CArnoldRenderOptionsNode::s_GI_glossy_depth;
MObject CArnoldRenderOptionsNode::s_GI_reflection_depth;
MObject CArnoldRenderOptionsNode::s_GI_refraction_depth;
MObject CArnoldRenderOptionsNode::s_GI_total_depth;
MObject CArnoldRenderOptionsNode::s_auto_transparency_depth;
MObject CArnoldRenderOptionsNode::s_auto_transparency_threshold;
MObject CArnoldRenderOptionsNode::s_auto_transparency_probabilistic;
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
MObject CArnoldRenderOptionsNode::s_sss_subpixel_cache;
MObject CArnoldRenderOptionsNode::s_show_samples;
MObject CArnoldRenderOptionsNode::s_max_subdivisions;
MObject CArnoldRenderOptionsNode::s_texture_automip;
MObject CArnoldRenderOptionsNode::s_texture_autotile;
MObject CArnoldRenderOptionsNode::s_texture_max_memory_MB;
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

// Callback is called whenever a ArnoldRenderOptions node is created
void CArnoldRenderOptionsNode::createdCallback(MObject& node, void* clientData)
{  
   MSelectionList list;
   
   list.add( "time1" );
   MObject timeNode;
   list.getDependNode( 0, timeNode ); 
   MFnDependencyNode timeFn( timeNode );
   MObject timeOutAttr = timeFn.attribute( "outTime" );

   MFnDependencyNode renderOptionsFn(node);
   MObject aaSeedAttr = renderOptionsFn.attribute( "aa_seed" );
   
   // Connect to time node
   // 
   MDGModifier modifier;
   modifier.connect( timeNode, timeOutAttr, node, aaSeedAttr );
   modifier.doIt();
}

// Setup the on creation callback
void CArnoldRenderOptionsNode::postConstructor()
{
   CArnoldRenderOptionsNode::sId = MDGMessage::addNodeAddedCallback( CArnoldRenderOptionsNode::createdCallback, "ArnoldRenderOptions" );
}

MStatus CArnoldRenderOptionsNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute eAttr;
   MFnStringData sData;
   MFnTypedAttribute tAttr;
   MFnMessageAttribute mAttr;
   MFnUnitAttribute	uAttr;

   s_arnoldRenderImageFormat = eAttr.create("arnoldRenderImageFormat", "arnif", 0);
   eAttr.setKeyable(false);
   eAttr.addField("OpenEXR", 0);
   eAttr.addField("Tiff", 1);
   eAttr.addField("Jpg", 2);
   eAttr.addField("Png", 3);
   addAttribute(s_arnoldRenderImageFormat);

   s_arnoldRenderImageCompression = eAttr.create("compression","arnic",0);
   eAttr.setKeyable(false);
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

   s_aovs = mAttr.create("aovs", "arniaovs");
   mAttr.setKeyable(false);
   addAttribute(s_aovs);

   s_renderType = eAttr.create("renderType", "arnrt", 0);
   eAttr.setKeyable(false);
   eAttr.addField("Interactive", 0);
   eAttr.addField("Export Ass", 1);
   eAttr.addField("Export and Render", 2);
   addAttribute(s_renderType);

   s_outputAssBoundingBox = nAttr.create("outputAssBoundingBox", "assbb", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_outputAssBoundingBox);

   s_progressive_rendering = nAttr.create("progressive_rendering", "prog", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_progressive_rendering);

   s_physically_based = nAttr.create("physically_based", "phy", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_physically_based);

   s_threads_autodetect = nAttr.create("threads_autodetect", "thr_auto", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_threads_autodetect);

   s_threads = nAttr.create("threads", "thrds", MFnNumericData::kInt, 1);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(64);
   addAttribute(s_threads);

   MAKE_ENUM(s_bucket_scanning, "bucket_scanning", "bktsc", 0, "options", "bucket_scanning");

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

   s_abort_on_license_fail = nAttr.create("abort_on_license_fail", "abortlic", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_abort_on_license_fail);

   s_skip_license_check = nAttr.create("skip_license_check", "skiplic", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_skip_license_check);

   s_plugins_path = tAttr.create("plugins_path", "ppath", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("$ARNOLD_PLUGIN_PATH"));
   addAttribute(s_plugins_path);

   s_AA_samples = nAttr.create("AA_samples", "AAsmpls", MFnNumericData::kInt, 3);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(10);
   nAttr.setMin(1);
   nAttr.setMax(100);
   addAttribute(s_AA_samples);

   s_GI_diffuse_samples = nAttr.create("GI_diffuse_samples", "GIdiffsmpls", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setMin(0);
   nAttr.setMax(100);
   addAttribute(s_GI_diffuse_samples);

   s_GI_glossy_samples = nAttr.create("GI_glossy_samples", "GIglossmpls", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setMin(0);
   nAttr.setMax(100);
   addAttribute(s_GI_glossy_samples);

   s_GI_sss_hemi_samples = nAttr.create("GI_sss_hemi_samples", "GIssshemismpls", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(10);
   nAttr.setMin(0);
   nAttr.setMax(100);
   addAttribute(s_GI_sss_hemi_samples);

   s_use_sample_clamp = nAttr.create("use_sample_clamp", "usesmpclamp", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_use_sample_clamp);

   s_AA_sample_clamp = nAttr.create("AA_sample_clamp", "smpclamp", MFnNumericData::kFloat, 10);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0.001);
   nAttr.setSoftMax(100);
   addAttribute(s_AA_sample_clamp);

   s_lock_sampling_noise = nAttr.create("lock_sampling_noise", "locksn", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_lock_sampling_noise);
      
   s_aa_seed = uAttr.create( "aa_seed", "aaseed", MFnUnitAttribute::kTime );
   uAttr.setStorable(false);
   uAttr.setConnectable(true);
   uAttr.setWritable(true);
   uAttr.setKeyable(false);
   addAttribute(s_aa_seed);
   
   {
      s_filter_type = eAttr.create("filter_type", "flttyp", 0);
      AtNodeEntryIterator* it = AiUniverseGetNodeEntryIterator(AI_NODE_FILTER);
      int i = 0;
      while (!AiNodeEntryIteratorFinished(it))
      {
         AtNodeEntry* nentry = AiNodeEntryIteratorGetNext(it);

         eAttr.addField(AiNodeEntryGetName(nentry), i++);
      }
      eAttr.setDefault("gaussian_filter");
      addAttribute(s_filter_type);
   }

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

   s_light_gamma = nAttr.create("light_gamma", "lgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_light_gamma);

   s_shader_gamma = nAttr.create("shader_gamma", "sgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_shader_gamma);

   s_texture_gamma = nAttr.create("texture_gamma", "tgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_texture_gamma);

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

   s_auto_transparency_depth = nAttr.create("auto_transparency_depth", "at_d", MFnNumericData::kInt, 10);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(16);
   nAttr.setMin(0);
   nAttr.setMax(10000);
   addAttribute(s_auto_transparency_depth);

   s_auto_transparency_threshold = nAttr.create("auto_transparency_threshold", "at_t", MFnNumericData::kFloat, 0.99);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_auto_transparency_threshold);

   s_auto_transparency_probabilistic = nAttr.create("auto_transparency_probabilistic", "at_p", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_auto_transparency_probabilistic);

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

   s_sss_subpixel_cache = nAttr.create("sss_subpixel_cache", "sssspc", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_sss_subpixel_cache);

   MAKE_ENUM(s_show_samples, "show_samples", "sssshs", 0, "options", "show_samples");

   s_max_subdivisions = nAttr.create("max_subdivisions", "maxs", MFnNumericData::kInt, 999);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(999);
   addAttribute(s_max_subdivisions);

   s_texture_automip = nAttr.create("texture_automip", "tx_am", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_texture_automip);

   s_texture_autotile = nAttr.create("texture_autotile", "tx_at", MFnNumericData::kInt, 64);
   nAttr.setKeyable(false);
   addAttribute(s_texture_autotile);

   s_texture_max_memory_MB = nAttr.create("texture_max_memory_MB", "tx_mm", MFnNumericData::kFloat, 100);
   nAttr.setKeyable(false);
   addAttribute(s_texture_max_memory_MB);

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

   s_background = mAttr.create("background", "bkg");
   mAttr.setKeyable(false);
   addAttribute(s_background);

   s_atmosphere = eAttr.create("atmosphere", "atm", 0);
   nAttr.setKeyable(false);
   eAttr.addField("None", 0);
   eAttr.addField("Fog", 1);
   eAttr.addField("Volume Scattering", 2);
   addAttribute(s_atmosphere);

   return MS::kSuccess;
}
