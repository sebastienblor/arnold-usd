#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaSurfaceLuminanceMtd);

node_parameters
{
   AiMetaDataSetStr(nentry, NULL, "maya.name", "surfaceLuminance");
   AiMetaDataSetInt(nentry, NULL, "maya.id", 1381190741);   
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
   AtRGB diffuse = AI_RGB_BLACK;

   AtLightSample light_sample;
   AiLightsPrepare(sg);

   while (AiLightsGetSample(sg, light_sample))
   {
      const float NDL = AiV3Dot(sg->Nf, sg->light_filter->Ld);

      if (NDL > 0.0f)
      {
         float d = AiLightGetDiffuse(light_sample.Lp);
         if (d > AI_EPSILON)
         {
            AtRGB Li_over_pdf = light_sample.Li / light_sample.pdf;
        	   diffuse += Li_over_pdf * NDL * d;
         }
      }
   }

   sg->out.FLT() = Luminance(diffuse);
}
