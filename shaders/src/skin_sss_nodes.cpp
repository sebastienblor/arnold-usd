#include "skin_sss.h"

//
// This is the standard shader turned into a method:
// void *CStandard::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
// and returning a CStandardOutput result
//

#define AI_SPECULAR_BRDF_STRETCHEDPHONG   0
#define AI_SPECULAR_BRDF_WARDDUER         1
#define AI_SPECULAR_BRDF_COOKTORRANCE     2

static const char* enum_specular_brdf[] = { "stretched_phong", "ward_duer", "cook_torrance", NULL };

typedef void* (*InitBrdfFunc)(const AtShaderGlobals *sg, const AtVector *U, const AtVector *V, float rx, float ry);
typedef AtColor (*IntegrateBrdfFunc)(void *brdf_data, AtShaderGlobals *sg);

struct BrdfMethods {
   BrdfMethods(InitBrdfFunc init, AtBRDFEvalBrdfFunc brdf, IntegrateBrdfFunc integrate) :
      init_func(init),
      sample_func(NULL),
      brdf_func(brdf),
      pdf_func(NULL),
      integrate_func(integrate) {}

   BrdfMethods(InitBrdfFunc init, AtBRDFEvalSampleFunc sample, AtBRDFEvalBrdfFunc brdf, AtBRDFEvalPdfFunc pdf, IntegrateBrdfFunc integrate) :
      init_func(init),
      sample_func(sample),
      brdf_func(brdf),
      pdf_func(pdf),
      integrate_func(integrate) {}

   InitBrdfFunc         init_func;
   AtBRDFEvalSampleFunc sample_func;
   AtBRDFEvalBrdfFunc   brdf_func;
   AtBRDFEvalPdfFunc    pdf_func;
   IntegrateBrdfFunc    integrate_func;
};



static inline float RoughnessToPhongExponent(const float r) { return 0.5f / (r*r) - 0.5f; }
static inline float PhongExponentToRoughness(const float e) { return sqrtf(0.5f / (e + 0.5f)); }

typedef struct {
   AtVector Normal, Ngf, View, U, V, R;
   float  pexp;
} StretchedPhongBrdf;

static void* StretchedPhongBrdfInit(const AtShaderGlobals *sg, const AtVector *U, const AtVector *V, float rx, float ry)
{
   return AiStretchedPhongMISCreateData(sg, RoughnessToPhongExponent(rx));
}
static AtColor StretchedPhongEvalIntegrate(void *brdf, AtShaderGlobals *sg)
{
   StretchedPhongBrdf *phong = (StretchedPhongBrdf*)brdf;
   return AiStretchedPhongIntegrate(&sg->Nf, sg, phong->pexp);
}
static BrdfMethods phong_methods(StretchedPhongBrdfInit, AiStretchedPhongMISSample, AiStretchedPhongMISBRDF, AiStretchedPhongMISPDF, StretchedPhongEvalIntegrate);

typedef struct {
   AtVector Normal, Ngf, View, U, V;
   float  rx, ry;
} WardDuerBrdf;

static void* WardDuerBrdfInit(const AtShaderGlobals *sg, const AtVector *U, const AtVector *V, float rx, float ry)
{
   return AiWardDuerMISCreateData(sg, U, V, rx, ry);
}
static AtColor WardDuerEvalIntegrate(void *brdf, AtShaderGlobals *sg)
{
   WardDuerBrdf *ward_duer = (WardDuerBrdf*)brdf;
   return AiWardDuerIntegrate(&sg->Nf, sg, &(ward_duer->U), &(ward_duer->V), ward_duer->rx, ward_duer->ry);
}
static BrdfMethods ward_duer_methods(WardDuerBrdfInit, AiWardDuerMISSample, AiWardDuerMISBRDF, AiWardDuerMISPDF, WardDuerEvalIntegrate);

typedef struct {
   AtVector Normal, Ngf, View, U, V;
   float  rx, rx_scaling;
} CookTorranceBrdf;

