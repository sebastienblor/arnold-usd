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
   CStandardParams *pParams = &params[sg->tid];

   output->opacity = pParams->opacity;

   float Ksss = pParams->Ksss;
   AtColor Ksss_color = pParams->Ksss_color;
   AtColor sss_radius = pParams->sss_radius;
   // evaluate textured params
   if (p_Ksss)
      Ksss = *(float*)p_Ksss->Evaluate(node, sg, options);
   if (p_Ksss_color)
      Ksss_color = *(AtColor*)p_Ksss_color->Evaluate(node, sg, options);
   if (p_sss_radius)
      sss_radius = *(float*)p_sss_radius->Evaluate(node, sg, options);
   
   // early out for shadow rays
   if (sg->Rt & AI_RAY_SHADOW)
   {
      float Kt = pParams->Kt;
      if (Kt > AI_EPSILON)
      {
         // approximate colored shadows from refractive objects
         AtColor Kt_color = pParams->Kt_color;
         AtColor shadow_opacity = 1 - (Kt_color * Kt);
         output->opacity *= shadow_opacity;

         // apply Beer's Law on interval between forward and back face intersections
         const AtColor transmittance = pParams->transmittance;
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
   float emission = pParams->emission;
   if (emission != 0.f)
   {
      AtColor emission_color = pParams->emission_color;
      output->rgb += emission * emission_color;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_emission), emission * emission_color);
   }

   // early out if bounce_factor is set to 0
   float bounce_coef = pParams->bounce_factor;
   if ((sg->Rt & (AI_RAY_DIFFUSE | AI_RAY_GLOSSY)) && bounce_coef == 0.f)
      return (void*)output;

   // setup Fresnel weights
   float refl_fresnel = 0;
   float spec_fresnel = 0;
   bool  Fresnel_on_diff = pParams->Fresnel_on_diff;
   bool  Fresnel = pParams->Fresnel;
   if (Fresnel)
      refl_fresnel = AiFresnelWeight(sg->Nf, sg->Rd, pParams->Krn);
   bool spec_Fresnel = pParams->spec_Fresnel;
   if (spec_Fresnel)
      spec_fresnel = AiFresnelWeight(sg->Nf, sg->Rd, pParams->Ksn);

   AtColor   Kd                     = pParams->Kd * pParams->Kd_color;
   AtColor   Ks                     = pParams->Ks * pParams->Ks_color;
   float     roughness              = pParams->roughness;
   bool      enable_glossy_caustics = pParams->enable_glossy_caustics;
   float     pexp                   = pParams->pexp;
   pexp = (pexp < 0) ? 0 : pexp;

   // compute specular roughness
   const int     specular_brdf       = pParams->specular_brdf;
   const float   specular_roughness  = (specular_brdf == AI_SPECULAR_BRDF_STRETCHEDPHONG && pexp != 10.f) ?
                                          sqrtf(PhongExponentToRoughness(pexp)) :
                                          pParams->specular_roughness;
   const float specular_roughness_clamped = CLAMP(specular_roughness, 0.0f, 1.0f);
   const float specular_roughness_squared = SQR(specular_roughness_clamped);

   // setup anisotropic specular 
   float specular_rx, specular_ry;
   AtVector rot_U, rot_V;
   if (specular_brdf == AI_SPECULAR_BRDF_WARDDUER)  // or any other anisotropic BRDFs
   {
      const float specular_anisotropy = pParams->specular_anisotropy;
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
      const float rot_angle = pParams->specular_rotation;
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
      float Kb = pParams->Kb;
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

      direct_diffuse *= Kd * pParams->direct_diffuse;
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

      direct_specular *= Ks * pParams->direct_specular;
      if (spec_Fresnel)
         direct_specular *= spec_fresnel;
      output->rgb += direct_specular;
      // stefano
      // AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_direct_specular), direct_specular);
   }

   //
   // Indirect diffuse layer
   //
   AtColor Kd_indirect = Kd * pParams->indirect_diffuse;
   if (!AiColorIsZero(Kd_indirect))
   {
      AtColor indirect_diffuse;
      if (roughness > 0)
         indirect_diffuse = AiOrenNayarIntegrate(&sg->Nf, sg, roughness);
      else
         indirect_diffuse = AiIndirectDiffuse(&sg->Nf, sg);

      // FIXME: Kb is later multiplied by Kd, so they can't be set independently?
      float Kb = pParams->Kb;
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
   bool enable_internal_reflections = pParams->enable_internal_reflections;
   AtColor Ks_indirect = Ks * pParams->indirect_specular;
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
   AtColor Kr = pParams->Kr * pParams->Kr_color;
   if (!AiColorIsZero(Kr) && (sg->Rr_diff == 0 || pParams->enable_reflective_caustics)
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
         reflection = pParams->reflection_exit_color;
         if (pParams->reflection_exit_use_environment)
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
   AtColor Kt = pParams->Kt * pParams->Kt_color;
   if (!AiColorIsZero(Kt) && (sg->Rr_diff == 0 || pParams->enable_refractive_caustics))
   {
      /*
       * How do we know if we are entering or exiting an object, and what 
       * IOR to use? i know of 3 methods:
       *    a) assume outward facing normals, objects embedded in air, no overlap
       *    b) store IOR of material the ray is traveling in, no overlap
       *    c) store stack of IOR's in the ray, objects can overlap
       * Method a) is the simplest to implement
       */
      float IOR = pParams->IOR;
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
      const float roughness = pParams->refraction_roughness;
      if (roughness > 0.f)
      {
         const float roughness_clamped = CLAMP(roughness, 0.0f, 1.0f);
         const float roughness_squared = SQR(roughness_clamped);
         const AtColor transmittance = pParams->transmittance;
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
            refraction = pParams->refraction_exit_color;
            if (pParams->refraction_exit_use_environment)
            {
               AiTraceBackground(&ray, &sample);
               refraction *= sample.color;
            }
         }

         // apply Beer's Law to forward-facing surfaces in the interval to sample.z
         if (sample.z > 0.f)
         {
            const AtColor transmittance = pParams->transmittance;
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
   CRaySwitchParams *pParams = &params[sg->tid];

   switch (sg->Rt)
   {
      case AI_RAY_CAMERA:
         if (p_camera) // camera has an input connection? The evaluate the connection
            *output = *(AtColor*)p_camera->Evaluate(node, sg, options);
         else
            *output = pParams->camera; // elsem return the plain camera value
         break;
      case AI_RAY_SHADOW:     
         if (p_shadow)
            *output = *(AtColor*)p_shadow->Evaluate(node, sg, options);
         else
            *output = pParams->shadow;      
         break;
      case AI_RAY_REFLECTED:  
         if (p_reflection)
            *output = *(AtColor*)p_reflection->Evaluate(node, sg, options);
         else
            *output = pParams->reflection;      
         break;
      case AI_RAY_REFRACTED:  
         if (p_refraction)
            *output = *(AtColor*)p_refraction->Evaluate(node, sg, options);
         else
            *output = pParams->refraction;      
         break;
      case AI_RAY_DIFFUSE:    
         if (p_diffuse)
            *output = *(AtColor*)p_diffuse->Evaluate(node, sg, options);
         else
            *output = pParams->diffuse;      
         break;
      case AI_RAY_GLOSSY:     
         if (p_glossy)
            *output = *(AtColor*)p_glossy->Evaluate(node, sg, options);
         else
            *output = pParams->glossy;      
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
   CColorSwitchParams *pParams = &params[sg->tid];

   if (pParams->switcher)
   {
      if (p_input2)
         *output = *(AtColor*)p_input2->Evaluate(node, sg, options);
      else
         *output = pParams->input2;
   }
   else
   {
      if (p_input1)
         *output = *(AtColor*)p_input1->Evaluate(node, sg, options);
      else
         *output = pParams->input1;
   }

   return (void*)output;
}


// SCALAR 2 COLOR
void* CScalarToColor::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];
   CScalarToColorParams *pParams = &params[sg->tid];

   // Never overwrite params.input
   // So, never use params.input in place of input below, else the threads would overwrite it
   AtFloat input; 
   if (p_input)
      input = *(AtFloat*)p_input->Evaluate(node, sg, options);
   else
      input = pParams->input;

   output->r = input;
   output->g = input;
   output->b = input;

   return (void*)output;
}


// SCALAR MULTIPLY
void* CScalarMultiply::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtFloat *output = &outputs[sg->tid];
   CScalarMultiplyParams *pParams = &params[sg->tid];

   AtFloat input1, input2;

   if (p_input1)
      input1 = *(AtFloat*)p_input1->Evaluate(node, sg, options);
   else
      input1 = pParams->input1;

   if (p_input2)
      input2 = *(AtFloat*)p_input2->Evaluate(node, sg, options);
   else
      input2 = pParams->input2;

   *output = input1 * input2;    
   return (void*)output;
}


// SCALAR ADD
void* CScalarAdd::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtFloat *output = &outputs[sg->tid];
   CScalarAddParams *pParams = &params[sg->tid];

   AtFloat input1, input2;

   if (p_input1)
      input1 = *(AtFloat*)p_input1->Evaluate(node, sg, options);
   else
      input1 = pParams->input1;

   if (p_input2)
      input2 = *(AtFloat*)p_input2->Evaluate(node, sg, options);
   else
      input2 = pParams->input2;

   *output = input1 + input2;    
   return (void*)output;
}


