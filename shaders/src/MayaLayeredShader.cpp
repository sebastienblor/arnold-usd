#include <ai.h>

#include "MayaUtils.h"

#include <vector>
#include <iostream>

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
      CF_SHADER_ADD = 0,
      CF_TEXTURE_MIX
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
   AiMetaDataSetBool(nentry, NULL, "maya.attrs", false);
   
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
   AtRGB layerTransparency = AI_RGB_BLACK;

   // Note: A layer's transparency is actually 1 - weight, not related to transparency at all
   // Note: The compositing mix mode Shader / Texture means actually  Add / Mix

   // This will provide the accumulated visible opacity for each layer
   AtRGB currentOpacity = AI_RGB_WHITE;
   
   // loop over all layers
   for (unsigned int i = 0; i < numInputs; ++i)
   {      
      // useTransparency is OFF when the connected shader for this layer's transparency
      // is the same as the one connected in the colo
      bool useTransparency = AiShaderEvalParamBool(p_useTransparency0 + i);
      if (useTransparency)
      {
         layerTransparency = AiShaderEvalParamRGB(p_transparency0 + i);
         // If my mix mode is CF_SHADER_ADD, the layer's transparency won't affect the weight of the current layer
         // so we always want to evaluate it. Otherwise, if the mix mode is CF_TEXTURE_MIX and layerTransparency is white
         // then we can directly skip this texture
         if (flag == CF_TEXTURE_MIX && AiMax(layerTransparency.r, layerTransparency.g, layerTransparency.b) > 1.f - AI_EPSILON)
            continue; // passthrough for next layer
         
      }

      // evaluate this layer's closure 
      AtClosureList layerClosure = AiShaderEvalParamClosure(p_color0 + i);
      // If use Transparency is OFF, we need to get this layer's shader transparency weight
      // So we accumulate all its transparent closures
      if (!useTransparency)
      {
         layerTransparency = AI_RGB_BLACK;
         for (AtClosure closure = layerClosure.front(); closure; closure = closure.next())
            if (closure.type() == AI_CLOSURE_TRANSPARENT)
               layerTransparency += closure.weight();

      }

      // in texture mix mode, we attenuate this layer by it weight = 1 - transparency
      if (flag == CF_TEXTURE_MIX)
      {
         AtRGB negMix = AI_RGB_WHITE - layerTransparency;
         layerClosure *= negMix * currentOpacity;
      } else
         layerClosure *= currentOpacity;   
            
      // now we accumulate this layer's closures to the final result.
      // We need to take the current opacity into account, which represents 
      // the transparency of all previous layers
      result.add(layerClosure);

      // now we attenuate the current Opacity with this layer's transparency,
      currentOpacity *= layerTransparency;

      // the accumulated transparency of all layers so far makes the current opacity 
      // too small. So there's no need to continue, the following layers won't have any effect
      if (AiMax(currentOpacity.r, currentOpacity.g, currentOpacity.b) < AI_EPSILON)
         break;

   }

   sg->out.CLOSURE() = result;
}