static void* CookTorranceBrdfInit(const AtShaderGlobals *sg, const AtVector *U, const AtVector *V, float rx, float ry)
{
   return AiCookTorranceMISCreateData(sg, U, V, rx, ry);
}
static AtColor CookTorranceEvalIntegrate(void *brdf, AtShaderGlobals *sg)
{
   CookTorranceBrdf *cook_torrance = (CookTorranceBrdf*)brdf;
   return AiCookTorranceIntegrate(&sg->Nf, sg, &(cook_torrance->U), &(cook_torrance->V), cook_torrance->rx, cook_torrance->rx);
}
static BrdfMethods cook_torrance_methods(CookTorranceBrdfInit, AiCookTorranceMISSample, AiCookTorranceMISBRDF, AiCookTorranceMISPDF, CookTorranceEvalIntegrate);

//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

BrdfMethods brdf_methods_array[3] = { phong_methods, ward_duer_methods, cook_torrance_methods };

// Here is the evaluator
void *CStandard::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   CStandardOutput *output = &outputs[sg->tid];

   output->opacity = params.opacity;

   float Ksss = params.Ksss;
   AtColor Ksss_color = params.Ksss_color;
   AtColor sss_radius = params.sss_radius;
   // evaluate textured params
   if (params.p_Ksss)
      Ksss = *(float*)params.p_Ksss->Evaluate(node, sg, options);
   if (params.p_Ksss_color)
      Ksss_color = *(AtColor*)params.p_Ksss_color->Evaluate(node, sg, options);
   if (params.p_sss_radius)
      sss_radius = *(float*)params.p_sss_radius->Evaluate(node, sg, options);
   
   // early out for shadow rays
   if (sg->Rt & AI_RAY_SHADOW)
   {
      float Kt = params.Kt;
      if (Kt > AI_EPSILON)
      {
         // approximate colored shadows from refractive objects
         AtColor Kt_color = params.Kt_color;
         AtColor shadow_opacity = 1 - (Kt_color * Kt);
         output->opacity *= shadow_opacity;

         // apply Beer's Law on interval between forward and back face intersections
         const AtColor transmittance = params.transmittance;
         AtColor transmittance_clamped = AiColorClamp(transmittance, 0, 1);
         if (!AiColorEqual(transmittance_clamped, AI_RGB_WHITE))
         {
            if (AiV3Dot(sg->Rd,sg->Ng) > 0)
            {
               AtPoint s_hit_previous;
               if (AiStateGetMsgPnt("s_hit_previous", &s_hit_previous))
               {
                  const float distance = AiV3Dist(sg->P, s_hit_previous);
                  const AtColor attenuation_coefficient = { logf(1 / transmittance_clamped.r),
                                                            logf(1 / transmittance_clamped.g),
                                                            logf(1 / transmittance_clamped.b) };
                  const AtColor attenuation = { expf(-attenuation_coefficient.r * distance),
                                                expf(-attenuation_coefficient.g * distance),
                                                expf(-attenuation_coefficient.b * distance) };
                  output->opacity = 1 - attenuation * (1 - output->opacity);
               }
            }
            else
            {
               AiStateSetMsgPnt("s_hit_previous", sg->P);
            }
         }
      }
      return (void*)output;
   }

   // early out for nearly transparent surfaces
   if (AiColorIsZero(output->opacity))
      return (void*)output;

   output->rgb = AI_RGB_BLACK;

   // emission
   float emission = params.emission;
   if (emission != 0.f)
   {
      AtColor emission_color = params.emission_color;
      output->rgb += emission * emission_color;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_emission), emission * emission_color);
   }

   // early out if bounce_factor is set to 0
   float bounce_coef = params.bounce_factor;
   if ((sg->Rt & (AI_RAY_DIFFUSE | AI_RAY_GLOSSY)) && bounce_coef == 0.f)
      return (void*)output;

   // setup Fresnel weights
   float refl_fresnel = 0;
   float spec_fresnel = 0;
   bool  Fresnel_on_diff = params.Fresnel_on_diff;
   bool  Fresnel = params.Fresnel;
   if (Fresnel)
      refl_fresnel = AiFresnelWeight(sg->Nf, sg->Rd, params.Krn);
   bool spec_Fresnel = params.spec_Fresnel;
   if (spec_Fresnel)
      spec_fresnel = AiFresnelWeight(sg->Nf, sg->Rd, params.Ksn);

   AtColor   Kd                     = params.Kd * params.Kd_color;
   AtColor   Ks                     = params.Ks * params.Ks_color;
   float     roughness              = params.roughness;
   bool      enable_glossy_caustics = params.enable_glossy_caustics;
   float     pexp                   = params.pexp;
   pexp = (pexp < 0) ? 0 : pexp;

   // compute specular roughness
   const int     specular_brdf       = params.specular_brdf;
   const float   specular_roughness  = (specular_brdf == AI_SPECULAR_BRDF_STRETCHEDPHONG && pexp != 10.f) ?
                                          sqrtf(PhongExponentToRoughness(pexp)) :
                                          params.specular_roughness;
   const float specular_roughness_clamped = CLAMP(specular_roughness, 0.0f, 1.0f);
   const float specular_roughness_squared = SQR(specular_roughness_clamped);

   // setup anisotropic specular 
   float specular_rx, specular_ry;
   AtVector rot_U, rot_V;
   if (specular_brdf == AI_SPECULAR_BRDF_WARDDUER)  // or any other anisotropic BRDFs
   {
      const float specular_anisotropy = params.specular_anisotropy;
      const float specular_anisotropy_clamped = CLAMP(specular_anisotropy, 0.0f, 1.0f);
      const float specular_t = sqrtf(fabsf(2 * specular_anisotropy_clamped - 1));  // interpolation param, in [0,1]
      specular_rx = (specular_anisotropy >= 0.5f) ? specular_roughness_squared : LERP(specular_t, specular_roughness_squared, 0.0f);
      specular_ry = (specular_anisotropy <= 0.5f) ? specular_roughness_squared : LERP(specular_t, specular_roughness_squared, 0.0f);

      AtVector U, V;
      if (!AiV3IsZero(sg->dPdu) && !AiV3IsZero(sg->dPdv))
      {
         // tangents available, use them
         U = sg->dPdu;
         V = sg->dPdv;
      }
      else
      {
         // no tangents given, compute a pair
         AiBuildLocalFramePolar(&U, &V, &sg->Nf);
      }

      // rotate tangents
      const float rot_angle = params.specular_rotation;
      const float cos_rot_angle = cosf(rot_angle * (float)AI_PI);
      const float sin_rot_angle = sqrtf(1 - SQR(cos_rot_angle));
      rot_U = cos_rot_angle * U - sin_rot_angle * V;
      rot_V = cos_rot_angle * V + sin_rot_angle * U;
   }
   else
   {
      specular_rx = specular_ry = specular_roughness_squared;
   }

   // reduce reflective caustic noise by clamping the minimum roughness for GI rays
   if (sg->Rr_diff > 0)
   {
      // after a diffuse bounce, clamp by a constant
      const float min_r = 0.1f;
      specular_rx = MAX(min_r, specular_rx);
      specular_ry = MAX(min_r, specular_ry);
   }
   else if (sg->Rr_gloss > 0)
   {
      // after a specular bounce clamp in proportion to its roughness (scaled by a "sharpness" coefficient)
      float prev_r = 0;
      AiStateGetMsgFlt("previous_roughness", &prev_r);
      const float min_r = 0.9f * prev_r;
      specular_rx = MAX(min_r, specular_rx);
      specular_ry = MAX(min_r, specular_ry);
   }

   // setup specular BRDF
   const BrdfMethods &brdf_methods = brdf_methods_array[specular_brdf];
   void *brdf_data = brdf_methods.init_func(sg, &rot_U, &rot_V, specular_rx, specular_ry);

   //
   // Direct diffuse layer
   //
   if (!AiColorIsZero(Kd))
   {
      float Kb = params.Kb;
      if (Kb > 0)
         sg->fhemi = false;

      void *oren_nayar_brdf = AiOrenNayarMISCreateData(sg, roughness);

      AtVector V = -sg->Rd;
      AtColor direct_diffuse = AI_RGB_BLACK;
      AiLightsPrepare(sg);
      while (AiLightsGetSample(sg))
      {
         if (AiLightGetAffectDiffuse(sg->Lp))
            direct_diffuse += AiEvaluateLightSample(sg, oren_nayar_brdf, AiOrenNayarMISSample, AiOrenNayarMISBRDF, AiOrenNayarMISPDF);
      }

      // Evaluate translucent effects
      if (Kb > 0)
      {
         sg->Nf  = -sg->Nf;
         sg->Ngf = -sg->Ngf;

         void *oren_nayar_brdf = AiOrenNayarMISCreateData(sg, roughness);

         AiLightsPrepare(sg);
         while (AiLightsGetSample(sg))
         {
            if (AiLightGetAffectDiffuse(sg->Lp))
               direct_diffuse += Kb * AiEvaluateLightSample(sg, oren_nayar_brdf, AiOrenNayarMISSample, AiOrenNayarMISBRDF, AiOrenNayarMISPDF);
         }
         sg->Nf  = -sg->Nf;
         sg->Ngf = -sg->Ngf;
      }

      direct_diffuse *= Kd * params.direct_diffuse;
      if (Fresnel_on_diff)
         direct_diffuse *= 1 - refl_fresnel - spec_fresnel;
      output->rgb += direct_diffuse;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_direct_diffuse), direct_diffuse);
   }

   //
   // Direct specular layer (with MIS)
   //
   if (!AiColorIsZero(Ks) && (sg->Rr_diff == 0 || enable_glossy_caustics))
   {
      AtColor direct_specular = AI_RGB_BLACK;
      AiLightsPrepare(sg);
      while (AiLightsGetSample(sg))
      {
         if (AiLightGetAffectSpecular(sg->Lp))
            direct_specular += AiEvaluateLightSample(sg, brdf_data, brdf_methods.sample_func, brdf_methods.brdf_func, brdf_methods.pdf_func);
      }

      direct_specular *= Ks * params.direct_specular;
      if (spec_Fresnel)
         direct_specular *= spec_fresnel;
      output->rgb += direct_specular;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_direct_specular), direct_specular);
   }

   //
   // Indirect diffuse layer
   //
   AtColor Kd_indirect = Kd * params.indirect_diffuse;
   if (!AiColorIsZero(Kd_indirect))
   {
      AtColor indirect_diffuse;
      if (roughness > 0)
         indirect_diffuse = AiOrenNayarIntegrate(&sg->Nf, sg, roughness);
      else
         indirect_diffuse = AiIndirectDiffuse(&sg->Nf, sg);

      // FIXME: Kb is later multiplied by Kd, so they can't be set independently?
      float Kb = params.Kb;
      if (Kb > AI_EPSILON)
      {
         sg->Nf  = -sg->Nf;
         sg->Ngf = -sg->Ngf;
         indirect_diffuse += AiIndirectDiffuse(&sg->Nf, sg) * Kb;
         sg->Nf  = -sg->Nf;
         sg->Ngf = -sg->Ngf;
      }

      indirect_diffuse *= Kd_indirect;
      if (Fresnel_on_diff)
         indirect_diffuse *= 1 - refl_fresnel - spec_fresnel;
      output->rgb += indirect_diffuse;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_indirect_diffuse), indirect_diffuse);
   }

   //
   // Indirect specular layer
   //
   bool enable_internal_reflections = params.enable_internal_reflections;
   AtColor Ks_indirect = Ks * params.indirect_specular;
   if (!AiColorIsZero(Ks_indirect) && (sg->Rr_diff == 0 || enable_glossy_caustics)
                                   && (sg->Rr_refr == 0 || enable_internal_reflections))
   {
      AiStateSetMsgFlt("previous_roughness", MIN(specular_rx, specular_ry)); // propagate roughness for clamping - FIXME: should be max so far?
      AtColor indirect_specular = Ks_indirect * brdf_methods.integrate_func(brdf_data, sg);
      if (spec_Fresnel)
         indirect_specular *= spec_fresnel;
      output->rgb += indirect_specular;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_indirect_specular), indirect_specular);
   }

   //
   // Mirror reflection layer
   //
   AtColor Kr = params.Kr * params.Kr_color;
   if (!AiColorIsZero(Kr) && (sg->Rr_diff == 0 || params.enable_reflective_caustics)
                          && (sg->Rr_refr == 0 || enable_internal_reflections))
   {
      AtVector R;
      AiReflectSafe(&sg->Rd, &sg->Nf, &sg->Ng, &R);  // AiReflect() caused artifacts

      AtRay ray;
      AiMakeRay(&ray, AI_RAY_REFLECTED, &sg->P, &R, AI_BIG, sg);

      AtColor reflection;
      if (sg->Rr_refl < options.GI_reflection_depth)
      {
         AtScrSample sample;
         AiTrace(&ray, &sample);
         reflection = sample.color;
      }
      else
      {
         reflection = params.reflection_exit_color;
         if (params.reflection_exit_use_environment)
         {
            AtScrSample sample;
            AiTraceBackground(&ray, &sample);
            reflection *= sample.color;
         }
      }

      reflection *= Kr;
      if (Fresnel)
         reflection *= refl_fresnel;
      output->rgb += reflection;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_reflection), reflection);
   }

   //
   // Refraction layer
   //
   AtColor Kt = params.Kt * params.Kt_color;
   if (!AiColorIsZero(Kt) && (sg->Rr_diff == 0 || params.enable_refractive_caustics))
   {
      /*
       * How do we know if we are entering or exiting an object, and what 
       * IOR to use? i know of 3 methods:
       *    a) assume outward facing normals, objects embedded in air, no overlap
       *    b) store IOR of material the ray is traveling in, no overlap
       *    c) store stack of IOR's in the ray, objects can overlap
       * Method a) is the simplest to implement
       */
      float IOR = params.IOR;
      float n1, n2;
      if (AiV3Dot(sg->N, sg->Rd) < 0.f)
      {
         n1 = 1.f;
         n2 = IOR;
      }
      else
      {
         n1 = IOR;
         n2 = 1.f;
      }

      AtColor refraction;
      const float roughness = params.refraction_roughness;
      if (roughness > 0.f)
      {
         const float roughness_clamped = CLAMP(roughness, 0.0f, 1.0f);
         const float roughness_squared = SQR(roughness_clamped);
         const AtColor transmittance = params.transmittance;
         refraction = AiMicrofacetBTDFIntegrate(&(sg->Nf), sg, NULL, NULL, roughness_squared, roughness_squared, n1, n2, transmittance);
      }
      else
      {
         AtRay ray;
         AtVector T;
         if (AiRefract(&sg->Rd, &sg->Nf, &T, n1, n2))
         {
            AiMakeRay(&ray, AI_RAY_REFRACTED, &sg->P, &T, AI_BIG, sg);
         }
         else
         {
            AtVector R;
            AiReflectSafe(&sg->Rd, &sg->Nf, &sg->Ng, &R);
            AiMakeRay(&ray, AI_RAY_REFRACTED, &sg->P, &R, AI_BIG, sg);  // TIR
         }

         AtScrSample sample;
         if (sg->Rr_refr < options.GI_refraction_depth)
         {
            AiTrace(&ray, &sample);
            refraction = sample.color;
         }
         else
         {
            refraction = params.refraction_exit_color;
            if (params.refraction_exit_use_environment)
            {
               AiTraceBackground(&ray, &sample);
               refraction *= sample.color;
            }
         }

         // apply Beer's Law to forward-facing surfaces in the interval to sample.z
         if (sample.z > 0.f)
         {
            const AtColor transmittance = params.transmittance;
            AtColor transmittance_clamped = AiColorClamp(transmittance, 0, 1);
            if (!AiColorEqual(transmittance_clamped, AI_RGB_WHITE) && AiV3Dot(sg->Rd,sg->Ng) <= 0)
            {
               const AtColor attenuation_coefficient = { logf(1 / transmittance_clamped.r),
                                                         logf(1 / transmittance_clamped.g),
                                                         logf(1 / transmittance_clamped.b) };
               const AtColor attenuation = { expf(-attenuation_coefficient.r * (float)sample.z),
                                             expf(-attenuation_coefficient.g * (float)sample.z),
                                             expf(-attenuation_coefficient.b * (float)sample.z) };
               refraction *= attenuation;
            }
         }
      }

      refraction *= Kt * (1 - refl_fresnel - spec_fresnel);

      output->rgb += refraction;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_refraction), refraction);
   }

   //
   // Sub-surface scattering (SSS) layer
   //
   AtColor cKsss = Ksss * Ksss_color;
   if (!AiColorIsZero(cKsss))
   {
      AtColor csss_radius = sss_radius;
      AtColor sss = cKsss * AiSSSPointCloudLookupCubic(sg, csss_radius);
      if (Fresnel_on_diff)
         sss *= 1 - refl_fresnel - spec_fresnel;
      output->rgb += sss;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_sss), sss);
   }

   // artificially pump up the bounces
   if (sg->Rt & (AI_RAY_DIFFUSE | AI_RAY_GLOSSY))
      output->rgb *= bounce_coef;

   return (void*)output;
}



