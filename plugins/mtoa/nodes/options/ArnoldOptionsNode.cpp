
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

/*
   I'm not exactly happy about this approach but seemingly
   the lookup of the options node using MSelectionList
   greatly increases the memory usage.
*/

MObject CArnoldOptionsNode::s_optionsNode = MObject();
MObject CArnoldOptionsNode::s_imageFormat;
MObject CArnoldOptionsNode::s_aovs;
MObject CArnoldOptionsNode::s_aovMode;
MObject CArnoldOptionsNode::s_driver;
MObject CArnoldOptionsNode::s_drivers;
MObject CArnoldOptionsNode::s_renderType;
MObject CArnoldOptionsNode::s_outputAssBoundingBox;
MObject CArnoldOptionsNode::s_progressive_rendering;
MObject CArnoldOptionsNode::s_progressive_initial_level;
MObject CArnoldOptionsNode::s_force_scene_update_before_IPR_refresh;
MObject CArnoldOptionsNode::s_force_texture_cache_flush_after_render;
MObject CArnoldOptionsNode::s_threads;
MObject CArnoldOptionsNode::s_threads_autodetect;
MObject CArnoldOptionsNode::s_bucket_scanning;
MObject CArnoldOptionsNode::s_clear_before_render;
MObject CArnoldOptionsNode::s_plugins_path;
MObject CArnoldOptionsNode::s_use_sample_clamp;
MObject CArnoldOptionsNode::s_use_sample_clamp_AOVs;
MObject CArnoldOptionsNode::s_AA_sample_clamp;
MObject CArnoldOptionsNode::s_lock_sampling_noise;
MObject CArnoldOptionsNode::s_aa_seed;
MObject CArnoldOptionsNode::s_filterType;
MObject CArnoldOptionsNode::s_filter;
MObject CArnoldOptionsNode::s_driver_gamma;
MObject CArnoldOptionsNode::s_light_linking;
MObject CArnoldOptionsNode::s_shadow_linking;
MObject CArnoldOptionsNode::s_motion_blur_enable;
MObject CArnoldOptionsNode::s_mb_lights_enable;
MObject CArnoldOptionsNode::s_mb_camera_enable;
MObject CArnoldOptionsNode::s_mb_objects_enable;
MObject CArnoldOptionsNode::s_mb_object_deform_enable;
MObject CArnoldOptionsNode::s_mb_shader_enable;
MObject CArnoldOptionsNode::s_motion_steps;
MObject CArnoldOptionsNode::s_range_type;
MObject CArnoldOptionsNode::s_motion_frames;
MObject CArnoldOptionsNode::s_motion_start;
MObject CArnoldOptionsNode::s_motion_end;
MObject CArnoldOptionsNode::s_autotile;
MObject CArnoldOptionsNode::s_use_existing_tiled_textures;
MObject CArnoldOptionsNode::s_autotx;
MObject CArnoldOptionsNode::s_output_ass_filename;
MObject CArnoldOptionsNode::s_output_ass_compressed;
MObject CArnoldOptionsNode::s_output_ass_mask;
MObject CArnoldOptionsNode::s_log_to_file;
MObject CArnoldOptionsNode::s_log_to_console;
MObject CArnoldOptionsNode::s_log_filename;
MObject CArnoldOptionsNode::s_log_max_warnings;
MObject CArnoldOptionsNode::s_log_verbosity;
MObject CArnoldOptionsNode::s_background;
MObject CArnoldOptionsNode::s_atmosphere;
MObject CArnoldOptionsNode::s_atmosphereShader;
MObject CArnoldOptionsNode::s_displayAOV;
MObject CArnoldOptionsNode::s_enable_swatch_render;
MObject CArnoldOptionsNode::s_texture_searchpath;
MObject CArnoldOptionsNode::s_procedural_searchpath;
MObject CArnoldOptionsNode::s_shader_searchpath;
MObject CArnoldOptionsNode::s_user_options;
MObject CArnoldOptionsNode::s_expand_procedurals;
MObject CArnoldOptionsNode::s_kick_render_flags;
MObject CArnoldOptionsNode::s_absolute_texture_paths;
MObject CArnoldOptionsNode::s_absolute_procedural_paths;
MObject CArnoldOptionsNode::s_force_translate_shading_engines;
MObject CArnoldOptionsNode::s_version;
MObject CArnoldOptionsNode::s_enable_standin_draw;
MObject CArnoldOptionsNode::s_IPRRefinementStartedCallback;
MObject CArnoldOptionsNode::s_IPRRefinementFinishedCallback;
MObject CArnoldOptionsNode::s_IPRStepStartedCallback;
MObject CArnoldOptionsNode::s_IPRStepFinishedCallback;
MObject CArnoldOptionsNode::s_output_overscan;
MObject CArnoldOptionsNode::s_render_unit;
MObject CArnoldOptionsNode::s_scene_scale;
MObject CArnoldOptionsNode::s_offset_origin;
MObject CArnoldOptionsNode::s_origin;
MObject CArnoldOptionsNode::s_legacyLightTemperature;


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
   s_optionsNode = thisMObject();
}

