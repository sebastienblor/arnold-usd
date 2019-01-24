#include <ai.h>
#include "../utils.h"

AI_SHADER_NODE_EXPORT_METHODS(ldkColorCompositeMtd);

namespace
{

enum ldkColorCompositeParams
{
   p_colorA,
   p_alphaA,
   p_colorB,
   p_alphaB,
   p_operation,
   p_factor
};

};

node_parameters
{
   AiParameterRGB("colorA", 1.0f, 0.0f, 0.0f);
   AiParameterFlt("alphaA", 1.0f);
   AiParameterRGB("colorB", 0.0f, 1.0f, 0.0f);
   AiParameterFlt("alphaB", 1.0f);
   AiParameterEnum("operation", COP_ADD, CompositeOperationNames);
   AiParameterFlt("factor", 1.0f);

   AiMetaDataSetStr(nentry, NULL, "maya.name", "colorComposite");
   AiMetaDataSetBool(nentry, NULL, "maya.hide", true);
   AiMetaDataSetBool(nentry, NULL, "deprecated", true);
   //AiMetaDataSetInt(nentry, NULL, "maya.id", 0x81630);
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
   const AtRGB colorA = AiShaderEvalParamRGB(p_colorA);
   const float alphaA = AiShaderEvalParamFlt(p_alphaA);
   const float factor = AiShaderEvalParamFlt(p_factor);

   if (factor <= 0.0f)
   {
      sg->out.RGB() = colorA; 
      sg->out.RGBA().a = alphaA;
   }
   else
   {
      const AtRGB colorB = AiShaderEvalParamRGB(p_colorB);
      const float alphaB = AiShaderEvalParamFlt(p_alphaB);
      const int op = AiShaderEvalParamEnum(p_operation);
      switch (op)
      {
      case COP_ADD:
         sg->out.RGBA() = AtRGBA(CompositeAdd(colorA.r, colorB.r, factor),
               CompositeAdd(colorA.g, colorB.g, factor),
               CompositeAdd(colorA.b, colorB.b, factor),
               CompositeAdd(alphaA, alphaB, factor));
         break;
      case COP_SUBTRACT:
         sg->out.RGBA() = AtRGBA(CompositeSubtract(colorA.r, colorB.r, factor),
            CompositeSubtract(colorA.g, colorB.g, factor),
            CompositeSubtract(colorA.b, colorB.b, factor),
            CompositeSubtract(alphaA, alphaB, factor));
         break;
      case COP_MIX:
         sg->out.RGBA() = AtRGBA(CompositeMix(colorA.r, colorB.r, factor), 
            CompositeMix(colorA.g, colorB.g, factor),
            CompositeMix(colorA.b, colorB.b, factor),
            CompositeMix(alphaA, alphaB, factor));
         break;
      case COP_MULTIPLY:
         sg->out.RGBA() = AtRGBA(CompositeMultiply(colorA.r, colorB.r, factor),
            CompositeMultiply(colorA.g, colorB.g, factor),
            CompositeMultiply(colorA.b, colorB.b, factor),
            CompositeMultiply(alphaA, alphaB, factor));
         break;
      case COP_SCREEN:
         sg->out.RGBA() = AtRGBA(CompositeScreen(colorA.r, colorB.r, factor),
            CompositeScreen(colorA.g, colorB.g, factor),
            CompositeScreen(colorA.b, colorB.b, factor),
            CompositeScreen(alphaA, alphaB, factor));
         break;
      case COP_OVERLAY:
         sg->out.RGBA() = AtRGBA(CompositeOverlay(colorA.r, colorB.r, factor),  
            CompositeOverlay(colorA.g, colorB.g, factor),
            CompositeOverlay(colorA.b, colorB.b, factor),
            CompositeOverlay(alphaA, alphaB, factor));
         break;
      case COP_DIFFERENCE:
         sg->out.RGBA() = AtRGBA(CompositeDifference(colorA.r, colorB.r, factor),
            CompositeDifference(colorA.g, colorB.g, factor),
            CompositeDifference(colorA.b, colorB.b, factor),
            CompositeDifference(alphaA, alphaB, factor));
         break;
      case COP_DODGE:
         sg->out.RGBA() = AtRGBA(CompositeDodge(colorA.r, colorB.r, factor),
            CompositeDodge(colorA.g, colorB.g, factor),
            CompositeDodge(colorA.b, colorB.b, factor),
            CompositeDodge(alphaA, alphaB, factor));
         break;
      case COP_BURN:
         sg->out.RGBA() = AtRGBA(CompositeBurn(colorA.r, colorB.r, factor),
            CompositeBurn(colorA.g, colorB.g, factor),
            CompositeBurn(colorA.b, colorB.b, factor),
            CompositeBurn(alphaA, alphaB, factor));
         break;
      default:
      // FIXME Arnold5 we used to call AiRGBAReset 
         sg->out.RGBA() = AI_RGBA_ZERO;
         break;
      }
   }
}
