//
// Simple hair shader, roughly based on Kay/Kajiya's shading model
//

#include <ai.h>

#include <cstring>
#include <string>

AI_SHADER_NODE_EXPORT_METHODS(ShaveHairMtd);

enum ShaveHairParams
{
   p_rootcolor,
   p_tipcolor,
   p_strand_opacity,
   p_ambdiff,
   p_ambient,
   p_gloss,
   p_spec_color,
   p_spec,
   p_kd_ind,
   p_direct_diffuse,
   p_indirect_diffuse,
   p_aov_direct_diffuse,
   p_aov_direct_specular,
   p_aov_indirect_diffuse
};

node_parameters
{
   AiParameterStr(       "rootcolor"        , NULL);
   AiParameterStr(       "tipcolor"         , NULL);
   AiParameterRGB(       "strand_opacity"   , 1.0f, 1.0f, 1.0f);
   AiParameterFlt(       "ambdiff"          , 1.0f);
   AiParameterRGB(       "ambient"          , 1.0f, 1.0f, 1.0f);
   AiParameterFlt(       "gloss"            , 10.0f);
   AiParameterRGB(       "spec_color"       , 1.0f, 1.0f, 1.0f);
   AiParameterFlt(       "spec"             , 1.0f);
   AiParameterFlt(       "kd_ind"           , 1.0f);

   AiMetaDataSetFlt(nentry, "kd_ind"           , "softmax", 10.0f);
   AiMetaDataSetFlt(nentry, "kd_ind"           , "min",     0.0f);
   AiParameterFlt(       "direct_diffuse"   , 1.0f);
   AiMetaDataSetFlt(nentry, "direct_diffuse"   , "softmax", 1.0f);
   AiMetaDataSetFlt(nentry, "direct_diffuse"   , "min",     0.0f);
   AiParameterFlt(       "indirect_diffuse" , 1.0f);
   
   AiParameterStr ( "aov_direct_diffuse"             , "direct_diffuse"    );
   AiMetaDataSetInt(nentry, "aov_direct_diffuse"        , "aov.type", AI_TYPE_RGB);
   AiParameterStr ( "aov_direct_specular"            , "direct_specular"   );
   AiMetaDataSetInt(nentry, "aov_direct_specular"       , "aov.type", AI_TYPE_RGB);
   AiParameterStr ( "aov_indirect_diffuse"           , "indirect_diffuse"  );
   AiMetaDataSetInt(nentry, "aov_indirect_diffuse"      , "aov.type", AI_TYPE_RGB);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "shaveHair");
}

typedef struct
{
   int                max_diffuse_depth;
}
ShaderData;

node_initialize
{
   ShaderData *data = (ShaderData*) AiMalloc(sizeof(ShaderData));
   AiNodeSetLocalData(node, data);
}

node_update
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   AtNode *options = AiUniverseGetOptions();
   data->max_diffuse_depth = AiNodeGetInt(options, "GI_diffuse_depth");

}

node_finish
{
   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
   AiFree(data);
}

shader_evaluate
{
   AtRGB opacity = AiShaderEvalParamRGB(p_strand_opacity);

   // This piece of user-data is automatically set by the curves node when
   // using auto-enlargement (min_pixel_width > 0)
   float geo_opacity;
   static AtString geo_opacityStr("geo_opacity");
   if (AiUDataGetFlt(geo_opacityStr, geo_opacity))
      opacity *= geo_opacity;
      
      
   // early out for shadow rays and totally transparent objects
   if (sg->Rt & AI_RAY_SHADOW)
      return;

   AtVector V = -sg->Rd;
   AtVector T = AiV3Normalize(sg->dPdv);

   AtRGB Cdiff = AI_RGB_BLACK;
   AtRGB Cspec = AI_RGB_BLACK;


   float ambdiff    = AiShaderEvalParamFlt(p_ambdiff);
   float gloss      = AiShaderEvalParamFlt(p_gloss) * 2000;
   float spec       = AiShaderEvalParamFlt(p_spec);

   float direct_c   = AiShaderEvalParamFlt(p_direct_diffuse);
   float indirect_c = AiShaderEvalParamFlt(p_indirect_diffuse);

   AtRGB spec_color = AiShaderEvalParamRGB(p_spec_color);
   AtRGB root_color;
   AtRGB tip_color;

   ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);


   AiUDataGetRGB(AiShaderEvalParamStr(p_rootcolor), root_color);
   AiUDataGetRGB(AiShaderEvalParamStr(p_tipcolor), tip_color);

   

   // mix root and tip colors
   AtRGB diff_color = (sg->v * tip_color) + ((1.f - sg->v) * root_color);
   
   // Since the curves are represented as ray-facing ribbons
   // their normal is usually pointing roughly towards the incoming ray
   // - but not always. It can have discontinuities - which then causes
   // some lights to be ignored if they happen to be on the wrong side of this
   // normal. Setting sg->fhemi forces the lights to be gathered regardless of
   // where the normal is pointing
   sg->fhemi = false;

   // direct lighting
   AiLightsPrepare(sg);
   AtLightSample light_sample;
   while (AiLightsGetSample(sg, light_sample))
   {
      float lDiff = AiLightGetDiffuse(light_sample.Lp);
      if (lDiff > 0)
      {
         float TdotL = (float)AiV3Dot(T, light_sample.Ld);
         float d = 1 - TdotL * TdotL;
         d = d > 0 ? sqrtf(d) : 0;
         float diffterm = (1 - ambdiff) + d * ambdiff;  // limits gamut of diffuse term
         // diffuse x illumination
         Cdiff += (light_sample.Li * diffterm) * direct_c * lDiff;
      }

      float lSpec = AiLightGetSpecular(light_sample.Lp);
      if (spec > 0 && lSpec > 0)
      {
         AtVector H = light_sample.Ld + V;
         H = AiV3Normalize(H);
         float HdotT = (float)AiV3Dot(H, T);
         float s = 1 - HdotT * HdotT;
         if (s > 0)
         {
            // note: s holds sin^2 of the angle between H and T
            //       we don't need to take the sqrt, because we
            //       compensate by halving the gloss factor
            s = powf(s, gloss * 0.5f);
            // specular exponent x illumination
            Cspec += light_sample.Li * s * lSpec;
         }
      }
   }
   Cdiff *= diff_color;
   AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_direct_diffuse), Cdiff);

   Cspec *= spec * spec_color;
   AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_direct_specular), Cspec);

   // indirect diffuse or ambient
   // if kd_ind = 0 then use ambient
   // FIXME: we should be checking for the arnold diffuse depth before we use Indirect gi
   //
   float kd_ind = AiShaderEvalParamFlt(p_kd_ind);
   AtRGB ind_diff;
   if (kd_ind > 0)
      ind_diff = (kd_ind * AiIndirectDiffuse(V,sg, AI_RGB_WHITE)) * indirect_c;
   else
      ind_diff = AiShaderEvalParamRGB(p_ambient);
   ind_diff *= diff_color;
   AiAOVSetRGB(sg, AiShaderEvalParamStr(p_aov_indirect_diffuse), ind_diff);
   Cdiff += ind_diff;

   sg->out.RGB() = Cdiff + Cspec;
}

node_loader
{
   if (i > 0)
      return false;

   node->methods      = ShaveHairMtd;
   node->output_type  = AI_TYPE_RGB;
   node->name         = "ShaveHair";
   node->node_type    = AI_NODE_SHADER;
   strcpy(node->version, AI_VERSION);
   return true;
}
