#include <ai.h>

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaCheckerMtd);

namespace
{

enum MayaCheckerParams
{
   p_color1,
   p_color2,
   p_contrast,
   p_uvCoord,
   p_repeatUV,
   MAYA_COLOR_BALANCE_ENUM,
   MAYA_FILTER_ENUM
};

};

node_parameters
{
   AiParameterRGB("color1", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("color2", 0.0f, 0.0f, 0.0f);
   AiParameterFLT("contrast", 1.0);
   AiParameterPNT2("uvCoord", 0.0f, 0.0f);
   AiParameterPNT2("repeatUV", 1.0f, 1.0f); // for filtering
   MAYA_COLOR_BALANCE_PARAMS
   MAYA_FILTER_PARAMS
   
   AiMetaDataSetStr(mds, NULL, "maya.counterpart", "checker");
   AiMetaDataSetInt(mds, NULL, "maya.counterpart_id", 0x52544348);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   AtPoint2 uv;

   uv.x = sg->u;
   uv.y = sg->v;

   if (AiNodeGetLink(node, "uvCoord") ||
       AiNodeGetLink(node, "uvCoord.x") ||
       AiNodeGetLink(node, "uvCoord.y"))
   {
      uv = AiShaderEvalParamPnt2(p_uvCoord);
   }

   AtPoint2 repeatUV = AiShaderEvalParamPnt2(p_repeatUV);
   AtRGB color1 = AiShaderEvalParamRGB(p_color1);
   AtRGB color2 = AiShaderEvalParamRGB(p_color2);
   EVAL_MAYA_COLOR_BALANCE_PARAMS
   EVAL_MAYA_FILTER_PARAMS

   if (!IsValidUV(uv.x, uv.y))
   {
      MAYA_DEFAULT_COLOR(sg->out.RGBA);
      return;
   }

   float ss = fmod(uv.x, 1.000001f);
   float tt = fmod(uv.y, 1.000001f);

   // Filterwidth
   //float fw = static_cast<float>(MAX(sqrt(sg->area), AI_EPSILON));
   //fw = fw * filter + filterOffset * 2.0f;
   
   // Set fw to zero at the moment 
   float fw = 0.0f; 

   float dss = fw * repeatUV.x;
   float dtt = fw * repeatUV.y;

   float f = 0.5f - 2.0f *
             (FilteredPulseTrain(0.5, 1, ss, dss) - 0.5f) *
             (FilteredPulseTrain(0.5, 1, tt, dtt) - 0.5f);

   f = 0.5f + (f - 0.5f) * AiShaderEvalParamFlt(p_contrast);

   sg->out.RGBA.r = color1.r + (color2.r - color1.r) * f;
   sg->out.RGBA.g = color1.g + (color2.g - color1.g) * f;
   sg->out.RGBA.b = color1.b + (color2.b - color1.b) * f;
   sg->out.RGBA.a = 1.0f - f;

   MAYA_COLOR_BALANCE(sg->out.RGBA);
}