/////////////
/////////////
// RAY SWITCH 
/////////////
/////////////

// (All the others classes work the same way).
void* CRaySwitch::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   // Store the result in the thread-th slot.
   // If we had a single output result, threads would write at the same time in it.
   AtColor *output = &outputs[sg->tid];
   switch (sg->Rt)
   {
      case AI_RAY_CAMERA:
         if (params.p_camera) // camera has an input connection? The evaluate the connection
            *output = *(AtColor*)params.p_camera->Evaluate(node, sg, options);
         else
            *output = params.camera; // elsem return the plain camera value
         break;
      case AI_RAY_SHADOW:     
         if (params.p_shadow)
            *output = *(AtColor*)params.p_shadow->Evaluate(node, sg, options);
         else
            *output = params.shadow;      
         break;
      case AI_RAY_REFLECTED:  
         if (params.p_reflection)
            *output = *(AtColor*)params.p_reflection->Evaluate(node, sg, options);
         else
            *output = params.reflection;      
         break;
      case AI_RAY_REFRACTED:  
         if (params.p_refraction)
            *output = *(AtColor*)params.p_refraction->Evaluate(node, sg, options);
         else
            *output = params.refraction;      
         break;
      case AI_RAY_DIFFUSE:    
         if (params.p_diffuse)
            *output = *(AtColor*)params.p_diffuse->Evaluate(node, sg, options);
         else
            *output = params.diffuse;      
         break;
      case AI_RAY_GLOSSY:     
         if (params.p_glossy)
            *output = *(AtColor*)params.p_glossy->Evaluate(node, sg, options);
         else
            *output = params.glossy;      
         break;
      default:
         *output = AI_RGB_BLACK;
         break;
   }

   // All the node classes return a void* as their result
   return (void*)output;
}


