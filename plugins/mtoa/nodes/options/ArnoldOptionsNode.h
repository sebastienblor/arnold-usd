#pragma once

#include "attributes/AttrHelper.h"

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MObject.h>
#include <maya/MMessage.h>

class CArnoldOptionsNode
   :  public MPxNode
{

public:
   ~CArnoldOptionsNode();

   virtual MStatus compute(const MPlug& plug, MDataBlock& data)
   {
     return MS::kSuccess;
   }

   static MObject getOptionsNode();

   static void* creator();
   static MStatus initialize();
   void postConstructor();
   static void createdCallback(MObject& node, void* clientData);
   static MTypeId id;
   static MCallbackId sId;

   // Attributes
   static CStaticAttrHelper s_attributes;

   static MObject s_optionsNode;
  
   static MObject s_imageFormat;
   static MObject s_aovs;
   static MObject s_aovMode;
   static MObject s_outputVarianceAOVs;
   static MObject s_denoiseBeauty;
   static MObject s_driver;
   static MObject s_drivers;

   static MObject s_renderType;
   static MObject s_outputAssBoundingBox;

   static MObject s_progressive_rendering;
   static MObject s_progressive_initial_level;
   static MObject s_threads;
   static MObject s_threads_autodetect;
   static MObject s_bucket_scanning;
   static MObject s_clear_before_render;
   static MObject s_force_scene_update_before_IPR_refresh;
   static MObject s_force_texture_cache_flush_after_render;
   static MObject s_plugins_path;

   static MObject s_use_sample_clamp;
   static MObject s_use_sample_clamp_AOVs;
   static MObject s_AA_sample_clamp;
   static MObject s_indirect_sample_clamp;
   static MObject s_lock_sampling_noise;
   static MObject s_aa_seed;
   static MObject s_filterType;
   static MObject s_filter;

   static MObject s_driver_gamma;

   static MObject s_light_linking;
   static MObject s_shadow_linking;

   static MObject s_motion_blur_enable;
   static MObject s_mb_camera_enable;
   static MObject s_mb_objects_enable;
   static MObject s_mb_object_deform_enable;
   static MObject s_mb_lights_enable;
   static MObject s_mb_shader_enable;
   static MObject s_motion_steps;
   static MObject s_range_type;
   static MObject s_motion_frames;
   static MObject s_motion_start;
   static MObject s_motion_end;
      
   static MObject s_autotile;
   static MObject s_use_existing_tiled_textures;
   static MObject s_autotx;

   static MObject s_output_ass_filename;
   static MObject s_output_ass_compressed;
   static MObject s_output_ass_mask;

   static MObject s_log_to_file;
   static MObject s_log_to_console;
   static MObject s_log_filename;
   static MObject s_log_max_warnings;
   static MObject s_log_verbosity;
   static MObject s_mtoa_translation_info;

   static MObject s_background;
   static MObject s_atmosphere;
   static MObject s_operator;
   static MObject s_atmosphereShader;

   static MObject s_displayAOV;

   static MObject s_enable_swatch_render;
   
   static MObject s_texture_searchpath;
   static MObject s_procedural_searchpath;
   static MObject s_plugin_searchpath;
   
   static MObject s_user_options;
   
   static MObject s_expand_procedurals;
   static MObject s_kick_render_flags;
   static MObject s_absolute_texture_paths;
   static MObject s_absolute_procedural_paths;
   static MObject s_force_translate_shading_engines;
   static MObject s_export_all_shading_groups;
   static MObject s_export_full_paths;
   static MObject s_export_shading_engine;

   static MObject s_version;
   static MObject s_enable_standin_draw;

   static MObject s_postTranslationCallback;

   static MObject s_IPRRefinementStartedCallback;
   static MObject s_IPRRefinementFinishedCallback;

   static MObject s_IPRStepStartedCallback;
   static MObject s_IPRStepFinishedCallback;

   static MObject s_output_overscan;

   static MObject s_render_unit;
   static MObject s_scene_scale;

   static MObject s_offset_origin;
   static MObject s_origin;
   static MObject s_aov_shaders;
   static MObject s_legacy_gi_glossy_samples;
   static MObject s_legacy_gi_refraction_samples;

   static MObject s_gpu;   
   static MObject s_render_devices;   
   static MObject s_manual_devices;
   
   static MObject s_stats_enable;
   static MObject s_stats_file;
   static MObject s_stats_mode;
   static MObject s_profile_enable;
   static MObject s_profile_file;

};  // class CArnoldOptionsNode
