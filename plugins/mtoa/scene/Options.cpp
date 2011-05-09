#include "Options.h"
#include "render/RenderSession.h"
#include "render/RenderOptions.h"

#include <ai_universe.h>
#include <ai_msg.h>

#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MPlugArray.h>

void CRenderOptionsTranslator::SetupImageOptions(AtNode* options)
{
   const CRenderOptions* renderOptions = CRenderSession::GetInstance()->RenderOptions();
   if (renderOptions->useRenderRegion())
   {
      AiNodeSetInt(options, "region_min_x", renderOptions->minX());
      AiNodeSetInt(options, "region_min_y", renderOptions->height() - renderOptions->maxY() - 1);
      AiNodeSetInt(options, "region_max_x", renderOptions->maxX());
      AiNodeSetInt(options, "region_max_y", renderOptions->height() - renderOptions->minY() - 1);
   }

   AiNodeSetInt(options, "xres", renderOptions->width());
   AiNodeSetInt(options, "yres", renderOptions->height());
   AiNodeSetFlt(options, "aspect_ratio", renderOptions->pixelAspectRatio());
}

AtNode* CRenderOptionsTranslator::CreateArnoldNodes()
{
   return AiUniverseGetOptions();
}

void CRenderOptionsTranslator::Export(AtNode* options)
{
   SetupImageOptions(options);
   MTime currentTime;

   currentTime = MAnimControl::currentTime();

   ProcessParameter(options, "physically_based", AI_TYPE_BOOLEAN);
   AiNodeSetInt(options, "threads", GetFnNode().findPlug("threads_autodetect").asBool() ? 0 : GetFnNode().findPlug("threads").asInt());
   ProcessParameter(options, "bucket_scanning", AI_TYPE_ENUM);
   ProcessParameter(options, "bucket_size", AI_TYPE_INT);
   ProcessParameter(options, "abort_on_error", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "abort_on_license_fail", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "skip_license_check", AI_TYPE_BOOLEAN);

   ProcessParameter(options, "AA_samples", AI_TYPE_INT);
   ProcessParameter(options, "GI_diffuse_samples", AI_TYPE_INT);
   ProcessParameter(options, "GI_glossy_samples", AI_TYPE_INT);
   ProcessParameter(options, "GI_sss_hemi_samples", AI_TYPE_INT);
   if (GetFnNode().findPlug("use_sample_clamp").asBool())
      ProcessParameter(options, "AA_sample_clamp", AI_TYPE_FLOAT);

   // FIXME: this is supposed to use a connection to AA_seed attribute
   if (!GetFnNode().findPlug("lock_sampling_noise").asBool())
      AiNodeSetInt(options, "AA_seed", (AtInt)currentTime.value());

   ProcessParameter(options, "light_gamma", AI_TYPE_FLOAT);
   ProcessParameter(options, "shader_gamma", AI_TYPE_FLOAT);
   ProcessParameter(options, "texture_gamma", AI_TYPE_FLOAT);

   ProcessParameter(options, "GI_diffuse_depth", AI_TYPE_INT);
   ProcessParameter(options, "GI_glossy_depth", AI_TYPE_INT);
   ProcessParameter(options, "GI_reflection_depth", AI_TYPE_INT);
   ProcessParameter(options, "GI_refraction_depth", AI_TYPE_INT);
   ProcessParameter(options, "GI_total_depth", AI_TYPE_INT);

   ProcessParameter(options, "auto_transparency_depth", AI_TYPE_INT);
   ProcessParameter(options, "auto_transparency_threshold", AI_TYPE_FLOAT);
   ProcessParameter(options, "auto_transparency_probabilistic", AI_TYPE_BOOLEAN);

   ProcessParameter(options, "sss_subpixel_cache", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "show_samples", AI_TYPE_ENUM);

   ProcessParameter(options, "max_subdivisions", AI_TYPE_INT);

   ProcessParameter(options, "enable_hit_refinement", AI_TYPE_BOOLEAN);

   ProcessParameter(options, "texture_automip", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "texture_autotile", AI_TYPE_INT);
   ProcessParameter(options, "texture_max_memory_MB", AI_TYPE_FLOAT);
   ProcessParameter(options, "texture_max_open_files", AI_TYPE_INT);
   ProcessParameter(options, "texture_accept_untiled", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "texture_failure_retries", AI_TYPE_INT);
   ProcessParameter(options, "texture_conservative_lookups", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "texture_glossy_blur", AI_TYPE_FLOAT);
   ProcessParameter(options, "texture_diffuse_blur", AI_TYPE_FLOAT);
   ProcessParameter(options, "texture_per_file_stats", AI_TYPE_BOOLEAN);

   ProcessParameter(options, "ignore_textures", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_shaders", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_lights", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_shadows", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_subdivision", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_displacement", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_motion_blur", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_smoothing", AI_TYPE_BOOLEAN);
   ProcessParameter(options, "ignore_sss", AI_TYPE_BOOLEAN);

   MObject background;
   MPlugArray conns;
   MPlug pBG = GetFnNode().findPlug("background");
   pBG.connectedTo(conns, true, false);
   if (conns.length() == 1)
   {
      background = conns[0].node();
   }
   else
   {
      background = MObject::kNullObj;
   }


   // BACKGROUND SHADER
   //
   if (!background.isNull())
   {
      AiNodeSetPtr(options, "background", ExportShader(background));
   }


   // ATMOSPHERE SHADER
   //

   MSelectionList list;
   MObject        node;

   AtInt atmosphere = GetFnNode().findPlug("atmosphere").asInt();
   switch (atmosphere)
   {
   case 0:
      break;

   case 1:  // Fog
      list.add("defaultFogShader");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         AiNodeSetPtr(options, "atmosphere", ExportShader(node));
      }
      break;

   case 2:  // Volume Scattering
      list.add("defaultVolumeScatteringShader");
      if (list.length() > 0)
      {
         list.getDependNode(0, node);
         AiNodeSetPtr(options, "atmosphere", ExportShader(node));
      }
      break;
   }

   // Shadow stuff
   ProcessParameter(options, "shadow_terminator_fix", AI_TYPE_BOOLEAN);
}