// COLOR SWITCH 
void* CColorSwitch::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];

   if (params.switcher)
   {
      if (params.p_input2)
         *output = *(AtColor*)params.p_input2->Evaluate(node, sg, options);
      else
         *output = params.input2;
   }
   else
   {
      if (params.p_input1)
         *output = *(AtColor*)params.p_input1->Evaluate(node, sg, options);
      else
         *output = params.input1;
   }

   return (void*)output;
}


// SCALAR 2 COLOR
void* CScalarToColor::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];

   // Never overwrite params.input
   // So, never use params.input in place of input below, else the threads would overwrite it
   AtFloat input; 
   if (params.p_input)
      input = *(AtFloat*)params.p_input->Evaluate(node, sg, options);
   else
      input = params.input;

   output->r = input;
   output->g = input;
   output->b = input;
   // output.a = params.alpha;
   return (void*)output;
}


// SCALAR MULTIPLY
void* CScalarMultiply::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtFloat *output = &outputs[sg->tid];

   AtFloat input1, input2;

   if (params.p_input1)
      input1 = *(AtFloat*)params.p_input1->Evaluate(node, sg, options);
   else
      input1 = params.input1;

   if (params.p_input2)
      input2 = *(AtFloat*)params.p_input2->Evaluate(node, sg, options);
   else
      input2 = params.input2;

   *output = input1 * input2;    
   return (void*)output;
}


