#include <ai.h>
#include "utils.h"

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
   AiParameterFLT("alphaA", 1.0f);
   AiParameterRGB("colorB", 0.0f, 1.0f, 0.0f);
   AiParameterFLT("alphaB", 1.0f);
   AiParameterENUM("operation", COP_ADD, CompositeOperationNames);
   AiParameterFLT("factor", 1.0f);

   AiMetaDataSetStr(mds, NULL, "maya.name", "colorComposite");
   AiMetaDataSetInt(mds, NULL, "maya.id", 0x81630);
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
      sg->out.RGBA.rgb() = colorA;
      sg->out.RGBA.a = alphaA;
   }
   else
   {
      const AtRGB colorB = AiShaderEvalParamRGB(p_colorB);
      const float alphaB = AiShaderEvalParamFlt(p_alphaB);
      const int op = AiShaderEvalParamEnum(p_operation);
      switch (op)
      {
      case COP_ADD:
         sg->out.RGBA.r = CompositeAdd(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeAdd(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeAdd(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeAdd(alphaA, alphaB, factor);
         break;
      case COP_SUBTRACT:
         sg->out.RGBA.r = CompositeSubtract(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeSubtract(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeSubtract(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeSubtract(alphaA, alphaB, factor);
         break;
      case COP_MIX:
         sg->out.RGBA.r = CompositeMix(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeMix(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeMix(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeMix(alphaA, alphaB, factor);
         break;
      case COP_MULTIPLY:
         sg->out.RGBA.r = CompositeMultiply(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeMultiply(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeMultiply(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeMultiply(alphaA, alphaB, factor);
         break;
      case COP_SCREEN:
         sg->out.RGBA.r = CompositeScreen(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeScreen(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeScreen(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeScreen(alphaA, alphaB, factor);
         break;
      case COP_OVERLAY:
         sg->out.RGBA.r = CompositeOverlay(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeOverlay(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeOverlay(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeOverlay(alphaA, alphaB, factor);
         break;
      case COP_DIFFERENCE:
         sg->out.RGBA.r = CompositeDifference(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeDifference(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeDifference(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeDifference(alphaA, alphaB, factor);
         break;
      case COP_DODGE:
         sg->out.RGBA.r = CompositeDodge(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeDodge(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeDodge(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeDodge(alphaA, alphaB, factor);
         break;
      case COP_BURN:
         sg->out.RGBA.r = CompositeBurn(colorA.r, colorB.r, factor);
         sg->out.RGBA.g = CompositeBurn(colorA.g, colorB.g, factor);
         sg->out.RGBA.b = CompositeBurn(colorA.b, colorB.b, factor);
         sg->out.RGBA.a = CompositeBurn(alphaA, alphaB, factor);
         break;
      default:
         AiRGBAReset(sg->out.RGBA);
         break;
      }
   }
}
