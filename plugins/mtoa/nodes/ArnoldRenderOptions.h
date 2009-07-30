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

   static MObject s_background;

};  // class CArnoldRenderOptionsNode

#endif // ARNOLD_RENDER_OPTIONS_H