// SCALAR ADD
void* CScalarAdd::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtFloat *output = &outputs[sg->tid];

   AtFloat input1, input2;

   if (params.p_input1)
      input1 = *(AtFloat*)params.p_input1->Evaluate(node, sg, options);
   else
      input1 = params.input1;

   if (params.p_input2)
      input2 = *(AtFloat*)params.p_input2->Evaluate(node, sg, options);
   else
      input2 = params.input2;

   *output = input1 + input2;    
   return (void*)output;
}


// COLORS ADD
void* CColorsAdd::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];

   AtColor baseColor, color1, color2, color3, color4;

   if (params.p_baseColor)
      baseColor = *(AtColor*)params.p_baseColor->Evaluate(node, sg, options);
   else
      baseColor = params.baseColor;

   if (params.p_color1)
      color1 = *(AtColor*)params.p_color1->Evaluate(node, sg, options);
   else
      color1 = params.color1;

   if (params.p_color2)
      color2 = *(AtColor*)params.p_color2->Evaluate(node, sg, options);
   else
      color2 = params.color2;

   if (params.p_color3)
      color3 = *(AtColor*)params.p_color3->Evaluate(node, sg, options);
   else
      color3 = params.color3;

   if (params.p_color4)
      color4 = *(AtColor*)params.p_color4->Evaluate(node, sg, options);
   else
      color4 = params.color4;

   AtColor c12 = color1 + color2;    
   AtColor c34 = color3 + color4;
   *output = baseColor + c12;
   *output+= c34;
   return (void*)output;
}


