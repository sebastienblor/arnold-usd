#include <ai.h>

#include "MayaUtils.h"

#include <vector>

AI_SHADER_NODE_EXPORT_METHODS(MayaLayeredShaderMtd);

namespace
{
   enum MayaLayeredShaderParams
   {
      p_compositingFlag = 0,
      p_numInputs,
      p_color0,
      p_color1,
      p_color2,
      p_color3,
      p_color4,
      p_color5,
      p_color6,
      p_color7,
      p_color8,
      p_color9,
      p_color10,
      p_color11,
      p_color12,
      p_color13,
      p_color14,
      p_color15,
      p_transparency0,
      p_transparency1,
      p_transparency2,
      p_transparency3,
      p_transparency4,
      p_transparency5,
      p_transparency6,
      p_transparency7,
      p_transparency8,
      p_transparency9,
      p_transparency10,
      p_transparency11,
      p_transparency12,
      p_transparency13,
      p_transparency14,
      p_transparency15,
      p_useTransparency0,
      p_useTransparency1,
      p_useTransparency2,
      p_useTransparency3,
      p_useTransparency4,
      p_useTransparency5,
      p_useTransparency6,
      p_useTransparency7,
      p_useTransparency8,
      p_useTransparency9,
      p_useTransparency10,
      p_useTransparency11,
      p_useTransparency12,
      p_useTransparency13,
      p_useTransparency14,
      p_useTransparency15,
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
   AiParameterEnum("compositingFlag", 0, gs_CompositingFlagNames);
   AiParameterUInt("numInputs", 0);
   AiParameterClosure("color0");
   AiParameterClosure("color1");
   AiParameterClosure("color2");
   AiParameterClosure("color3");
   AiParameterClosure("color4");
   AiParameterClosure("color5");
   AiParameterClosure("color6");
   AiParameterClosure("color7");
   AiParameterClosure("color8");
   AiParameterClosure("color9");
   AiParameterClosure("color10");
   AiParameterClosure("color11");
   AiParameterClosure("color12");
   AiParameterClosure("color13");
   AiParameterClosure("color14");
   AiParameterClosure("color15");
   AiParameterRGB("transparency0", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency1", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency2", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency3", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency4", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency5", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency6", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency7", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency8", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency9", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency10", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency11", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency12", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency13", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency14", 1.0f, 1.0f, 1.0f);
   AiParameterRGB("transparency15", 1.0f, 1.0f, 1.0f);
   AiParameterBool("useTransparency0", false);
   AiParameterBool("useTransparency1", false);
   AiParameterBool("useTransparency2", false);
   AiParameterBool("useTransparency3", false);
   AiParameterBool("useTransparency4", false);
   AiParameterBool("useTransparency5", false);
   AiParameterBool("useTransparency6", false);
   AiParameterBool("useTransparency7", false);
   AiParameterBool("useTransparency8", false);
   AiParameterBool("useTransparency9", false);
   AiParameterBool("useTransparency10", false);
   AiParameterBool("useTransparency11", false);
   AiParameterBool("useTransparency12", false);
   AiParameterBool("useTransparency13", false);
   AiParameterBool("useTransparency14", false);
   AiParameterBool("useTransparency15", false);

   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
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
   unsigned int numInputs = AiShaderEvalParamUInt(p_numInputs);
   int flag = AiShaderEvalParamInt(p_compositingFlag);

   AtClosureList result;
   AtRGB curOpacity = AI_RGB_BLACK;

   if (numInputs > 0)
   {      

      for (unsigned int i = 0; i < numInputs; ++i)
      {
         AtRGB transparency = AI_RGB_BLACK;
         bool useTransparency = AiShaderEvalParamBool(p_useTransparency0 + i);
         
         if (useTransparency && (curOpacity.r < 1.f || curOpacity.g < 1.f || curOpacity.b < 1.f))
            transparency = AiShaderEvalParamRGB(p_transparency0 + i);
         if ((curOpacity.r < 1.f || curOpacity.g < 1.f || curOpacity.b < 1.f ) &&
            ((flag != CF_TEXTURE)  || (transparency.r < 1.f || transparency.g < 1.f || transparency.b < 1.f)))
         {
            AtClosureList closures = AiShaderEvalParamClosure(p_color0 + i);

         // use transparency from closure instead of from parameter
            if (!useTransparency)
            {
            transparency = AI_RGB_BLACK;
            for (AtClosure closure = closures.front(); closure; closure = closure.next())
               if (closure.type() == AI_CLOSURE_TRANSPARENT)
                  transparency += closure.weight();

            // remove transparency, we add our own at the end
            if (transparency != AI_RGB_BLACK)
               closures.add(AiClosureTransparent(sg, -transparency));
         }

         AtRGB opacity = AI_RGB_WHITE - transparency;

         // TODO texture makes no sense to me, this should actually be changed to
         // if (useTransparency), so that whenever the transparency is used it is
         // properly premultiplied in
            if (flag == CF_TEXTURE)
            {
            // premultiply opacity
            closures *= opacity * (1.0f - curOpacity);
            }
         else
         {
            // consider opacity pre-multiplied already
            closures *= (1.0f - curOpacity);
         }

         result.add(closures);
            curOpacity += (1.0f - curOpacity) * opacity;
         }
      }
   }

   if (curOpacity != AI_RGB_WHITE)
   {
      result *= curOpacity;
      result.add(AiClosureTransparent(sg, 1.0f - curOpacity));
   }

   sg->out.CLOSURE() = result;

   // NOTES:
   // removed AOV blending, which closures handle automatically
   // removed setting of result.a, which is matte alpha and made no sense
}
