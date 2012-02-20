
#include "ArnoldOptionsNode.h"
#include "render/RenderOptions.h"
#include "session/SessionOptions.h"
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

MTypeId CArnoldOptionsNode::id(ARNOLD_NODEID_RENDER_OPTIONS);

MCallbackId CArnoldOptionsNode::sId;

MObject CArnoldOptionsNode::s_imageFormat;
MObject CArnoldOptionsNode::s_aovs;
MObject CArnoldOptionsNode::s_aovMode;
MObject CArnoldOptionsNode::s_driver;
MObject CArnoldOptionsNode::s_renderType;
MObject CArnoldOptionsNode::s_outputAssBoundingBox;
MObject CArnoldOptionsNode::s_progressive_rendering;
MObject CArnoldOptionsNode::s_progressive_initial_level;
MObject CArnoldOptionsNode::s_force_scene_update_before_IPR_refresh;
MObject CArnoldOptionsNode::s_threads;
MObject CArnoldOptionsNode::s_threads_autodetect;
MObject CArnoldOptionsNode::s_clear_before_render;
MObject CArnoldOptionsNode::s_plugins_path;
MObject CArnoldOptionsNode::s_use_sample_clamp;
MObject CArnoldOptionsNode::s_AA_sample_clamp;
MObject CArnoldOptionsNode::s_lock_sampling_noise;
MObject CArnoldOptionsNode::s_aa_seed;
MObject CArnoldOptionsNode::s_filterType;
MObject CArnoldOptionsNode::s_filter;
MObject CArnoldOptionsNode::s_driver_gamma;
MObject CArnoldOptionsNode::s_light_gamma;
MObject CArnoldOptionsNode::s_shader_gamma;
MObject CArnoldOptionsNode::s_texture_gamma;
MObject CArnoldOptionsNode::s_light_linking;
MObject CArnoldOptionsNode::s_shadow_linking;
MObject CArnoldOptionsNode::s_motion_blur_enable;
MObject CArnoldOptionsNode::s_mb_lights_enable;
MObject CArnoldOptionsNode::s_mb_camera_enable;
MObject CArnoldOptionsNode::s_mb_objects_enable;
MObject CArnoldOptionsNode::s_mb_object_deform_enable;
MObject CArnoldOptionsNode::s_mb_shader_enable;
MObject CArnoldOptionsNode::s_shutter_size;
MObject CArnoldOptionsNode::s_shutter_offset;
MObject CArnoldOptionsNode::s_shutter_type;
MObject CArnoldOptionsNode::s_motion_steps;
MObject CArnoldOptionsNode::s_motion_frames;
MObject CArnoldOptionsNode::s_use_existing_tiled_textures;
MObject CArnoldOptionsNode::s_output_ass_filename;
MObject CArnoldOptionsNode::s_output_ass_compressed;
MObject CArnoldOptionsNode::s_output_ass_mask;
MObject CArnoldOptionsNode::s_log_filename;
MObject CArnoldOptionsNode::s_log_max_warnings;
MObject CArnoldOptionsNode::s_log_console_verbosity;
MObject CArnoldOptionsNode::s_log_file_verbosity;
MObject CArnoldOptionsNode::s_background;
MObject CArnoldOptionsNode::s_atmosphere;
MObject CArnoldOptionsNode::s_displayAOV;
MObject CArnoldOptionsNode::s_enable_swatch_render;
MObject CArnoldOptionsNode::s_texture_searchpath;
MObject CArnoldOptionsNode::s_procedural_searchpath;
MObject CArnoldOptionsNode::s_shader_searchpath;

CStaticAttrHelper CArnoldOptionsNode::s_attributes(CArnoldOptionsNode::addAttribute);

void* CArnoldOptionsNode::creator()
{
   return new CArnoldOptionsNode();
}

// Callback is called whenever a aiOptions node is created
void CArnoldOptionsNode::createdCallback(MObject& node, void* clientData)
{  
   MSelectionList list;
   
   list.add("time1");
   MObject timeNode;
   list.getDependNode(0, timeNode);
   MFnDependencyNode timeFn(timeNode);
   MObject timeOutAttr = timeFn.attribute("outTime");

   MFnDependencyNode renderOptionsFn(node);
   MObject aaSeedAttr = renderOptionsFn.attribute("aa_seed");
   
   // Connect to time node
   // 
   MDGModifier modifier;
   modifier.connect(timeNode, timeOutAttr, node, aaSeedAttr);
   modifier.doIt();
}

// Setup the on creation callback
void CArnoldOptionsNode::postConstructor()
{
   setExistWithoutInConnections(true);
   setExistWithoutOutConnections(true);
   CArnoldOptionsNode::sId = MDGMessage::addNodeAddedCallback(CArnoldOptionsNode::createdCallback, "aiOptions");
}