CArnoldOptionsNode::~CArnoldOptionsNode()
{
   s_optionsNode = MObject();
}

MObject CArnoldOptionsNode::getOptionsNode()
{
   return s_optionsNode;
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
   nAttr.setSoftMin(-3);
   nAttr.setSoftMax(10);
   addAttribute(s_progressive_initial_level);

   s_threads_autodetect = nAttr.create("threads_autodetect", "thr_auto", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_threads_autodetect);

   s_threads = nAttr.create("threads", "thrds", MFnNumericData::kInt, 1);
   nAttr.setKeyable(false);
   nAttr.setMin(-AI_MAX_THREADS);
   nAttr.setSoftMin(1);
   nAttr.setMax(AI_MAX_THREADS);
   addAttribute(s_threads);

   s_bucket_scanning = eAttr.create("bucketScanning", "bktsc");
   eAttr.addField("top", 0);
   eAttr.addField("left", 1);
   eAttr.addField("random", 2);
   eAttr.addField("spiral", 3);
   eAttr.addField("hilbert", 4);
   eAttr.addField("list", 5);
   eAttr.setDefault(3);
   eAttr.setKeyable(false);
   addAttribute(s_bucket_scanning);
   
   s_attributes.MakeInput("bucket_size");

   s_clear_before_render = nAttr.create("clear_before_render", "clear", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_clear_before_render);

   s_force_scene_update_before_IPR_refresh = nAttr.create("force_scene_update_before_IPR_refresh", "rec_before_IPR", MFnNumericData::kBoolean, false);
   nAttr.setKeyable(false);
   addAttribute(s_force_scene_update_before_IPR_refresh);
   
   s_force_texture_cache_flush_after_render = nAttr.create("force_texture_cache_flush_after_render", "force_texture_flush", MFnNumericData::kBoolean, false);
   nAttr.setKeyable(false);
   addAttribute(s_force_texture_cache_flush_after_render);
   
   
   s_attributes.MakeInput("abort_on_error");
   s_attributes.MakeInput("error_color_bad_texture");
   s_attributes.MakeInput("error_color_bad_pixel");
   s_attributes.MakeInput("abort_on_license_fail");

   s_attributes.MakeInput("skip_license_check");

   s_plugins_path = tAttr.create("plugins_path", "ppath", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("$ARNOLD_PLUGIN_PATH"));
   addAttribute(s_plugins_path);

   s_attributes.MakeInput("AA_samples");
   s_attributes.MakeInput("GI_diffuse_samples");
   s_attributes.MakeInput("GI_specular_samples");
   s_attributes.MakeInput("GI_transmission_samples");
   s_attributes.MakeInput("GI_sss_samples");
   s_attributes.MakeInput("GI_volume_samples");
   
   s_attributes.MakeInput("region_min_x");
   s_attributes.MakeInput("region_max_x");
   s_attributes.MakeInput("region_min_y");   
   s_attributes.MakeInput("region_max_y");

   s_use_sample_clamp = nAttr.create("use_sample_clamp", "usesmpclamp", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_use_sample_clamp);

   s_use_sample_clamp_AOVs = nAttr.create("use_sample_clamp_AOVs", "usesmpclampaovs", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_use_sample_clamp_AOVs);
   
   s_attributes.MakeInput("AA_sample_clamp");

   s_lock_sampling_noise = nAttr.create("lock_sampling_noise", "locksn", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_lock_sampling_noise);

   s_attributes.MakeInput("sss_use_autobump");
      
   s_aa_seed = uAttr.create("AA_seed", "aaseed", MFnUnitAttribute::kTime);
   uAttr.setStorable(false);
   uAttr.setConnectable(true);
   uAttr.setWritable(true);
   uAttr.setKeyable(false);
   addAttribute(s_aa_seed);

   s_filterType = tAttr.create("filterType", "fltr", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_filterType);

#ifdef MTOA_ENABLE_GAMMA
   s_driver_gamma = nAttr.create("display_gamma", "dgamma", MFnNumericData::kFloat, 2.2f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(3);
   nAttr.setMin(0);
   nAttr.setMax(10);
   addAttribute(s_driver_gamma);

   
#endif

   s_attributes.MakeInput("GI_diffuse_depth");
   s_attributes.MakeInput("GI_specular_depth");
   s_attributes.MakeInput("GI_transmission_depth");
   s_attributes.MakeInput("GI_volume_depth");
   s_attributes.MakeInput("GI_total_depth");

   s_attributes.MakeInput("auto_transparency_depth");

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

   s_attributes.MakeInput("low_light_threshold");

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

   s_motion_steps = nAttr.create("motion_steps", "mots", MFnNumericData::kInt, 2);
   nAttr.setKeyable(false);
   nAttr.setMin(2);
   nAttr.setSoftMax(30);
   addAttribute(s_motion_steps);
   
   s_range_type = eAttr.create("range_type", "rgtp");
   eAttr.setKeyable(false);
   eAttr.addField("Start On Frame", MTOA_MBLUR_TYPE_START);
   eAttr.addField("Center On Frame", MTOA_MBLUR_TYPE_CENTER);
   eAttr.addField("End On Frame", MTOA_MBLUR_TYPE_END);
   eAttr.addField("Custom", MTOA_MBLUR_TYPE_CUSTOM);
   eAttr.setDefault(MTOA_MBLUR_TYPE_CENTER);
   addAttribute(s_range_type);
   
   s_motion_frames = nAttr.create("motion_frames", "motf", MFnNumericData::kFloat, 0.5f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(1);
   nAttr.setMin(0);
   addAttribute(s_motion_frames);
   
   s_motion_start = nAttr.create("motion_start", "motstart", MFnNumericData::kFloat, -0.25f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(-1);
   nAttr.setSoftMax(0);
   addAttribute(s_motion_start);
   
   s_motion_end = nAttr.create("motion_end", "motend", MFnNumericData::kFloat, 0.25f);
   nAttr.setKeyable(false);
   nAttr.setSoftMin(0);
   nAttr.setSoftMax(1);
   addAttribute(s_motion_end);

   s_attributes.MakeInput("max_subdivisions");

   // textures
#if MAYA_API_VERSION < 201700
   s_attributes.MakeInput("texture_automip");
#endif
   s_attributes.MakeInput("texture_autotile");
   s_attributes.MakeInput("texture_max_memory_MB");
   s_attributes.MakeInput("texture_max_open_files");
   s_attributes.MakeInput("texture_accept_untiled");
   s_attributes.MakeInput("texture_accept_unmipped");
   s_attributes.MakeInput("texture_conservative_lookups");
   s_attributes.MakeInput("texture_diffuse_blur");
   s_attributes.MakeInput("texture_specular_blur");   
   
   s_autotile = nAttr.create("autotile", "autotile", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_autotile);
   
   int defaultAutoTx = 0;
#if MAYA_API_VERSION >= 201700
   defaultAutoTx = 1;
#endif
   s_use_existing_tiled_textures = nAttr.create("use_existing_tiled_textures", "usetx", MFnNumericData::kBoolean, defaultAutoTx); 
   nAttr.setKeyable(false); 
   addAttribute(s_use_existing_tiled_textures);

   s_autotx = nAttr.create("autotx", "autotx", MFnNumericData::kBoolean, defaultAutoTx);
   nAttr.setKeyable(false);
   addAttribute(s_autotx);

   // feature overrides
   s_attributes.MakeInput("ignore_textures");
   s_attributes.MakeInput("ignore_shaders");
   s_attributes.MakeInput("ignore_atmosphere");
   s_attributes.MakeInput("ignore_lights");
   s_attributes.MakeInput("ignore_shadows");
   s_attributes.MakeInput("ignore_subdivision");
   s_attributes.MakeInput("ignore_displacement");
   s_attributes.MakeInput("ignore_bump");   
   s_attributes.MakeInput("ignore_smoothing");   
   s_attributes.MakeInput("ignore_motion_blur");
   s_attributes.MakeInput("ignore_sss");
   s_attributes.MakeInput("ignore_dof");

   s_output_ass_filename = tAttr.create("output_ass_filename", "file", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_output_ass_filename);

   s_output_ass_compressed = nAttr.create("output_ass_compressed", "oasc", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_output_ass_compressed);

   s_output_ass_mask = nAttr.create("output_ass_mask", "oamask", MFnNumericData::kInt, 0xFFFF);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setMax(0xFFFF);
   addAttribute(s_output_ass_mask);

   s_log_to_file = nAttr.create("log_to_file", "ltofi", MFnNumericData::kBoolean, 0);
   nAttr.setKeyable(false);
   addAttribute(s_log_to_file);

   s_log_to_console = nAttr.create("log_to_console", "ltocon", MFnNumericData::kBoolean, 1);
   nAttr.setKeyable(false);
   addAttribute(s_log_to_console);

   s_log_filename = tAttr.create("log_filename", "logf", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("$MTOA_LOG_PATH/arnold.log"));
   addAttribute(s_log_filename);

   s_log_max_warnings = nAttr.create("log_max_warnings", "logw", MFnNumericData::kInt, 100);
   nAttr.setKeyable(false);
   nAttr.setMin(0);
   nAttr.setSoftMax(100);
   nAttr.setDefault(5);
   addAttribute(s_log_max_warnings);

   s_log_verbosity = eAttr.create("log_verbosity", "logv", 0);
   nAttr.setKeyable(false);
   eAttr.addField("Errors", MTOA_LOG_ERRORS);
   eAttr.addField("Warnings + Info", MTOA_LOG_WANINGS_INFO);
   eAttr.addField("Debug", MTOA_LOG_DEBUG);
   addAttribute(s_log_verbosity);

   s_background = mAttr.create("background", "bkg");
   mAttr.setKeyable(false);
   mAttr.setReadable(true);
   addAttribute(s_background);

   s_atmosphere = mAttr.create("atmosphere", "atm");
   mAttr.setKeyable(false);
   mAttr.setReadable(true);
   addAttribute(s_atmosphere);

   s_displayAOV = tAttr.create("displayAOV", "daov", MFnData::kString);
   tAttr.setKeyable(false);
   tAttr.setDefault(sData.create("beauty"));
   addAttribute(s_displayAOV);

   //s_attributes.MakeInput("binary_ass");
   MObject tempAttr = nAttr.create(s_attributes.GetMayaAttrName("binary_ass"), s_attributes.GetMayaAttrShortName("binary_ass"), MFnNumericData::kBoolean);
   nAttr.setDefault(true);
   nAttr.setKeyable(false);
   addAttribute(tempAttr);

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
   mAttr.setReadable(false);
   addAttribute(s_driver);

   s_filter = mAttr.create("filter", "filt");
   mAttr.setKeyable(false);
   mAttr.setReadable(false);
   addAttribute(s_filter);
   
   s_user_options = tAttr.create("aiUserOptions", "ai_user_options", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_user_options);

   s_drivers = mAttr.create("drivers", "drivers");
   mAttr.setKeyable(false);
   mAttr.setArray(true);
   mAttr.setReadable(false);
   mAttr.setIndexMatters(false);
   addAttribute(s_drivers);
   
   s_expand_procedurals = nAttr.create("expandProcedurals", "expand_procedurals", MFnNumericData::kBoolean);
   nAttr.setKeyable(false);
   nAttr.setDefault(false);
   addAttribute(s_expand_procedurals);

   s_kick_render_flags = tAttr.create("kickRenderFlags", "kick_render_flags", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_kick_render_flags);

   s_absolute_texture_paths = nAttr.create("absoluteTexturePaths", "absolute_texture_paths", MFnNumericData::kBoolean);
   nAttr.setKeyable(false);
   nAttr.setDefault(true);
   addAttribute(s_absolute_texture_paths);

   s_absolute_procedural_paths = nAttr.create("absoluteProceduralPaths", "absolute_procedural_paths", MFnNumericData::kBoolean);
   nAttr.setKeyable(false);
   nAttr.setDefault(true);
   addAttribute(s_absolute_procedural_paths);

   s_force_translate_shading_engines = nAttr.create("forceTranslateShadingEngines", "force_translate_shading_engines", MFnNumericData::kBoolean);
   nAttr.setKeyable(false);
   nAttr.setDefault(false);
   addAttribute(s_force_translate_shading_engines);
   
   s_version = tAttr.create("version", "version", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_version);

   s_enable_standin_draw = eAttr.create("standinDrawOverride", "standin_draw_override");
   eAttr.addField("Use Local Settings", 0);   
   eAttr.addField("Bounding Box", 1);
   eAttr.addField("Disable Draw", 2);
   eAttr.addField("Disable Load", 3);
   eAttr.setDefault(0);
   addAttribute(s_enable_standin_draw);

   s_IPRRefinementStartedCallback = tAttr.create("IPRRefinementStarted", "ipr_refinement_started", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_IPRRefinementStartedCallback);

   s_IPRRefinementFinishedCallback = tAttr.create("IPRRefinementFinished", "ipr_refinement_finished", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_IPRRefinementFinishedCallback);

   s_IPRStepStartedCallback = tAttr.create("IPRStepStarted", "ipr_step_started", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_IPRStepStartedCallback);

   s_IPRStepFinishedCallback = tAttr.create("IPRStepFinished", "ipr_step_finished", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_IPRStepFinishedCallback);

   s_output_overscan = tAttr.create("outputOverscan", "output_overscan", MFnData::kString);
   tAttr.setKeyable(false);
   addAttribute(s_output_overscan);

   s_render_unit = eAttr.create("renderUnit", "render_unit");
   eAttr.setKeyable(false);
   eAttr.addField("Use Maya Unit", 0);
   eAttr.addField("Use Custom Scaling", 1);
   eAttr.addField("Inch", 2);
   eAttr.addField("Feet", 3);
   eAttr.addField("Yard", 4);
   eAttr.addField("Mile", 5);
   eAttr.addField("Millimeter", 6);
   eAttr.addField("Centimeter", 7);
   eAttr.addField("Kilometer", 8);
   eAttr.addField("Meter", 9);
   eAttr.setDefault(0);
   addAttribute(s_render_unit);

   s_scene_scale = nAttr.create("sceneScale", "scene_scale", MFnNumericData::kDouble);
   nAttr.setDefault(1.0);
   nAttr.setKeyable(false);
   nAttr.setMin(0.0);
   nAttr.setSoftMin(0.01);
   nAttr.setSoftMax(5.0);
   addAttribute(s_scene_scale);

   s_offset_origin = nAttr.create("offsetOrigin", "offset_origin", MFnNumericData::kBoolean);
   nAttr.setDefault(false);
   nAttr.setKeyable(false);
   addAttribute(s_offset_origin);

   s_origin = mAttr.create("origin", "orig");
   mAttr.setKeyable(false);
   addAttribute(s_origin);

   s_legacyLightTemperature = nAttr.create("legacyLightTemperature", "legacyLightTemperature", MFnNumericData::kBoolean);
   nAttr.setDefault(false);
   nAttr.setKeyable(false);
   addAttribute(s_legacyLightTemperature);

   return MS::kSuccess;
}