// COLORS ADD BOUND
#define ColorMixAddBound(_r, _b, _c)                \
{ (_r).r = (AtFloat) CLAMP(((_b).r + (_c).r), 0.0, 1.0);     \
  (_r).g = (AtFloat) CLAMP(((_b).g + (_c).g), 0.0, 1.0);     \
  (_r).b = (AtFloat) CLAMP(((_b).b + (_c).b), 0.0, 1.0);     }

void* CColorClip::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];

   AtColor color;

   if (params.p_color)
      color = *(AtColor*)params.p_color->Evaluate(node, sg, options);
   else
      color = params.color;

   output->r = CLAMP(color.r, 0.0f, 1.0f);
   output->g = CLAMP(color.g, 0.0f, 1.0f);
   output->b = CLAMP(color.b, 0.0f, 1.0f);
   return (void*)output;
}


// COLORS SCREEN
#define ColorMixScreen(_r, _b, _c)                  \
{ (_r).r = (AtFloat)(1.0 - ((1.0-(_b).r) * (1.0-(_c).r)));    \
  (_r).g = (AtFloat)(1.0 - ((1.0-(_b).g) * (1.0-(_c).g)));    \
  (_r).b = (AtFloat)(1.0 - ((1.0-(_b).b) * (1.0-(_c).b)));    }

