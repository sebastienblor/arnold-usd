#ifndef ARNOLD_RENDER_OPTIONS_H
#define ARNOLD_RENDER_OPTIONS_H

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class CArnoldRenderOptionsNode
   :  public MPxNode
{

public:

   virtual MStatus compute(const MPlug& plug, MDataBlock& data)
   {
      return MS::kSuccess;
   }

   static void* creator();
   static MStatus initialize();

	static MTypeId id;

   // Attributes

   static MObject s_threads;
   static MObject s_threads_autodetect;
   static MObject s_bucket_scanning;
   static MObject s_bucket_size;
   static MObject s_clear_before_render;
   static MObject s_abort_on_error;
   static MObject s_plugins_path;

   static MObject s_AA_samples;
   static MObject s_GI_hemi_samples;
   static MObject s_GI_specular_samples;
   static MObject s_use_sample_clamp;
   static MObject s_AA_sample_clamp;

   static MObject s_driver_gamma;
   static MObject s_TM_lgamma;
   static MObject s_TM_sgamma;
   static MObject s_TM_tgamma;

   static MObject s_GI_diffuse_depth;
   static MObject s_GI_glossy_depth;
   static MObject s_GI_reflection_depth;
   static MObject s_GI_refraction_depth;
   static MObject s_GI_total_depth;

   static MObject s_motion_blur_enable;
   static MObject s_mb_camera_enable;
   static MObject s_mb_objects_enable;
   static MObject s_mb_object_deform_enable;
   static MObject s_mb_lights_enable;
   static MObject s_shutter_size;
   static MObject s_shutter_offset;
   static MObject s_shutter_type;
   static MObject s_motion_steps;
   static MObject s_motion_frames;

   static MObject s_max_subdivisions;

   static MObject s_output_ass_filename;
   static MObject s_output_ass_compressed;
   static MObject s_output_ass_mask;

   static MObject s_log_filename;
   static MObject s_log_max_warnings;
   static MObject s_log_console_verbosity;
   static MObject s_log_file_verbosity;

   static MObject s_background;
   static MObject s_atmosphere;

};  // class CArnoldRenderOptionsNode

#endif // ARNOLD_RENDER_OPTIONS_H
