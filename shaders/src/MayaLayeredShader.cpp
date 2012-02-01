#include <ai.h>

#include "MayaUtils.h"

AI_SHADER_NODE_EXPORT_METHODS(MayaLayeredShaderMtd);

namespace
{
   enum MayaLayeredShaderParams
   {
      p_compositingFlag = 0,
      p_color,
      p_transparency
   };

   enum CompositingFlag
   {
      CF_SHADER = 0,
      CF_TEXTURE
   };

   static const char* gs_CompositingFlagNames[] =
   {
      "shader",
      "texture",
      0,
   };
}

node_parameters
{
   AiParameterENUM("compositingFlag", 0, gs_CompositingFlagNames);
   AiParameterARRAY("color", AiArray(0, 0, AI_TYPE_RGBA));
   AiParameterARRAY("transparency", AiArray(0, 0, AI_TYPE_RGB));
  	
   AiMetaDataSetBool(mds, NULL, "maya.hide", true);
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
   int flag = AiShaderEvalParamInt(p_compositingFlag);

   AtArray* colors = AiShaderEvalParamArray(p_color);
   AtArray* transparencies = AiShaderEvalParamArray(p_transparency);
   unsigned int numInputs = colors->nelements;

   AtRGBA outColor = AI_RGBA_BLACK;
   AtRGB outOpacity = AI_RGB_WHITE;

   if (numInputs > 0)
   {
      AtRGB curColor = AI_RGB_BLACK;
      AtRGB curOpacity = AI_RGB_BLACK;

      for (unsigned int i = 0; i < numInputs; ++i)
      {
         AtRGB opacity = 1.0f - AiArrayGetRGB(transparencies, i);
         AtRGBA colorAlpha = AiArrayGetRGBA(colors, i);

         // Maya layer premults color by input shader opacity
         // FIXME: we need actual opacity for each shader
         // color *= sg->out_opacity;
         AtRGB color;
         color.r = colorAlpha.r;
         color.g = colorAlpha.g;
         color.b = colorAlpha.b;

         if (flag == CF_TEXTURE)
         {
            color *= opacity;
         }

         curColor += (1.0f - curOpacity) * color;
         curOpacity += (1.0f - curOpacity) * opacity;

      }

      outColor.r = curColor.r;
      outColor.g = curColor.g;
      outColor.b = curColor.b;
      // Shader resulting opacity
      outOpacity = curOpacity;

      // Un-premultiply output color ans set its alpha
      if (outOpacity.r > AI_EPSILON)
      {
         outColor.r /= outOpacity.r;
      }
      if (outOpacity.g > AI_EPSILON)
      {
         outColor.g /= outOpacity.g;
      }
      if (outOpacity.b > AI_EPSILON)
      {
         outColor.b /= outOpacity.b;
      }
      outColor.a = CLAMP(Luminance(outOpacity), 0.0f, 1.0f);
   }

   sg->out.RGBA = outColor;
   sg->out_opacity = outOpacity;

}
