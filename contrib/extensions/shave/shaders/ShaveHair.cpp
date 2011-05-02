//
// Simple hair shader, roughly based on Kay/Kajiya's shading model
//

#include <ai.h>

#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(ShaveHairMtd);

enum ShaveHairParams
{
   p_rootcolor,
   p_tipcolor,
   p_gamma,
   p_strand_opacity,
   p_ambdiff,
   p_ambient,
   p_gloss,
   p_spec_color,
   p_spec,
   p_kd_ind,
   p_uparam,
   p_vparam,
   p_direct_diffuse,
   p_indirect_diffuse,
   p_diffuse_cache
};

node_parameters
{
   AiParameterSTR( "rootcolor"        , NULL             );
   AiParameterSTR( "tipcolor"         , NULL             );
   AiParameterFLT( "gamma"            , 0.45f            );
   AiParameterRGB( "strand_opacity"   , 1.0f, 1.0f, 1.0f );
   AiParameterFLT( "ambdiff"          , 1.0f             );
   AiParameterRGB( "ambient"          , 1.0f, 1.0f, 1.0f );
   AiParameterFLT( "gloss"            , 10.0f            );
   AiParameterRGB( "spec_color"       , 1.0f, 1.0f, 1.0f );
   AiParameterFLT( "spec"             , 1.0f             );
   AiParameterFLT( "kd_ind"           , 1.0f             );
   AiParameterSTR( "uparam"           , NULL             );
   AiParameterSTR( "vparam"           , NULL             );
   AiParameterFLT( "direct_diffuse"   , 1.0f             );
   AiParameterFLT( "indirect_diffuse" , 1.0f             );
   AiParameterBOOL("diffuse_cache"    , TRUE             );

   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
}

typedef struct
{
   AtFloat              gamma;
   AtInt                max_diffuse_depth;
}
ShaderData;

node_initialize
{
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
   node->local_data = data;
}

node_update
{
   ShaderData *data = (ShaderData*)node->local_data;
   AtNode *options = AiUniverseGetOptions();
   data->max_diffuse_depth = AiNodeGetInt(options, "GI_diffuse_depth");
   data->gamma = 1.0f / AiNodeGetFlt(options, "shader_gamma");
}

node_finish
{
   ShaderData *data = (ShaderData*)node->local_data;
   AiFree(data);
}