// COLORS ADD
void* CColorsAdd::Evaluate(AtNode *node, AtShaderGlobals *sg, const COptions options)
{
   AtColor *output = &outputs[sg->tid];
   CColorsAddParams *pParams = &params[sg->tid];

   AtColor baseColor, color1, color2, color3, color4;

   if (p_baseColor)
      baseColor = *(AtColor*)p_baseColor->Evaluate(node, sg, options);
   else
      baseColor = pParams->baseColor;

   if (p_color1)
      color1 = *(AtColor*)p_color1->Evaluate(node, sg, options);
   else
      color1 = pParams->color1;

   if (p_color2)
      color2 = *(AtColor*)p_color2->Evaluate(node, sg, options);
   else
      color2 = pParams->color2;

   if (p_color3)
      color3 = *(AtColor*)p_color3->Evaluate(node, sg, options);
   else
      color3 = pParams->color3;

   if (p_color4)
      color4 = *(AtColor*)p_color4->Evaluate(node, sg, options);
   else
      color4 = pParams->color4;

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
   CColorClipParams *pParams = &params[sg->tid];

   AtColor color;

   if (p_color)
      color = *(AtColor*)p_color->Evaluate(node, sg, options);
   else
      color = pParams->color;

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
   CColorsScreenParams *pParams = &params[sg->tid];

   AtColor baseColor, color1, color2, color3, color4;

   if (p_baseColor)
      baseColor = *(AtColor*)p_baseColor->Evaluate(node, sg, options);
   else
      baseColor = pParams->baseColor;

   if (p_color1)
      color1 = *(AtColor*)p_color1->Evaluate(node, sg, options);
   else
      color1 = pParams->color1;

   if (p_color2)
      color2 = *(AtColor*)p_color2->Evaluate(node, sg, options);
   else
      color2 = pParams->color2;

   if (p_color3)
      color3 = *(AtColor*)p_color3->Evaluate(node, sg, options);
   else
      color3 = pParams->color3;

   if (p_color4)
      color4 = *(AtColor*)p_color4->Evaluate(node, sg, options);
   else
      color4 = pParams->color4;

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
   CColorsDivideParams *pParams = &params[sg->tid];

   AtColor color1, color2;

   if (p_color1)
      color1 = *(AtColor*)p_color1->Evaluate(node, sg, options);
   else
      color1 = pParams->color1;

   if (p_color2)
      color2 = *(AtColor*)p_color2->Evaluate(node, sg, options);
   else
      color2 = pParams->color2;

   *output = color1 / color2;
   return (void*)output;
}
