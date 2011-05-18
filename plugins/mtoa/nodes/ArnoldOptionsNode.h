#ifndef ARNOLDOPTIONSNODE_H
#define ARNOLDOPTIONSNODE_H

#include "utils/AttrHelper.h"

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

   virtual MStatus compute(const MPlug& plug, MDataBlock& data)
   {
     return MS::kSuccess;
   }

   static void* creator();
   static MStatus initialize();
   void postConstructor();
   static void createdCallback(MObject& node, void* clientData);
   static MTypeId id;
   static MCallbackId sId;

   // Attributes
   static CStaticAttrHelper s_attributes;

   static MObject s_arnoldRenderImageFormat;
   static MObject s_arnoldRenderImageCompression;
   static MObject s_arnoldRenderImageHalfPrecision;
   static MObject s_arnoldRenderImageOutputPadded;
   static MObject s_arnoldRenderImageGamma;
   static MObject s_arnoldRenderImageQuality;
   static MObject s_arnoldRenderImageOutputFormat;
   static MObject s_arnoldRenderImageTiled;
   static MObject s_arnoldRenderImageUnpremultAlpha;
   static MObject s_aovs;

   static MObject s_renderType;
   static MObject s_outputAssBoundingBox;

   static MObject s_progressive_rendering;
   static MObject s_threads;
   static MObject s_threads_autodetect;
   static MObject s_clear_before_render;
   static MObject s_plugins_path;

   static MObject s_use_sample_clamp;
   static MObject s_AA_sample_clamp;
   static MObject s_lock_sampling_noise;
   static MObject s_aa_seed;
   static MObject s_filter_type;
   static MObject s_filter_width;
   static MObject s_filter_domain;
   static MObject s_filter_scalar_mode;
   static MObject s_filter_maximum;
   static MObject s_filter_minimum;

   static MObject s_driver_gamma;
   static MObject s_light_gamma;
   static MObject s_shader_gamma;
   static MObject s_texture_gamma;

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

   static MObject s_output_ass_filename;
   static MObject s_output_ass_compressed;
   static MObject s_output_ass_mask;

   static MObject s_log_filename;
   static MObject s_log_max_warnings;
   static MObject s_log_console_verbosity;
   static MObject s_log_file_verbosity;

   static MObject s_background;
   static MObject s_atmosphere;

};  // class CArnoldOptionsNode

#endif // ARNOLDOPTIONSNODE_H