shader_evaluate
{
   // mix root and tip colors
   AtColor opacity_color;
   AtColor opacity = AiShaderEvalParamRGB(p_strand_opacity);

   AiColorLerp(opacity_color, sg->v, opacity, opacity);

   sg->out_opacity = opacity_color;

   // This piece of user-data is automatically set by the curves node when
   // using auto-enlargement (min_pixel_width > 0)
   AtFloat geo_opacity;
   if (AiUDataGetFlt("geo_opacity", &geo_opacity))
      sg->out_opacity *= geo_opacity;

   // early out for shadow rays and totally transparent objects
   if ((sg->Rt & AI_RAY_SHADOW) || AiColorIsZero(sg->out_opacity))
      return;

   AtVector V = -sg->Rd;
   AtVector T;
   AiV3Normalize(T, sg->dPdv);

   AtColor Cdiff = AI_RGB_BLACK;
   AtColor Cspec = AI_RGB_BLACK;

   // change the current (u,v) position according to the specified userdata channels
   AtFloat oldU = sg->u;
   AtFloat oldV = sg->v;

   AiUDataGetFlt(node->params[p_uparam].STR, &(sg->u));
   AiUDataGetFlt(node->params[p_vparam].STR, &(sg->v));
   //AtFloat getGamma   = AiShaderEvalParamFlt(p_gamma);
   AtFloat ambdiff    = AiShaderEvalParamFlt(p_ambdiff);
   AtFloat gloss      = AiShaderEvalParamFlt(p_gloss);
   AtFloat spec       = AiShaderEvalParamFlt(p_spec);

   AtFloat direct_c   = AiShaderEvalParamFlt(p_direct_diffuse);
   AtFloat indirect_c = AiShaderEvalParamFlt(p_indirect_diffuse);

   AtColor spec_color = AiShaderEvalParamRGB(p_spec_color);
   AtColor root_color;
   AtColor tip_color;

   ShaderData *data = (ShaderData*)node->local_data;

   // FIXME: we need to gamma correct according to global settings
   AiUDataGetRGB(node->params[p_rootcolor].STR, &root_color);
   AiUDataGetRGB(node->params[p_tipcolor].STR, &tip_color);

   //AiColorGamma(&root_color, getGamma);
   //AiColorGamma(&tip_color, getGamma);

   AiColorGamma(&root_color, data->gamma);
   AiColorGamma(&tip_color, data->gamma);

   // restore original (u,v)
   sg->u = oldU;
   sg->v = oldV;

   // mix root and tip colors
   AtColor diff_color;
   AiColorLerp(diff_color, sg->v, root_color, tip_color);

   if (AiShaderEvalParamBool(p_diffuse_cache) && (sg->Rt & AI_RAY_DIFFUSE))
   {
      // quick viz
      Cdiff = AiHairDirectDiffuseCache(sg);
      sg->out.RGB = Cdiff * diff_color;
      return;
   }

   // Since the curves are represented as ray-facing ribbons
   // their normal is usually pointing roughly towards the incoming ray
   // - but not always. It can have discontinuities - which then causes
   // some lights to be ignored if they happen to be on the wrong side of this
   // normal. Setting sg->fhemi forces the lights to be gathered regardless of
   // where the normal is pointing
   sg->fhemi = FALSE;

   // direct lighting
   AiLightsPrepare(sg);
   while (AiLightsGetSample(sg))
   {
      if (AiLightGetAffectDiffuse(sg->Lp))
      {
         AtFloat TdotL = (float)AiV3Dot(T, sg->Ld);
         AtFloat d = 1 - TdotL * TdotL;
         d = d > 0 ? sqrtf(d) : 0;
         AtFloat diffterm = (1 - ambdiff) + d * ambdiff;  // limits gamut of diffuse term
         // diffuse x illumination
         Cdiff += (sg->Li * diffterm * sg->we) * direct_c;
      }

      if (spec > 0 && AiLightGetAffectSpecular(sg->Lp))
      {
         AtVector H = sg->Ld + V;
         AiV3Normalize(H, H);
         AtFloat HdotT = (float)AiV3Dot(H, T);
         AtFloat s = 1 - HdotT * HdotT;
         if (s > 0)
         {
            // note: s holds sin^2 of the angle between H and T
            //       we don't need to take the sqrt, because we
            //       compensate by halving the gloss factor
            s = powf(s, gloss * 0.5f);
            // specular exponent x illumination
            Cspec += sg->Li * s * sg->we;
         }
      }
   }

   // indirect diffuse or ambient
   // if kd_ind = 0 then use ambient
   // FIXME: we should be checking for the arnold diffuse depth before we use Indirect gi
   //
   AtFloat kd_ind = AiShaderEvalParamFlt(p_kd_ind);
   if (kd_ind > 0)
      Cdiff += (kd_ind * AiIndirectDiffuse(&V,sg)) * indirect_c;
   else
      Cdiff +=  AiShaderEvalParamRGB(p_ambient);


   Cdiff *= diff_color;
   Cspec *= spec * spec_color;
   sg->out.RGB = Cdiff + Cspec;
}

node_loader
{
   if (i > 0)
      return FALSE;

   node->methods      = ShaveHairMtd;
   node->output_type  = AI_TYPE_RGB;
   node->name         = "ShaveHair";
   node->node_type    = AI_NODE_SHADER;
   strcpy(node->version, AI_VERSION);
   return TRUE;
}