void* CColorsScreen::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];

   AtColor baseColor, color1, color2, color3, color4;

   if (params.p_baseColor)
      baseColor = *(AtColor*)params.p_baseColor->Evaluate(node, sg, options);
   else
      baseColor = params.baseColor;

   if (params.p_color1)
      color1 = *(AtColor*)params.p_color1->Evaluate(node, sg, options);
   else
      color1 = params.color1;

   if (params.p_color2)
      color2 = *(AtColor*)params.p_color2->Evaluate(node, sg, options);
   else
      color2 = params.color2;

   if (params.p_color3)
      color3 = *(AtColor*)params.p_color3->Evaluate(node, sg, options);
   else
      color3 = params.color3;

   if (params.p_color4)
      color4 = *(AtColor*)params.p_color4->Evaluate(node, sg, options);
   else
      color4 = params.color4;

   ColorMixScreen(*output, baseColor, color1);
   ColorMixScreen(*output, *output, color2);
   ColorMixScreen(*output, *output, color3);
   ColorMixScreen(*output, *output, color4);
   return (void*)output;
}


// COLORS DIVIDE
void* CColorsDivide::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];

   AtColor color1, color2;

   if (params.p_color1)
      color1 = *(AtColor*)params.p_color1->Evaluate(node, sg, options);
   else
      color1 = params.color1;

   if (params.p_color2)
      color2 = *(AtColor*)params.p_color2->Evaluate(node, sg, options);
   else
      color2 = params.color2;

   *output = color1 / color2;
   return (void*)output;
}
