#include <ai.h>
#include "simplex.h"
#include "utils.h"

AI_SHADER_NODE_EXPORT_METHODS(ldkSimplexNoiseMtd);

namespace
{

enum ldkSimplexNoiseParams
{
   p_uvCoord,
   p_uvFilterSize,
   p_scale,
   p_amplitude,
   p_threshold,
   p_ratio,
   p_octaves,
   p_frequency,
   p_frequencyRatio,
   p_distortionU,
   p_distortionV,
   p_distortionRatio,
   p_gamma,
   p_noiseType
};

enum NoiseType
{
   NT_FRACTAL = 0,
   NT_CELLULAR,
   NT_RIDGED
};

const char* NoiseTypeNames[] =
{
   "Fractal",
   "Cellular",
   "Ridged",
   NULL
};

};

node_parameters
{
   AiParameterPnt2("uvCoord", 0.0f, 0.0f);
   AiParameterPnt2("uvFilterSize", 0.0f, 0.0f);
   AiParameterFlt("scale", 6.0f);
   AiParameterFlt("amplitude", 1.0f);
   AiParameterFlt("threshold", 0.0f);
   AiParameterFlt("ratio", 0.707f);
   AiParameterInt("octaves", 3);
   AiParameterFlt("frequency", 2.0f);
   AiParameterFlt("frequencyRatio", 1.0f);
   AiParameterFlt("distortionU", 0.0f);
   AiParameterFlt("distortionV", 0.0f);
   AiParameterFlt("distortionRatio", 0.0f);
   AiParameterFlt("gamma", 1.0f);
   AiParameterEnum("noiseType", NT_FRACTAL, NoiseTypeNames);

   AiMetaDataSetStr(mds, NULL, "maya.name", "simplexNoise");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x81654);
}

node_initialize
{
}

node_update
{
   // Unconnected render attributes (uvCoords, normalCamera, etc)
   // should use globals as following ldk's behavior
//   if (!AiNodeGetLink(node, "uvCoord"))
//   {
//      AtPoint2 uv = AI_P2_ZERO;
//      if (!AiNodeGetLink(node, "uvCoord.x")) uv.x = UV_GLOBALS;
//      if (!AiNodeGetLink(node, "uvCoord.y")) uv.y = UV_GLOBALS;
//      AiNodeSetPnt2(node, "uvCoord", uv.x, uv.y);
//   }
}

node_finish
{
}

shader_evaluate
{
   AtPoint2 uv = AiShaderEvalParamPnt2(p_uvCoord);
   //AtPoint2 uvFilterSize = AiShaderEvalParamPnt2(p_uvFilterSize);  NOT USED ?
   const float scale = AiShaderEvalParamFlt(p_scale);
   const float amplitude = AiShaderEvalParamFlt(p_amplitude);
   const float threshold = AiShaderEvalParamFlt(p_threshold);
   const float ratio = AiShaderEvalParamFlt(p_ratio);
   const int octaves = AiShaderEvalParamInt(p_octaves);
   const float frequency = AiShaderEvalParamFlt(p_frequency);
   const float frequencyRatio = AiShaderEvalParamFlt(p_frequencyRatio);
   const float distortionU = AiShaderEvalParamFlt(p_distortionU);
   const float distortionV = AiShaderEvalParamFlt(p_distortionV);
   const float distortionRatio = AiShaderEvalParamFlt(p_distortionRatio);
   const float gamma = AiShaderEvalParamFlt(p_gamma);
   const int noiseType = AiShaderEvalParamEnum(p_noiseType);

   // Will be set to GLOBALS by update if unconnected
//   if (uv.x == UV_GLOBALS) uv.x = sg->u;
//   if (uv.y == UV_GLOBALS) uv.y = sg->v;

   if (scale > AI_EPSILON)
   {
      uv.x *= scale;
      uv.y *= scale;
   }

   float result = 0.0f;

   switch(noiseType)
   {
      case NT_FRACTAL:
         result = simplex::FractalNoise(uv.x, uv.y, amplitude, ratio, octaves, frequency, frequencyRatio, distortionU, distortionV, distortionRatio);
         if (result < 0.0f)
             result = 0.0f;
         break;
      case NT_CELLULAR:
         if (distortionRatio > AI_EPSILON)
         {
            uv.x += distortionU * distortionRatio;
            uv.y += (distortionV + 0.2f) * distortionRatio;
         }
         result = simplex::CellNoise(uv.x, uv.y, amplitude, frequency);
         break;
      case NT_RIDGED:
         result = simplex::RidgedNoise(uv.x, uv.y, amplitude, ratio, octaves, frequency, frequencyRatio, distortionU, distortionV, distortionRatio);
         break;
   }

   result += threshold;

   if (gamma != 1.0f)
      result = Gamma(result, gamma);

   if (result < 0.0f)
      result = 0.0f;

   sg->out.RGBA.r = result;
   sg->out.RGBA.g = result;
   sg->out.RGBA.b = result;
   sg->out.RGBA.a = result;
}
