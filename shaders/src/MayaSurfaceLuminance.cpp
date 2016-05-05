#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaSurfaceLuminanceMtd);

node_parameters
{
   AiMetaDataSetStr(mds, NULL, "maya.name", "surfaceLuminance");
   AiMetaDataSetInt(mds, NULL, "maya.id", 1381190741);   
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
   AtColor diffuse = AI_RGB_BLACK;

   AiLightsPrepare(sg);

   while (AiLightsGetSample(sg))
   {
      const float NDL = AiV3Dot(sg->Nf, sg->Ld);

      if (NDL > 0.0f)
      {
         if (AiLightGetAffectDiffuse(sg->Lp))
         {
         	const float d = AiLightGetDiffuse(sg->Lp);
         	if (d > AI_EPSILON)
            	diffuse += sg->Li * NDL * sg->we * d;
         }
      }
   }

   sg->out.FLT() = Luminance(diffuse);
}