MStatus CArnoldOptionsNode::initialize()
{
   MFnNumericAttribute  nAttr;
   MFnEnumAttribute eAttr;
   MFnStringData sData;
   MFnTypedAttribute tAttr;
   MFnMessageAttribute mAttr;
   MFnUnitAttribute	uAttr;

   // initialize the attribute helper
   s_attributes.SetNode("options");

   s_imageFormat = tAttr.create("imageFormat", "img", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_imageFormat);

   s_aovs = mAttr.create("aovList", "aovs");
   mAttr.setKeyable(false);
   mAttr.setArray(true);
   mAttr.setReadable(false);
   mAttr.setIndexMatters(false);
   addAttribute(s_aovs);

   s_aovMode = eAttr.create("aovMode", "aovm");
   eAttr.setKeyable(false);
   eAttr.addField("disabled", 0);
   eAttr.addField("enabled", 1);
   eAttr.addField("batch_only", 2);
   eAttr.setDefault(1);
   addAttribute(s_aovMode);
   
   s_attributes.MakeInput("enable_aov_composition");   

   s_renderType = eAttr.create("renderType", "arnrt", 0);
   eAttr.setKeyable(false);
   eAttr.addField("Interactive", MTOA_RENDER_INTERACTIVE);
   eAttr.addField("Export Ass", MTOA_RENDER_EXPORTASS);
   eAttr.addField("Export Ass and Kick", MTOA_RENDER_EXPORTASS_AND_KICK);
   addAttribute(s_renderType);

   s_outputAssBoundingBox = nAttr.create("outputAssBoundingBox", "assbb", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_outputAssBoundingBox);

   s_attributes.MakeInput("preserve_scene_data");

   s_progressive_rendering = nAttr.create("progressive_rendering", "prog", MFnNumericData::kBoolean, true);
   nAttr.setKeyable(false);
   addAttribute(s_progressive_rendering);

   s_progressive_initial_level = nAttr.create("progressive_initial_level", "progil", MFnNumericData::kInt, -3);
   nAttr.setKeyable(false);
   nAttr.setMin(-10);
   nAttr.setMax(100);
   nAttr.setSoftMin(-10);
   nAttr.setSoftMax(10);
   addAttribute(s_progressive_initial_level);

   s_attributes.MakeInput("physically_based");

   s_threads_autodetect = nAttr.create("threads_autodetect", "thr_auto", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_threads_autodetect);

   s_threads = nAttr.create("threads", "thrds", MFnNumericData::kInt, 1);
   nAttr.setKeyable(false);
   nAttr.setMin(1);
   nAttr.setSoftMin(1);
   nAttr.setSoftMax(64);
   addAttribute(s_threads);

   s_attributes.MakeInput("bucket_scanning");
   s_attributes.MakeInput("bucket_size");

   s_clear_before_render = nAttr.create("clear_before_render", "clear", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_clear_before_render);

   s_force_scene_update_before_IPR_refresh = nAttr.create("force_scene_update_before_IPR_refresh", "rec_before_IPR", MFnNumericData::kBoolean, false);
   nAttr.setKeyable(false);
   addAttribute(s_force_scene_update_before_IPR_refresh);
   
   s_attributes.MakeInput("abort_on_error");
   s_attributes.MakeInput("abort_on_license_fail");

   s_attributes.MakeInput("skip_license_check");

   s_plugins_path = tAttr.create("plugins_path", "ppath", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("$ARNOLD_PLUGIN_PATH"));
   addAttribute(s_plugins_path);

   s_attributes.MakeInput("AA_samples");
   s_attributes.MakeInput("GI_diffuse_samples");
   s_attributes.MakeInput("GI_glossy_samples");
   s_attributes.MakeInput("GI_refraction_samples");
   s_attributes.MakeInput("sss_sample_factor");

   s_use_sample_clamp = nAttr.create("use_sample_clamp", "usesmpclamp", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_use_sample_clamp);

   s_attributes.MakeInput("AA_sample_clamp");

   s_lock_sampling_noise = nAttr.create("lock_sampling_noise", "locksn", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_lock_sampling_noise);
      
   s_aa_seed = uAttr.create("AA_seed", "aaseed", MFnUnitAttribute::kTime);
   uAttr.setStorable(false);
   uAttr.setConnectable(true);
   uAttr.setWritable(true);
   uAttr.setKeyable(false);
   addAttribute(s_aa_seed);

   s_filterType = tAttr.create("filterType", "fltr", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_filterType);

   s_driver_gamma = nAttr.create("display_gamma", "dgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_driver_gamma);

   // mtoa has overridden arnold's gamma default of 1.0
   s_light_gamma = nAttr.create("light_gamma", "lgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_light_gamma);

   // mtoa has overridden arnold's gamma default of 1.0
   s_shader_gamma = nAttr.create("shader_gamma", "sgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_shader_gamma);

   // mtoa has overridden arnold's gamma default of 1.0
   s_texture_gamma = nAttr.create("texture_gamma", "tgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_texture_gamma);

   s_attributes.MakeInput("GI_diffuse_depth");
   s_attributes.MakeInput("GI_glossy_depth");
   s_attributes.MakeInput("GI_reflection_depth");
   s_attributes.MakeInput("GI_refraction_depth");
   s_attributes.MakeInput("GI_total_depth");

   s_attributes.MakeInput("auto_transparency_depth");
   s_attributes.MakeInput("auto_transparency_threshold");
   s_attributes.MakeInput("auto_transparency_probabilistic");

   s_light_linking = eAttr.create("lightLinking", "llnk", 0);
      eAttr.setKeyable(false);
      eAttr.addField("None", MTOA_LIGHTLINK_NONE);
      eAttr.addField("Maya Light Links", MTOA_LIGHTLINK_MAYA);
      eAttr.setDefault(MTOA_LIGHTLINK_MAYA);
      addAttribute(s_light_linking);

   s_shadow_linking = eAttr.create("shadowLinking", "slnk", 0);
      eAttr.setKeyable(false);
      eAttr.addField("None", MTOA_SHADOWLINK_NONE);
      eAttr.addField("Follows Light Linking", MTOA_SHADOWLINK_LIGHT);
      eAttr.addField("Maya Shadow Links", MTOA_SHADOWLINK_MAYA);
      eAttr.setDefault(MTOA_SHADOWLINK_LIGHT);
      addAttribute(s_shadow_linking);

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

   s_mb_shader_enable = nAttr.create("mb_shader_enable", "mb_sen", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_mb_shader_enable);

   s_shutter_size = nAttr.create("shutter_size", "shuts", MFnNumericData::kFloat, 1.0f);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(1);
   addAttribute(s_shutter_size);

   s_shutter_offset = nAttr.create("shutter_offset", "shuto", MFnNumericData::kFloat, 0.0f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(-0.5f);
   nAttr.setSoftMax(0.5f);
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

   s_motion_frames = nAttr.create("motion_frames", "motf", MFnNumericData::kFloat, 0.5f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(1);
   nAttr.setMin(0);
   addAttribute(s_motion_frames);

   s_attributes.MakeInput("sss_subpixel_cache");
   s_attributes.MakeInput("show_samples");
   s_attributes.MakeInput("max_subdivisions");
   s_attributes.MakeInput("enable_hit_refinement");
   s_attributes.MakeInput("shadow_terminator_fix");

   // textures
   s_attributes.MakeInput("texture_automip");
   s_attributes.MakeInput("texture_autotile");
   s_attributes.MakeInput("texture_max_memory_MB");
   s_attributes.MakeInput("texture_max_open_files");
   s_attributes.MakeInput("texture_accept_untiled");
   s_attributes.MakeInput("texture_accept_unmipped");
   s_attributes.MakeInput("texture_conservative_lookups");
   s_attributes.MakeInput("texture_per_file_stats");
   s_attributes.MakeInput("texture_diffuse_blur");
   s_attributes.MakeInput("texture_glossy_blur");
   s_use_existing_tiled_textures = nAttr.create("use_existing_tiled_textures", "usetx", MFnNumericData::kBoolean, 0); 
   nAttr.setKeyable(false); 
   addAttribute(s_use_existing_tiled_textures);

   // feature overrides
   s_attributes.MakeInput("ignore_textures");
   s_attributes.MakeInput("ignore_shaders");
   s_attributes.MakeInput("ignore_atmosphere");
   s_attributes.MakeInput("ignore_lights");
   s_attributes.MakeInput("ignore_shadows");
   s_attributes.MakeInput("ignore_subdivision");
   s_attributes.MakeInput("ignore_displacement");
   s_attributes.MakeInput("ignore_motion_blur");
   s_attributes.MakeInput("ignore_smoothing");
   s_attributes.MakeInput("ignore_sss");
   s_attributes.MakeInput("ignore_bump");
   s_attributes.MakeInput("ignore_mis");

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

   s_log_console_verbosity = nAttr.create("log_console_verbosity", "logcv", MFnNumericData::kInt, 3);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(6);
   addAttribute(s_log_console_verbosity);

   s_log_file_verbosity = nAttr.create("log_file_verbosity", "logfv", MFnNumericData::kInt, 3);
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

   s_displayAOV = tAttr.create("displayAOV", "daov", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("RGBA"));
   addAttribute(s_displayAOV);

   s_attributes.MakeInput("binary_ass");

   s_attributes.MakeInput("reference_time");
      
   s_enable_swatch_render = nAttr.create("enable_swatch_render", "ensr", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_enable_swatch_render);

   s_procedural_searchpath = tAttr.create("procedural_searchpath", "pspath", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_procedural_searchpath);

   s_shader_searchpath = tAttr.create("shader_searchpath", "sspath", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_shader_searchpath);

   s_texture_searchpath = tAttr.create("texture_searchpath", "tspath", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create(""));
   addAttribute(s_texture_searchpath);

   s_driver = mAttr.create("driver", "drvr");
   mAttr.setKeyable(false);
   addAttribute(s_driver);

   s_filter = mAttr.create("filter", "filt");
   mAttr.setKeyable(false);
   addAttribute(s_filter);

   return MS::kSuccess;
}
